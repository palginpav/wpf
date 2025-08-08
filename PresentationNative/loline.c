#include <stdio.h>

#include "loservice.h"

struct RunData
{
    INT start_cp;
    void *run;
    INT length;
    INT width;
};

struct LoLine
{
    struct LoContext *ploc;
    struct RunData *run_data;
    UINT num_runs;
    WCHAR *text;
    UINT num_chars;
    INT *glyph_advance;
};

enum LsErr WINAPI LoCreateLine(struct LoContext* ploc, INT cp, INT ccpLim, INT durColumn, UINT dwLineFlags,
        void* pInputBreakRec, struct LsLInfo* plslinfo, struct LoLine** pploline, INT* maxDepth, struct LsLineWidths* lineWidths)
{
    BOOL bufused, ishidden, eol = FALSE;
    struct RunData *run_data;
    INT total_width, chars;
    struct LoLine *loline;
    struct LsTxM metrics;
    WCHAR *text_pointer;
    struct LsChp lschp;
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

    do
    {
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

            if (loline->num_runs == max_runs)
            {
                max_runs *= 2;
                run_data = realloc(loline->run_data, sizeof(*loline->run_data) * max_runs);

                if (!run_data)
                    goto out_of_memory;

                loline->run_data = run_data;
            }

            run_data = loline->run_data + loline->num_runs;
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

    for (i = 0; i < loline->num_runs; i++)
    {
        run_data = loline->run_data + i;

        ploc->contextInfo.pfnGetRunTextMetrics(ploc, run_data->run, Presentation, lstflowDefault, &metrics);
        plslinfo->dvpAscent = max(plslinfo->dvpAscent, metrics.dvAscent);
        plslinfo->dvpDescent = max(plslinfo->dvpDescent, metrics.dvDescent);
        plslinfo->dvpMultiLineHeight = max(plslinfo->dvpMultiLineHeight, metrics.dvMultiLineHeight);

        ploc->contextInfo.pfnGetRunTextMetrics(loline, run_data->run, Reference, lstflowDefault, &metrics);
        plslinfo->dvrAscent = max(plslinfo->dvrAscent, metrics.dvAscent);
        plslinfo->dvrDescent = max(plslinfo->dvrDescent, metrics.dvDescent);
        plslinfo->dvrMultiLineHeight = max(plslinfo->dvrMultiLineHeight, metrics.dvMultiLineHeight);

        lineWidths->upLimLine += run_data->width;
    }

    plslinfo->cpLimToContinue = plslinfo->cpLimToStay = cp;
    plslinfo->endr = endrEndPara;
    *maxDepth = 1;

    return None;

out_of_memory:
    lserr = OutOfMemory;

error:
    if (text) free(text);

    if (loline)
    {
        if (loline->run_data) free(loline->run_data);
        if (loline->glyph_advance) free(loline->glyph_advance);
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
    free(ploline);

    return None;
}

enum LsErr WINAPI LoDisplayLine(struct LoLine* ploline, struct LSPOINT* pt, UINT displayMode, struct LSRECT* clipRect)
{
    struct RunData *run_data;
    enum LsErr err = None;
    INT idx = 0, i;

    for (i = 0; i < ploline->num_runs && err >= 0; i++)
    {
        run_data = ploline->run_data + i;
        err = ploline->ploc->contextInfo.pfnDrawTextRun(ploline, run_data->run, pt, &ploline->text[idx],
                &ploline->glyph_advance[idx], run_data->length, lstflowDefault, 1, pt, NULL, run_data->width, clipRect);

        idx += run_data->length;
        pt->x += run_data->width;
    }

    return err;
}

enum LsErr LoEnumLine(struct LoLine* ploline, BOOL reverseOder, BOOL fGeometryneeded, struct LSPOINT *pt)
{
    return None;
}

enum LsErr WINAPI LoQueryLinePointPcp(struct LoLine* ploline, struct LSPOINT* ptQuery, INT depthQueryMax,
        struct LsQSubInfo* pSubLineInfo, INT* actualDepthQuery, struct LsTextCell* lsTextCell)
{
    return None;
}

