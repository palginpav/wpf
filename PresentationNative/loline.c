#include <stdio.h>

#include "loservice.h"

struct ObjDim
{
    struct LsHeights    heightsRef;
    struct LsHeights    heightsPres;
    INT                 dur;
};

typedef enum LsErr WINAPI (*InlineFormat)(
        void *                      pols,               // Line Layout context
        void *                      plsrun,             // plsrun
        INT                         lscpInline,         // first cp of the run
        INT                         currentPosition,    // inline's current pen location in text direction
        INT                         rightMargin,        // right margin
        struct ObjDim *             pobjDim,            // object dimension
        INT *                       fFirstRealOnLine,   // is this run the first in line; logically boolean
        INT *                       fPenPositionUsed,   // is pen position used to format object; logically boolean
        enum LsBrkCond *            breakBefore,        // break condition before this object
        enum LsBrkCond *            breakAfter          // break condition after this object
        );

typedef enum LsErr WINAPI (*InlineDraw)(
        void *                      pols,               // Line Layout context
        void *                      plsrun,             // plsrun
        struct LSPOINT *            runOrigin,          // pen position at which to render the object
        enum LsTFlow                textFlow,           // text flow direction
        INT                         runWidth            // object width
        );

struct InlineInit
{
    UINT             dwVersion;
    InlineFormat     pfnFormat;
    InlineDraw       pfnDraw;
};

enum Type
{
    TextType,
    InlineObjectType,
};

struct RunData
{
    enum Type type;
    INT start_cp;
    void *run;
    INT length;
    INT width;
};

struct LoLine
{
    struct LoContext *ploc;
    struct LSPOINT pt;
    INT x_offset;
    struct RunData *run_data;
    UINT num_runs;
    WCHAR *text;
    UINT num_chars;
    INT *glyph_advance;
    USHORT *cluster_map;
    USHORT *glyph_map;
    struct GlyphOffset *glyph_offset;
};

static BOOL apply_line_break(struct LoLine *loline, WCHAR wchSpace)
{
    struct RunData *run_data;
    BOOL break_done = FALSE;
    INT len, i, idx = 0;

    if (loline->num_runs)
    {
        len = loline->num_chars;
        while (len)
        {
            if (loline->text[len-1] == wchSpace)
                break;
            len--;
        }

        if (len)
        {
            /* We found whitespace, so now we need to find the run with this whitespace and adjust it.
             * We also need to remove any run that occurred after.
             */
            loline->num_chars = len;
            for (i = 0; i < loline->num_runs; i++)
            {
                run_data = loline->run_data + i;
                if (len > idx && len <= idx + run_data->length)
                {
                    /* We have found our run */
                    loline->num_runs = i + 1;
                    for (i = len; i < idx + run_data->length; i++)
                        run_data->width -= loline->glyph_advance[i];
                    run_data->length = len - idx;
                    break;
                }
                idx += run_data->length;
            }
            break_done = TRUE;
        }
    }

    return break_done;
}

static enum LsErr get_glyphs(struct LoLine *loline)
{
    USHORT *glyph_prop, *char_prop;
    INT idx, i, j, glyph_count;
    struct RunData *run_data;
    enum LsErr lserr = None;
    INT *alone;
    BOOL used;

    alone = calloc(loline->num_chars, sizeof(*alone));
    glyph_prop = calloc(loline->num_chars, sizeof(*glyph_prop));
    char_prop = calloc(loline->num_chars, sizeof(*char_prop));

    /* These allocations will be freed when loline is freed, even on error */
    loline->cluster_map = calloc(loline->num_chars, sizeof(*loline->cluster_map));
    loline->glyph_map = calloc(loline->num_chars, sizeof(*loline->glyph_map));
    loline->glyph_offset = calloc(loline->num_chars, sizeof(*loline->glyph_offset));

    if (!alone || !glyph_prop || !char_prop || !loline->cluster_map || !loline->glyph_map || !loline->glyph_offset)
    {
        lserr = OutOfMemory;
        goto error;
    }

    idx = 0;

    /* TODO: There is room for optimisation here. These calls can handle multiple runs in a single call, however, they expect the same font
     * size to be used (and possibly other properties too). So for now, we just make one call per run.
     */
    for (i = 0; i < loline->num_runs; i++)
    {
        run_data = loline->run_data + i;

        if (run_data->type != TextType)
            continue;

        /* Some fonts seem to fail this call. In which case we will fallback to DrawTextRun */
        if (loline->ploc->lscbkRedef.pfnGetGlyphsRedefined(loline, &run_data->run, &run_data->length, 1, &loline->text[idx],
                    run_data->length, lstflowDefault, &loline->glyph_map[idx], glyph_prop, run_data->length, &used, &loline->cluster_map[idx],
                    char_prop, alone, &glyph_count) < 0)
            goto skip_glyphs;

        if ((lserr = loline->ploc->contextInfo.pfnGetGlyphPositions(loline, &run_data->run, &run_data->length, 1, Presentation,
                        &loline->text[idx], &loline->cluster_map[idx], char_prop, run_data->length, &loline->glyph_map[idx], glyph_prop,
                        glyph_count, lstflowDefault, &loline->glyph_advance[idx], &loline->glyph_offset[idx])) < 0)
            goto error;

        /* The glyph_advance values can be modified here, so we need to recalculate the runs width */
        run_data->width = 0;

        for (j = 0; j < run_data->length; j++)
            run_data->width += loline->glyph_advance[idx + j];

skip_glyphs:
        idx += run_data->length;
    }

error:
    free(alone);
    free(glyph_prop);
    free(char_prop);

    return lserr;
}

enum LsErr WINAPI LoCreateLine(struct LoContext* ploc, INT cp, INT ccpLim, INT durColumn, UINT dwLineFlags,
        void* pInputBreakRec, struct LsLInfo* plslinfo, struct LoLine** pploline, INT* maxDepth, struct LsLineWidths* lineWidths)
{
    BOOL bufused, ishidden, eol = FALSE;
    enum LsEndRes endr = endrEndPara;
    struct LsLineProps line_props;
    struct RunData *run_data;
    INT total_width, chars;
    struct LoLine *loline;
    struct LsTxM metrics;
    WCHAR *text_pointer;
    struct LsChp lschp;
    struct LsPap pap;
    INT max_runs = 4;
    enum LsErr lserr;
    void *curr_run;
    INT max_chars;
    WCHAR *text;
    INT len;
    INT i;

    max_chars = ploc->contextInfo.cEstimatedCharsPerLine;
    loline = *pploline = calloc(1, sizeof(*loline));
    text = calloc(max_chars, sizeof(*text));

    if (!loline || !text)
        goto out_of_memory;

    loline->ploc = ploc;
    loline->run_data = calloc(max_runs, sizeof(*loline->run_data));
    loline->text = text;
    loline->glyph_advance = calloc(max_chars, sizeof(*loline->glyph_advance));

    if (!loline->run_data || !loline->glyph_advance)
        goto out_of_memory;

    if ((lserr = ploc->contextInfo.pfnFetchPap(ploc, cp, &pap)) < 0)
        goto error;

    /* This is an auto numbering run */
    if (pap.grpf & fFmiAnm)
    {
        struct LsChp chp, chpAddedChar;
        void *plsrun, *plsrunAddedChar;
        INT word95, anm_offset, width;
        enum LsKAlign alignment;
        USHORT addedChar;

        if ((lserr = ploc->contextInfo.pfnGetAutoNumberInfo(ploc, &alignment, &chp, &plsrun, &addedChar,
                        &chpAddedChar, &plsrunAddedChar, &word95, &anm_offset, &width)) < 0)
            goto error;
        /* The cp in an auto numbering run is always negative. This is used to determine that the marker store
         * should be used (see the StoreFrom method in FullTextState) */
        cp -= 0x7FFFFFFF;
    }

    do
    {
        if ((lserr = ploc->contextInfo.pfnFetchLineProps(ploc, cp, !loline->num_runs, &line_props)) < 0)
            goto error;

        if (!loline->num_runs)
        {
            loline->x_offset = lineWidths->upStartMainText = line_props.durLeft;
            durColumn -= line_props.durLeft;
        }

        if ((lserr = ploc->lscbkRedef.pfnFetchRunRedefined(ploc, cp, 0, NULL, text + loline->num_chars,
                        max_chars - loline->num_chars, &bufused,
                        &text_pointer, &len, &ishidden, &lschp, &curr_run)) < 0)
            goto error;

        if (bufused)
        {
            /* GetRunCharWidths will only ever return at most, one character that doesn't fully fit within the maximum width allowance */
            if ((lserr = ploc->contextInfo.pfnGetRunCharWidths(loline, curr_run, Presentation, text + loline->num_chars,
                            len, durColumn, lstflowDefault, loline->glyph_advance + loline->num_chars, &total_width, &chars)) < 0)
                goto error;

            if (pap.grpf & fFmiApplyBreakingRules && total_width > durColumn)
            {
                INT orig_width = total_width;

                eol = TRUE;
                endr = endrNormal;
                len = chars;

                /* We have exceeded the available column space and we need to apply line breaking.
                 * We will work backwards within the current run until we find whitespace on which to break.
                 */
                while (len)
                {
                    if (text[loline->num_chars+len-1] == ploc->contextInfo.wchSpace)
                        break;
                    len--;
                    total_width -= loline->glyph_advance[loline->num_chars+len];
                }

                /* If no whitespace was found */
                if (!len)
                {
                    /* We'll attempt the line break logic over the rest of the line */
                    if (apply_line_break(loline, ploc->contextInfo.wchSpace))
                    {
                        run_data = loline->run_data + loline->num_runs - 1;
                        cp = run_data->start_cp + run_data->length;
                        break;
                    }

                    /* Otherwise we'll just remove the last character (unless there's only one, and then we'll just keep it) */
                    len = chars;
                    total_width = orig_width;

                    if (loline->num_chars + len > 1)
                    {
                        len--;
                        total_width -= loline->glyph_advance[loline->num_chars+len];
                    }
                }
                chars = len;

                /* We don't want to add a run with zero chars */
                if (!chars)
                    break;
            }

            if (loline->num_runs == max_runs)
            {
                max_runs *= 2;
                run_data = realloc(loline->run_data, sizeof(*loline->run_data) * max_runs);

                if (!run_data)
                    goto out_of_memory;

                loline->run_data = run_data;
            }

            if (pap.grpf & fFmiAnm && !loline->num_runs)
                loline->x_offset = line_props.durLeft - total_width;

            run_data = loline->run_data + loline->num_runs;
            run_data->type = TextType;
            run_data->start_cp = cp;
            run_data->run = curr_run;
            run_data->length = chars;
            run_data->width = total_width;
            durColumn -= total_width;
            loline->num_runs++;
            loline->num_chars += chars;
        }
        else if (text_pointer)
        {
            eol = text_pointer[0] == ploc->contextInfo.wchEndPara1 || text_pointer[0] == ploc->contextInfo.wchEndLineInPara;

            /* 0xfffc indicates an inline object */
            if (text_pointer[0] == 0xfffc)
            {
                enum LsBrkCond break_before, break_after;
                struct InlineInit inline_init;
                struct ObjDim obj_dim;
                INT first, pen;

                if ((lserr = ploc->contextInfo.pfnGetObjectHandlerInfo(ploc, 1, &inline_init)) < 0)
                    goto error;

                if ((lserr = inline_init.pfnFormat(ploc, curr_run, cp, loline->num_chars, durColumn, &obj_dim,
                                &first, &pen, &break_before, &break_after)) < 0)
                    goto error;

                plslinfo->dvpAscent = max(plslinfo->dvrAscent, obj_dim.heightsPres.dvAscent);
                plslinfo->dvpDescent = max(plslinfo->dvrDescent, obj_dim.heightsPres.dvDescent);
                plslinfo->dvpMultiLineHeight = max(plslinfo->dvrMultiLineHeight, obj_dim.heightsPres.dvMultiLineHeight);

                plslinfo->dvrAscent = max(plslinfo->dvrAscent, obj_dim.heightsRef.dvAscent);
                plslinfo->dvrDescent = max(plslinfo->dvrDescent, obj_dim.heightsRef.dvDescent);
                plslinfo->dvrMultiLineHeight = max(plslinfo->dvrMultiLineHeight, obj_dim.heightsRef.dvMultiLineHeight);

                if (loline->num_runs == max_runs)
                {
                    max_runs *= 2;
                    run_data = realloc(loline->run_data, sizeof(*loline->run_data) * max_runs);

                    if (!run_data)
                        goto out_of_memory;

                    loline->run_data = run_data;
                }

                run_data = loline->run_data + loline->num_runs;
                run_data->type = InlineObjectType;
                run_data->start_cp = cp;
                run_data->run = curr_run;
                run_data->width = obj_dim.dur;
                loline->num_runs++;
                if (pap.grpf & fFmiApplyBreakingRules && obj_dim.dur > durColumn)
                {
                    eol = TRUE;
                    endr = endrNormal;
                    run_data->width = durColumn;
                }
                durColumn -= obj_dim.dur;
            }

            /* If there are no runs in this line, we still need to calculate its height */
            if (eol && !loline->num_runs)
            {
                ploc->contextInfo.pfnGetRunTextMetrics(ploc, curr_run, Presentation, lstflowDefault, &metrics);
                plslinfo->dvpAscent = metrics.dvAscent;
                plslinfo->dvpDescent = metrics.dvDescent;
                plslinfo->dvpMultiLineHeight = metrics.dvMultiLineHeight;

                ploc->contextInfo.pfnGetRunTextMetrics(loline, curr_run, Reference, lstflowDefault, &metrics);
                plslinfo->dvrAscent = metrics.dvAscent;
                plslinfo->dvrDescent = metrics.dvDescent;
                plslinfo->dvrMultiLineHeight = metrics.dvMultiLineHeight;
            }
        }
        else
        {
            INT *glyph_advance;

            /* We need to allocate more memory to fit the entire text */
            max_chars = loline->num_chars + len;
            text = realloc(loline->text, max_chars * sizeof(*text));
            if (!text)
            {
                text = loline->text;
                goto out_of_memory;
            }
            loline->text = text;

            glyph_advance = realloc(loline->glyph_advance, max_chars * sizeof(*loline->glyph_advance));
            if (!glyph_advance)
                goto out_of_memory;

            loline->glyph_advance = glyph_advance;
        }

        if (bufused || text_pointer)
            cp += len;
    }
    while (!eol);

    /* Attempt to get the glpyhs required for DrawGlyphs */
    if ((lserr = get_glyphs(loline) < 0))
        goto error;

    lineWidths->upLimLine = lineWidths->upStartMainText;

    for (i = 0; i < loline->num_runs; i++)
    {
        run_data = loline->run_data + i;

        lineWidths->upLimLine += run_data->width;

        if (run_data->type != TextType)
            continue;

        ploc->contextInfo.pfnGetRunTextMetrics(ploc, run_data->run, Presentation, lstflowDefault, &metrics);
        plslinfo->dvpAscent = max(plslinfo->dvpAscent, metrics.dvAscent);
        plslinfo->dvpDescent = max(plslinfo->dvpDescent, metrics.dvDescent);
        plslinfo->dvpMultiLineHeight = max(plslinfo->dvpMultiLineHeight, metrics.dvMultiLineHeight);

        ploc->contextInfo.pfnGetRunTextMetrics(loline, run_data->run, Reference, lstflowDefault, &metrics);
        plslinfo->dvrAscent = max(plslinfo->dvrAscent, metrics.dvAscent);
        plslinfo->dvrDescent = max(plslinfo->dvrDescent, metrics.dvDescent);
        plslinfo->dvrMultiLineHeight = max(plslinfo->dvrMultiLineHeight, metrics.dvMultiLineHeight);
    }

    lineWidths->upMinLimLine = lineWidths->upStartTrailing = lineWidths->upLimLine;

    if (loline->num_chars && loline->run_data[loline->num_runs-1].type == TextType)
    {
        run_data = loline->run_data + loline->num_runs - 1;

        len = loline->num_chars - 1;

        /* the start trailing width does not include whitespace */
        for (i = run_data->length - 1; i >= 0 && text[len] == ' '; i--)
            lineWidths->upStartTrailing -= loline->glyph_advance[len--];
    }

    lineWidths->upMinStartTrailing = lineWidths->upStartTrailing;

    plslinfo->cpLimToContinue = plslinfo->cpLimToStay = cp;
    plslinfo->endr = endr;
    *maxDepth = 1;

    return None;

out_of_memory:
    lserr = OutOfMemory;

error:
    free(text);

    if (loline)
    {
        free(loline->run_data);
        free(loline->glyph_advance);
        free(loline->glyph_offset);
        free(loline->glyph_map);
        free(loline->cluster_map);
        free(loline);
    }

    *pploline = NULL;

    return lserr;
}

enum LsErr WINAPI LoDisposeLine(struct LoLine* ploline, BOOL finalizing)
{
    free(ploline->run_data);
    free(ploline->text);
    free(ploline->glyph_advance);
    free(ploline->cluster_map);
    free(ploline->glyph_map);
    free(ploline->glyph_offset);
    free(ploline);

    return None;
}

enum LsErr WINAPI LoDisplayLine(struct LoLine* ploline, struct LSPOINT* pt, UINT displayMode, struct LSRECT* clipRect)
{
    struct RunData *run_data;
    enum LsErr err = None;
    INT idx = 0, i;

    pt->x += ploline->x_offset;
    ploline->pt = *pt;

    for (i = 0; i < ploline->num_runs && err >= 0; i++)
    {
        run_data = ploline->run_data + i;

        if (run_data->type != TextType)
        {
            pt->x += run_data->width;
            continue;
        }

        /* If we have a populated glyph_map, we will preference DrawGlyphs over DrawTextRun */
        if (ploline->glyph_map && ploline->glyph_map[idx])
            err = ploline->ploc->contextInfo.pfnDrawGlyphs(ploline, run_data->run, &ploline->text[idx],
                    &ploline->cluster_map[idx], NULL, run_data->length, &ploline->glyph_map[idx], &ploline->glyph_advance[idx],
                    NULL, &ploline->glyph_offset[idx], NULL, NULL, run_data->length, lstflowDefault, displayMode, pt, NULL, 0, clipRect);
        else
            err = ploline->ploc->contextInfo.pfnDrawTextRun(ploline, run_data->run, pt, &ploline->text[idx],
                    &ploline->glyph_advance[idx], run_data->length, lstflowDefault, 1, pt, NULL, run_data->width, clipRect);

        idx += run_data->length;
        pt->x += run_data->width;
    }

    return err;
}

enum LsErr LoEnumLine(struct LoLine* loline, BOOL reverseOder, BOOL fGeometryneeded, struct LSPOINT *pt)
{
    struct RunData *run_data;
    enum LsErr lserr = None;
    BOOL glyph_run;
    INT i, idx;

    idx = 0;
    for (i = 0; i < loline->num_runs; i++)
    {
        run_data = loline->run_data + i;
        glyph_run = loline->glyph_map && loline->glyph_map[idx];

        if (run_data->type == TextType)
        {
            if ((lserr = loline->ploc->contextInfo.pfnEnumText(loline, run_data->run, run_data->start_cp, run_data->length, loline->text + idx,
                    run_data->length, lstflowDefault, FALSE, FALSE, pt, NULL /* pheights is not used */, run_data->width,
                    glyph_run, loline->glyph_advance + idx, loline->cluster_map + idx, NULL, /* characterProperties is not used */
                    loline->glyph_map + idx, loline->glyph_advance + idx, loline->glyph_offset + idx, NULL, /*pGlyphProperties is not used */
                    run_data->length)) < 0)
                break;
        }

        idx += run_data->length;
        pt->x += run_data->width;
    }

    return lserr;
}

enum LsErr WINAPI LoQueryLinePointPcp(struct LoLine* ploline, struct LSPOINT* ptQuery, INT depthQueryMax,
        struct LsQSubInfo* pSubLineInfo, INT* actualDepthQuery, struct LsTextCell* lsTextCell)
{
    struct RunData *run_data = NULL;
    enum LsErr err = None;
    int i, x, cp, idx;

    lsTextCell->pointUvStartCell = ploline->pt;

    if (!ploline->num_runs)
        return None;

    x = ploline->pt.x;
    idx = 0;

    /* find the correct run */
    for (i = 0; i < ploline->num_runs; i++)
    {
        run_data = ploline->run_data + i;
        cp = run_data->start_cp;
        /* we'll break if we've found the right run or this is the last run */
        if (ptQuery->x < x + run_data->width || i + 1 == ploline->num_runs)
            break;

        x += run_data->width;
        idx += run_data->length;
    }

    /* if text, we now need to find the correct character */
    if (run_data->type == TextType)
    {
        for (i = 0; i < run_data->length; i++)
        {
            /* we'll break if we've found the right character or this is the last character */
            if (ptQuery->x < x + ploline->glyph_advance[idx] || i + 1 == run_data->length)
                break;

            x += ploline->glyph_advance[idx++];
            cp++;
        }

        lsTextCell->dupCell = ploline->glyph_advance[idx];
    }
    else
    {
        lsTextCell->dupCell = run_data->width;
    }

    lsTextCell->pointUvStartCell.x = x;
    lsTextCell->lscpStartCell = lsTextCell->lscpEndCell = cp;
    lsTextCell->cCharsInCell = lsTextCell->cGlyphsInCell = (run_data->type == TextType);
    lsTextCell->pointUvStartCell.x = x;

    pSubLineInfo[0].lscpFirstSubLine = 1;
    pSubLineInfo[0].plsrun = run_data->run;

    *actualDepthQuery = 1;

    return err;
}

enum LsErr WINAPI LoQueryLineCpPpoint(struct LoLine* ploline, INT lscpQuery, INT depthQueryMax,
        struct LsQSubInfo* pSubLineInfo, INT* actualDepthQuery, struct LsTextCell* lsTextCell)
{
    struct RunData *run_data = ploline->run_data;
    int i, x, cp, idx;

    lsTextCell->pointUvStartCell = ploline->pt;

    if (!ploline->num_runs)
        return None;

    x = ploline->pt.x;
    idx = 0;

    /* find the correct run */
    for (i = 0; i < ploline->num_runs; i++)
    {
        run_data = ploline->run_data + i;
        cp = run_data->start_cp;
        /* we'll break if we've found the right run or this is the last run */
        if (i+1 == ploline->num_runs || lscpQuery < run_data[1].start_cp)
            break;

        x += run_data->width;
        idx += run_data->length;
    }

    /* if text, we now need to find the correct character */
    if (run_data->type == TextType)
    {
        /* advance x and idx until we get to the right character */
        for (; cp < lscpQuery; cp++)
            x += ploline->glyph_advance[idx++];

        lsTextCell->dupCell = ploline->glyph_advance[idx];
    }
    else
    {
        lsTextCell->dupCell = run_data->width;
    }

    lsTextCell->pointUvStartCell.x = x;
    lsTextCell->lscpStartCell = lscpQuery;
    lsTextCell->lscpEndCell = lscpQuery;
    lsTextCell->cCharsInCell = lsTextCell->cGlyphsInCell = (run_data->type == TextType);

    pSubLineInfo[0].lscpFirstSubLine = 1;
    pSubLineInfo[0].plsrun = run_data->run;

    *actualDepthQuery = 1;

    return None;
}
