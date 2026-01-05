#include "fspage.h"

struct InstalledObject
{
    struct FSIMETHODS subtrackparamethods;
    struct FSIMETHODS subpageparamethods;
};

struct DocContext
{
    struct FSCONTEXTINFO fscontextinfo;
};

enum LsErr WINAPI CreateInstalledObjectsInfo(struct FSIMETHODS* fssubtrackparamethods, struct FSIMETHODS* fssubpageparamethods,
        struct InstalledObject** pInstalledObjects, INT* cInstalledObjects)
{
    struct InstalledObject *installed_object;

    if (!(installed_object = malloc(sizeof(*installed_object))))
        return OutOfMemory;

    installed_object->subtrackparamethods = *fssubtrackparamethods;
    installed_object->subpageparamethods = *fssubpageparamethods;
    *cInstalledObjects = 1;
    *pInstalledObjects = installed_object;

    return None;
}

enum LsErr WINAPI LoAcquirePenaltyModule(struct LoContext* ploc, uintptr_t* penaltyModuleHandle)
{
    return None;
}

enum LsErr WINAPI LoGetPenaltyModuleInternalHandle(void* penaltyModuleHandle, uintptr_t* penaltyModuleInternalHandle)
{
    return None;
}

enum LsErr WINAPI CreateDocContext(struct FSCONTEXTINFO* fscontextinfo, struct DocContext** pfscontext)
{
    struct DocContext* doc_context;

    if (!(doc_context = malloc(sizeof(*doc_context))))
        return OutOfMemory;

    doc_context->fscontextinfo = *fscontextinfo;
    *pfscontext = doc_context;

    return None;
}

struct LineSummary
{
    INT len;
    INT dvr_ascent;
    INT dvr_descent;
    INT ur_bbox;
    INT dur_bbox;
};

struct Container
{
    INT num_sub_tracks;
    struct SubTrack **sub_track;
};

struct Text
{
    void *nmp;
    void *paraclient;
    INT num_lines;
    struct LineSummary *line;
};

struct SubTrack
{
    INT idobj;
    INT ur;
    INT vr;
    INT dur;
    INT dvr;
    INT suppress_top_space;
    void *nms;
    void *fsparaclient;
    union
    {
        struct Container *container;
        struct Text *text;
    } data;
};

struct Page
{
    INT ur;
    INT vr;
    INT dur;
    INT dvr;
    void *nms;
    struct Container *container;
};

enum LsErr WINAPI FsDestroySubtrack(struct DocContext* pfscontext, struct SubTrack* pfsSubtrack);

/* The container paragraph is used for building a hierarchy of UI elements. For example: a numbered list which
 * itself could contain another list
 */
static enum LsErr create_container(struct DocContext* pfscontext, void* segment, UINT dir, INT ur, INT dur,
        INT *dvrUsed, struct Container **pcontainer)
{
    struct InstalledObject *installed_object;
    INT i, success, max_sub_tracks = 4;
    INT fPageBecomesUninterruptable;
    struct Container *container;
    struct SubTrack *sub_track;
    enum FSKCLEAR fskclearOut;
    enum LsErr lserr = None;
    enum FSFMTRBL fsfmtrbl;
    struct FSBBOX fsbbox;
    void* pmcsclientOut;
    void *paraclient;
    INT dvrTopSpace;
    void *nmp;

    if (!(container = calloc(1, sizeof(*container))))
        return OutOfMemory;

    if (!(container->sub_track = calloc(max_sub_tracks, sizeof(*container->sub_track))))
        goto out_of_memory;

    installed_object = pfscontext->fscontextinfo.pInstalledObjects;

    if ((lserr = pfscontext->fscontextinfo.fscbk.cbkgen.pfnGetFirstPara(pfscontext, segment, &success, &nmp)) < 0)
        goto error;

    while (success)
    {
        if ((lserr = pfscontext->fscontextinfo.fscbk.cbkgen.pfnCreateParaclient(pfscontext, nmp, &paraclient)) < 0)
            goto error;

        /* this could result in recursion if the current container paragraph contains other container paragraphs */
        if ((lserr = installed_object->subtrackparamethods.pfnFormatParaBottomless(0, paraclient, nmp, 0, NULL, 1, dir, ur,
                        dur, *dvrUsed, 0, fskclearNone, 1, &fsfmtrbl, (void**)&sub_track, dvrUsed, &fsbbox, &pmcsclientOut,
                        &fskclearOut, &dvrTopSpace, &fPageBecomesUninterruptable)) < 0)
        {
            pfscontext->fscontextinfo.fscbk.cbkgen.pfnDestroyParaclient(pfscontext, paraclient);
            goto error;
        }

        if (sub_track)
        {
            sub_track->fsparaclient = paraclient;

            if (container->num_sub_tracks == max_sub_tracks)
            {
                struct SubTrack **sub_tracks;

                max_sub_tracks *= 2;
                if (!(sub_tracks = realloc(container->sub_track, max_sub_tracks * sizeof(*sub_tracks))))
                {
                    FsDestroySubtrack(pfscontext, sub_track);
                    goto out_of_memory;
                }

                container->sub_track = sub_tracks;
            }
            container->sub_track[container->num_sub_tracks++] = sub_track;
        }

        if ((lserr = pfscontext->fscontextinfo.fscbk.cbkgen.pfnGetNextPara(pfscontext, segment, nmp, &success, &nmp)) < 0)
            goto error;
    }

    *pcontainer = container;

    return lserr;

out_of_memory:
    lserr = OutOfMemory;

error:
    if (container->sub_track)
    {
        for (i = 0; i < container->num_sub_tracks; i++)
            FsDestroySubtrack(pfscontext, container->sub_track[i]);

        free(container->sub_track);
    }

    free(container);

    return lserr;
}

/* The text paragraph contains runs of text */
static enum LsErr create_text(struct DocContext* pfscontext, void *nmp, INT iArea, UINT dir, INT ur, INT dur,
        INT *dvrUsed, struct Text **ptext)
{
    INT dcp_line, forced_broken, dvr_ascent, dvr_descent, ur_bbox, dur_bbox, dcp_depend, reformat;
    INT max_num_lines = 4;
    enum FSFLRES flres;
    struct Text *text;
    enum LsErr lserr;
    void *paraclient;
    void *line_out;
    INT dcp = 0;



    if (!(text = calloc(1, sizeof(*text))))
        return OutOfMemory;

    if (!(text->line = calloc(max_num_lines, sizeof(*text->line))))
        goto out_of_memory;

    if ((lserr = pfscontext->fscontextinfo.fscbk.cbkgen.pfnCreateParaclient(pfscontext, nmp, &paraclient)) < 0)
        goto error;

    text->nmp = nmp;
    text->paraclient = paraclient;

    do
    {
        struct LineSummary *line_summary;
        void *line;

        /* this calls in to line services */
        if ((lserr = pfscontext->fscontextinfo.fscbk.cbktxt.pfnFormatLine(pfscontext, paraclient, nmp, iArea, dcp, NULL, dir,
                        ur, dur, ur, dur, ur, TRUE, TRUE, TRUE, text->num_lines == 0, FALSE, TRUE, &line, &dcp_line, &line_out,
                        &forced_broken, &flres, &dvr_ascent, &dvr_descent, &ur_bbox, &dur_bbox, &dcp_depend, &reformat)) < 0)
            goto error;

        pfscontext->fscontextinfo.fscbk.cbktxt.pfnDestroyLine(pfscontext, line);

        if (text->num_lines == max_num_lines)
        {
            struct LineSummary *lines;

            max_num_lines *= 2;
            if (!(lines = realloc(text->line, max_num_lines * sizeof(*lines))))
                goto out_of_memory;

            text->line = lines;
        }

        line_summary = &text->line[text->num_lines++];
        line_summary->len = dcp_line;
        line_summary->dvr_ascent = dvr_ascent;
        line_summary->dvr_descent = dvr_descent;
        line_summary->ur_bbox = ur_bbox;
        line_summary->dur_bbox = dur_bbox;

        dcp += dcp_line;
        *dvrUsed += dvr_ascent + dvr_descent;
    }
    while (flres != fsflrEndOfParagraph);

    *ptext = text;

    return lserr;

out_of_memory:
    lserr = OutOfMemory;

error:
    if (text->paraclient)
        pfscontext->fscontextinfo.fscbk.cbkgen.pfnDestroyParaclient(pfscontext, text->paraclient);

    free(text->line);
    free(text);

    return lserr;
}

static enum LsErr populate_sub_track(struct DocContext* pfscontext, void *fsnmSegment, INT fswdir, INT ur, INT dur,
        INT vr, INT fSuppressTopSpace, INT iArea, INT *pdvrUsed, struct SubTrack *sub_track)
{
    void *nmp, *paraclient = NULL;
    enum LsErr lserr = None;
    struct FSPAP props;
    INT success;

    if ((lserr = pfscontext->fscontextinfo.fscbk.cbkgen.pfnGetFirstPara(pfscontext, fsnmSegment, &success, &nmp)) < 0)
        goto error;

    if ((lserr = pfscontext->fscontextinfo.fscbk.cbkgen.pfnCreateParaclient(pfscontext, nmp, &paraclient)) < 0)
        goto error;

    if ((lserr = pfscontext->fscontextinfo.fscbk.cbkgen.pfnGetParaProperties(pfscontext, nmp, &props)) < 0)
        goto error;

    sub_track->ur = ur;
    sub_track->vr = vr;
    sub_track->dur = dur;
    sub_track->suppress_top_space = fSuppressTopSpace;
    sub_track->nms = fsnmSegment;
    sub_track->fsparaclient = paraclient;

    *pdvrUsed = vr;

    switch (sub_track->idobj = props.idobj)
    {
        /* Container Paragraph */
        case 0:
            if ((lserr = create_container(pfscontext, fsnmSegment, fswdir, ur, dur, pdvrUsed, &sub_track->data.container)) < 0)
                goto error;
            break;

        /* Text Paragraph */
        case -1:
            if ((lserr = create_text(pfscontext, nmp, iArea, fswdir, ur, dur, pdvrUsed, &sub_track->data.text)) < 0)
                goto error;
            break;
    }

    sub_track->dvr = *pdvrUsed - sub_track->vr;

    return lserr;

error:
    if (paraclient)
        pfscontext->fscontextinfo.fscbk.cbkgen.pfnDestroyParaclient(pfscontext, paraclient);

    sub_track->fsparaclient = NULL;

    return lserr;
}

static enum LsErr populate_page(struct DocContext* pfscontext, struct Page* page, void* fsnmsect, enum FSFMTRBL* pfsfmtrbl)
{
    INT header_footer_pos, dur_page, dvr_page, dvrUsed = 0;
    enum LsErr lserr = None;
    struct FSRECT margin;
    void *segment;
    UINT dir;

    if ((lserr = pfscontext->fscontextinfo.fscbk.cbkgen.pfnGetMainTextSegment(pfscontext, fsnmsect, &segment)) < 0)
        goto error;

    page->nms = segment;

    if ((lserr = pfscontext->fscontextinfo.fscbk.cbkgen.pfnGetPageDimensions(pfscontext, fsnmsect, &dir,
                    &header_footer_pos, &dur_page, &dvr_page, &margin)) < 0)
        goto error;

    page->ur = margin.u;
    page->dur = margin.du;
    page->vr = dvrUsed;

    if ((lserr = create_container(pfscontext, segment, dir, margin.u, margin.du, &dvrUsed, &page->container)) < 0)
        goto error;

    page->dvr = dvrUsed;

    *pfsfmtrbl = fmtrblGoalReached;

error:
    return lserr;
}

enum LsErr WINAPI FsCreatePageBottomless(struct DocContext* pfscontext, void* fsnmsect, enum FSFMTRBL *pfsfmtrbl, void **ppfspage)
{
    struct Page *page;
    enum LsErr lserr;

    if (!(page = calloc(1, sizeof(*page))))
        return OutOfMemory;

    *ppfspage = page;

    if ((lserr = populate_page(pfscontext, page, fsnmsect, pfsfmtrbl)) < 0)
    {
        free(page);
        *ppfspage = NULL;
    }

    return lserr;
}

enum LsErr WINAPI FsFormatSubtrackBottomless(struct DocContext* pfscontext, void* fsnmSegment, INT iArea, void* pfsGeom,
        INT fSuppressTopSpace, UINT fswdir, INT ur, INT dur, INT vr, void* pfsMcsClientIn, enum FSKCLEAR fsKClearIn,
        INT fCanBeInterruptedIn, enum FSFMTRBL* pfsfmtrbl, void** ppfsSubtrack, INT* pdvrUsed, struct FSBBOX* pfsBBox,
        void** ppfsMcsClientOut, enum FSKCLEAR* pfsKClearOut, INT* pTopSpace, INT* pfCanBeInterruptedOut)
{
    struct SubTrack *sub_track;
    enum LsErr lserr;

    *ppfsMcsClientOut = NULL;
    *pfsfmtrbl = fmtrblGoalReached;

    if (!(sub_track = calloc(1, sizeof(*sub_track))))
        return OutOfMemory;

    *ppfsSubtrack = sub_track;

    if ((lserr = populate_sub_track(pfscontext, fsnmSegment, fswdir, ur, dur, vr, fSuppressTopSpace, iArea, pdvrUsed, sub_track)) < 0)
    {
        *ppfsSubtrack = NULL;
        free(sub_track);
    }

    return lserr;
}

enum LsErr WINAPI FsQueryPageDetails(struct DocContext* pfscontext, struct Page* pPage, struct FSPAGEDETAILS* pPageDetails)
{
    struct FSTRACKDESCRIPTION *track_desc;
    enum LsErr ret = None;
    struct FSRECT *rect;

    pPageDetails->fskupd = fskupdNew;
    pPageDetails->fSimple = 1;
    track_desc = &pPageDetails->u.pdsimple.trackdescr;
    track_desc->nms = pPage->nms;
    rect = &track_desc->fsrc;
    rect->u = pPage->ur;
    rect->v = pPage->vr;
    rect->du = pPage->dur;
    track_desc->fsbbox.fDefined = 1;
    rect = &track_desc->fsbbox.fsrc;
    rect->u = pPage->ur;
    rect->dv = pPage->dvr;
    rect->du = pPage->dur;
    pPageDetails->u.pdsimple.trackdescr.pfstrack = pPage;

    return ret;
}

enum LsErr WINAPI FsQueryTrackDetails(struct DocContext* pfscontext, struct Page* page, struct FSTRACKDETAILS* pTrackDetails)
{
    pTrackDetails->cParas = page->container->num_sub_tracks;

    return None;
}

static void populate_para_desc(struct SubTrack* sub_track, struct FSPARADESCRIPTION* para_desc)
{
    struct FSRECT *box;

    para_desc->pfspara = sub_track;
    para_desc->pfsparaclient = sub_track->fsparaclient;
    para_desc->nmp = sub_track->nms;

    para_desc->dvrUsed = sub_track->dvr;
    para_desc->fsbbox.fDefined = 1;
    box = &para_desc->fsbbox.fsrc;
    box->u = sub_track->ur;
    box->v = sub_track->vr;
    box->du = sub_track->dur;
    box->dv = sub_track->dvr;
}

enum LsErr WINAPI FsQueryTrackParaList(struct DocContext* pfscontext, struct Page* page, INT cParas,
        struct FSPARADESCRIPTION* rgParaDesc, INT* cParaDesc)
{
    struct FSPARADESCRIPTION *para_desc;
    struct SubTrack *sub_track;
    INT i;

    *cParaDesc = page->container->num_sub_tracks;
    for (i = 0; i < *cParaDesc; i++)
    {
        para_desc = rgParaDesc + i;
        sub_track = page->container->sub_track[i];

        populate_para_desc(sub_track, para_desc);
    }

    return None;
}

enum LsErr WINAPI FsQuerySubtrackDetails(struct DocContext* pfscontext, struct SubTrack* sub_track,
        struct FSSUBTRACKDETAILS* pSubTrackDetails)
{
    struct FSRECT *rect;

    rect = &pSubTrackDetails->fsrc;
    rect->u = sub_track->ur;
    rect->v = sub_track->vr;
    rect->du = sub_track->dur;
    rect->dv = sub_track->dvr;
    pSubTrackDetails->nms = sub_track->nms;

    switch (sub_track->idobj)
    {
        case -1:
            pSubTrackDetails->cParas = 1;
            break;

        case 0:
            pSubTrackDetails->cParas = sub_track->data.container->num_sub_tracks;
            break;
    }

    return None;
}

enum LsErr WINAPI FsQuerySubtrackParaList(struct DocContext* pfscontext, struct SubTrack* sub_track, INT cParas,
        struct FSPARADESCRIPTION* rgParaDesc, INT* cParaDesc)
{
    struct FSRECT *box;
    INT i;

    switch (sub_track->idobj)
    {
        /* Text Paragraph */
        case -1:
            {
                struct Text *text = sub_track->data.text;

                *cParaDesc = 1;
                rgParaDesc[0].idobj = -1;
                rgParaDesc[0].pfspara = sub_track;
                rgParaDesc[0].fsbbox.fDefined = 1;
                rgParaDesc[0].nmp = text->nmp;
                rgParaDesc[0].pfsparaclient = text->paraclient;
                rgParaDesc[0].dvrUsed = sub_track->dvr;
                box = &rgParaDesc[0].fsbbox.fsrc;
                box->u = sub_track->ur;
                box->v = sub_track->vr;
                box->dv = sub_track->dvr;
                break;
            }

        /* Container Paragraph */
        case 0:
            {
                struct Container *container = sub_track->data.container;

                *cParaDesc = container->num_sub_tracks;
                for (i = 0; i < container->num_sub_tracks; i++)
                {
                    struct SubTrack *sub_track2 = container->sub_track[i];
                    rgParaDesc[i].idobj = 0;
                    rgParaDesc[i].pfspara = sub_track2;
                    rgParaDesc[i].nmp = sub_track2->nms;
                    rgParaDesc[i].pfsparaclient = sub_track2->fsparaclient;
                    rgParaDesc[i].fsbbox.fDefined = 1;
                    rgParaDesc[i].dvrUsed = sub_track2->dvr;
                    box = &rgParaDesc[i].fsbbox.fsrc;
                    box->u = sub_track2->ur;
                    box->v = sub_track2->vr;
                    box->dv = sub_track2->dvr;
                }
                break;
            }
    }

    return None;
}

enum LsErr WINAPI FsQueryTextDetails(struct DocContext* pfscontext, struct SubTrack* sub_track,
        struct FSTEXTDETAILS* pTextDetails)
{
    INT dcpLim = 0, i;
    struct Text *text;

    text = sub_track->data.text;

    pTextDetails->fsktd = fsktdFull;
    pTextDetails->nested_u.full.fSuppressTopLineSpacing = sub_track->suppress_top_space;
    pTextDetails->nested_u.full.cLines = text->num_lines;
    for (i = 0; i < text->num_lines; i++)
       dcpLim += text->line[i].len;
    pTextDetails->nested_u.full.dcpLim = dcpLim;

    return None;
}

enum LsErr WINAPI FsQueryLineListSingle(struct DocContext* pfscontext, struct SubTrack* sub_track,
        INT cLines, struct FSLINEDESCRIPTIONSINGLE* rgLineDesc, INT* cLineDesc)
{
    INT first = 0, vrStart;
    struct Text *text;
    INT i;

    text = sub_track->data.text;

    *cLineDesc = text->num_lines;
    vrStart = sub_track->vr;
    for (i = 0; i < *cLineDesc; i++)
    {
        struct LineSummary *line = &text->line[i];
        rgLineDesc[i].dcpFirst = first;
        rgLineDesc[i].dcpLim = first + line->len;
        rgLineDesc[i].urStart = line->ur_bbox;
        rgLineDesc[i].dur = sub_track->dur;
        rgLineDesc[i].fAllowHyphenation = 1;
        rgLineDesc[i].urBBox = line->ur_bbox;
        rgLineDesc[i].durBBox = line->dur_bbox;
        rgLineDesc[i].vrStart = vrStart;
        rgLineDesc[i].dvrAscent = line->dvr_ascent;
        rgLineDesc[i].dvrDescent = line->dvr_descent;
        rgLineDesc[i].fClearOnLeft = 1;
        rgLineDesc[i].fClearOnRight = 1;
        rgLineDesc[i].fTreatedAsFirst = i == 0;
        first += line->len;
        vrStart += line->dvr_ascent + line->dvr_descent;
    }

    return None;
}

enum LsErr WINAPI FsClearUpdateInfoInPage(struct DocContext* pfscontext, struct Page* pfspage)
{
    return None;
}

enum LsErr WINAPI DestroyDocContext(struct DocContext* pfscontext)
{
    free(pfscontext);

    return None;
}

enum LsErr WINAPI DestroyInstalledObjectsInfo(struct InstalledObject* pInstalledObjects)
{
    free(pInstalledObjects);

    return None;
}

enum LsErr WINAPI FsDestroySubtrack(struct DocContext* pfscontext, struct SubTrack* pfsSubtrack)
{
    switch (pfsSubtrack->idobj)
    {
        /* Text Paragraph */
        case -1:
            {
                struct Text *text;

                text = pfsSubtrack->data.text;
                pfscontext->fscontextinfo.fscbk.cbkgen.pfnDestroyParaclient(pfscontext, text->paraclient);
                free(text);
                break;
            }

        /* Container Paragraph */
        case 0:
            {
                struct Container *container;
                int i;

                container = pfsSubtrack->data.container;
                for (i = 0; i < container->num_sub_tracks; i++)
                    FsDestroySubtrack(pfscontext, container->sub_track[i]);
                free(container);
                break;
            }
    }

    pfscontext->fscontextinfo.fscbk.cbkgen.pfnDestroyParaclient(pfscontext, pfsSubtrack->fsparaclient);
    free(pfsSubtrack);

    return None;
}

enum LsErr WINAPI FsDestroyPage(struct DocContext* pfscontext, struct Page* pfspage)
{
    INT i;

    for (i = 0; i < pfspage->container->num_sub_tracks; i++)
        FsDestroySubtrack(pfscontext, pfspage->container->sub_track[i]);

    free(pfspage->container);
    free(pfspage);

    return None;
}

enum LsErr WINAPI FsUpdateBottomlessPage(struct DocContext* pfscontext, struct Page* pfspage, void* fsnmsect, enum FSFMTRBL* pfsfmtrbl)
{
    INT i;

    for (i = 0; i < pfspage->container->num_sub_tracks; i++)
        FsDestroySubtrack(pfscontext, pfspage->container->sub_track[i]);

    free(pfspage->container);

    return populate_page(pfscontext, pfspage, fsnmsect, pfsfmtrbl);
}

