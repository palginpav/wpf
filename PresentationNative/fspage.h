// Wrappers calling SetLastError before a winapi call.

#include "loservice.h"

enum FSKCLEAR
{
    fskclearNone,
    fskclearLeft,
    fskclearRight,
    fskclearBoth,
};

enum FSKSUPPRESSHARDBREAKBEFOREFIRSTPARA
{
    fsksuppresshardbreakbeforefirstparaNone,
    fsksuppresshardbreakbeforefirstparaColumn,
    fsksuppresshardbreakbeforefirstparaPageAndColumn,
};

enum FSKUPDATE
{
    fskupdInherited,
    fskupdNoChange,
    fskupdNew,
    fskupdChangeInside,
    fskupdShifted,
};

enum FSKALIGNPAGE
{
    fskalpgTop,
    fskalpgCenter,
    fskalpgBottom,
};

enum FSFMTRKSTOP
{
    fmtrGoalReached,
    fmtrBrokenOutOfSpace,
    fmtrBrokenPageBreak,
    fmtrBrokenColumnBreak,
    fmtrBrokenPageBreakBeforePara,
    fmtrBrokenColumnBreakBeforePara,
    fmtrBrokenPageBreakBeforeSection,
    fmtrBrokenDelayable,
    fmtrNoProgressOutOfSpace,
    fmtrNoProgressPageBreak,
    fmtrNoProgressPageBreakBeforePara,
    fmtrNoProgressColumnBreakBeforePara,
    fmtrNoProgressPageBreakBeforeSection,
    fmtrNoProgressPageSkipped,
    fmtrNoProgressDelayable,
    fmtrCollision,
};

enum FSFMTRBL
{
    fmtrblGoalReached,
    fmtrblCollision,
    fmtrblInterrupted,
};

enum FSCOMPRESULT
{
    fscmprNoChange,
    fscmprChangeInside,
    fscmprShifted,
};

enum FSKCHANGE
{
    fskchNone,
    fskchNew,
    fskchInside,
};

enum FSFLRES           // line formatting result
{
    fsflrOutOfSpace,
    fsflrOutOfSpaceHyphenated,
    fsflrEndOfParagraph,
    fsflrEndOfParagraphClearLeft,
    fsflrEndOfParagraphClearRight,
    fsflrEndOfParagraphClearBoth,
    fsflrPageBreak,
    fsflrColumnBreak,
    fsflrSoftBreak,
    fsflrSoftBreakClearLeft,
    fsflrSoftBreakClearRight,
    fsflrSoftBreakClearBoth,
    fsflrNoProgressClear,
};

enum FSKALIGNFIG       // kind of alignment
{
    fskalfMin,
    fskalfCenter,
    fskalfMax,
};

enum FSKREF            // point of reference
{
    fskrefPage,
    fskrefMargin,
    fskrefParagraph,
    fskrefChar,
    fskrefOutOfMinMargin,
    fskrefOutOfMaxMargin
};

enum FSKWRAP           // kind of wrapping around obstacle
{
    fskwrNone,
    fskwrLeft,
    fskwrRight,
    fskwrBoth,
    fskwrLargest,
};

enum FSKTEXTDETAILS
{
    fsktdCached,
    fsktdFull,
};

enum FSKTEXTLINES
{
    fsklinesNormal = 0,                     // use normal FormatLine callback
    fsklinesOptimal = 1,                    // use reconstruction ReconstructLineVariant callback
    fsklinesForced = 2,                     // use special forced formatting FormatLineForced callback
                                            // note that forced paragraph can have only 1 line
    fsklinesWord = 3,
};

struct FSUPDATEINFO
{
    enum FSKUPDATE fskupd;
    INT dvrShifted;
};

struct FSRECT
{
    INT u;
    INT v;
    INT du;
    INT dv;
};

struct FSBBOX
{
    INT fDefined;
    struct FSRECT fsrc;
};

struct FSFMTR
{
    enum FSFMTRKSTOP kstop;
    INT fContainsItemThatStoppedBeforeFootnote;
    INT fForcedProgress;
};

struct FSTRACKDESCRIPTION
{
    struct FSUPDATEINFO fsupdinf;
    void* nms;
    struct FSRECT fsrc;
    struct FSBBOX fsbbox;
    INT fTrackRelativeToRect;
    void* pfstrack;
};

struct FSFTNINFO
{
    void* nmftn;  // name of footnote
    INT vrAccept;
    INT vrReject; // must be equal to vrAccept if fsffiWordFlowTextFinite is not set
};

struct FSCOLUMNINFO
{
    INT durBefore;                 // space before column
    INT durWidth;                  // width of the column
};

struct FSPAP
{
    INT idobj;
    INT fKeepWithNext;
    INT fBreakPageBefore;
    INT fBreakColumnBefore;
};

struct FSTXTPROPS
{
    UINT fswdir;
    INT dcpStartContent;
    INT fKeepTogether;
    INT fDropCap;
    INT cMinLinesAfterBreak;
    INT cMinLinesBeforeBreak;
    INT fVerticalGrid;
    INT fOptimizeParagraph;
    INT fAvoidHyphenationAtTrackBottom;
    INT fAvoidHyphenationOnLastChainElement;
    INT cMaxConsecutiveHyphens;
};

struct FSPOINT
{
    INT u;
    INT v;
};

struct FSLINEVARIANT
{
    void* pfsbreakreclineclient;
    void* pfslineclient;
    INT dcpLine;
    INT fForceBroken;
    enum FSFLRES fslres;
    INT dvrAscent;
    INT dvrDescent;
    INT fReformatNeighborsAsLastLine;
    void* ptsLinePenaltyInfo;
};

struct FSFIGUREPROPS
{
    enum FSKREF fskrefU;
    enum FSKREF fskrefV;
    enum FSKALIGNFIG fskalfU;
    enum FSKALIGNFIG fskalfV;
    enum FSKWRAP fskwrap;
    INT fNonTextPlane;
    INT fAllowOverlap;
    INT fDelayable;
};

struct EscStringInfo
{
    short*   szParaSeparator;
    short*   szLineSeparator;
    short*   szHidden;
    short*   szNbsp;
    short*   szObjectTerminator;
    short*   szObjectReplacement;
};

struct FSDROPCAPDETAILS
{
    struct FSRECT fsrcDropCap;            // position of drop cap rectangle
    int fSuppressDropCapTopSpacing;// was space at the top of the page suppressed?
    /* IntPtr */ void * pdcclient;              // ptr to drop cap created by client
};

struct FSTEXTDETAILSFULL
{
    UINT fswdir;                   // writing direction in text paragraph
    enum FSKTEXTLINES fsklines;         // kind of text lines: Word, Optimal, Normal
    INT fLinesComposite;           // if lines are composite
    INT cLines;                    // number of lines
    INT cAttachedObjects;          // number of floaters
    INT dcpFirst;                  // dcp of the first line, only if  cLines > 0
    INT dcpLim;                    // dcpLim of the last line, only if cLines > 0
    INT fDropCapPresent;           // is drop cap present?
    struct FSUPDATEINFO fsupdinfDropCap;  // update info for dropcap
    struct FSDROPCAPDETAILS dcdetails;    // drop cap details, iff drop cap present
    INT fSuppressTopLineSpacing;   // was top spacing of first line suppressed?
    INT fUpdateInfoForLinesPresent;// is following line update info meaningful? FALSE after initial formatting or clear update
    INT cLinesBeforeChange;
    INT dvrShiftBeforeChange;
    INT cLinesChanged;             // number of formatted lines in new line list
    INT dcLinesChanged;            // cLinesChanged minus number of old lines on their place
    INT dvrShiftAfterChange;
    INT ddcpAfterChange;
};

struct FSTEXTDETAILSCACHED
{
    UINT fswdir;                   // writing direction in text paragraph
    enum FSKTEXTLINES fsklines;         // kind of text lines: Word, Optimal, Normal
    struct FSRECT fsrcPara;               // paragraph's rectangle
    INT fSuppressTopLineSpacing;   // was top spacing of first line suppressed?
    INT dcpFirst;                  // dcp of the first line
    INT dcpLim;                    // dcpLim of the last line
    INT cLines;                    // number of simple lines
    INT fClearOnLeft;              // clear on left side?
    INT fClearOnRight;             // clear on right side?
    INT fOptimalLineDcpsCached;    // dcp's for optimal lines are available
};

struct FSTEXTDETAILS
{
    enum FSKTEXTDETAILS fsktd;
    union
    {
        struct FSTEXTDETAILSFULL full;
        struct FSTEXTDETAILSCACHED cached;
    } nested_u;
};

struct FSLINEDESCRIPTIONSINGLE
{
    void* pfslineclient;
    void* pfsbreakreclineclient;  // break record needed to format this line
    INT dcpFirst;              // dcpFirst of line
    INT dcpLim;                // dcpLim of line
    INT urStart;               // starting ur of the line
    INT dur;                   // width used during formatting
    INT fAllowHyphenation;     // was hyphenation allowed when formatting?
    INT urBBox;
    INT durBBox;
    INT vrStart;               // vr at the top of line
    INT dvrAscent;             // aggregated ascent (excluding suppressed space)
    INT dvrDescent;            // aggregated descent (including space after)
    INT fClearOnLeft;          // this line is clear on left side
    INT fClearOnRight;         // this line is clear on right side
    INT fTreatedAsFirst;       // was line treated as first during formatting?
    INT fForceBroken;          // value of fForcedBroken returned by FormatLine
};

struct FSPARADESCRIPTION
{
    struct FSUPDATEINFO fsupdinf;
    void* pfspara;
    void* pfsparaclient;
    void* nmp;
    INT idobj;
    INT dvrUsed;                   // input to paragraph formatting method
    struct FSBBOX fsbbox;
    INT dvrTopSpace;               // returned by paragraph formatting method
};

struct FSSUBTRACKDETAILS
{
    struct FSUPDATEINFO fsupdinf;
    void* nms;
    struct FSRECT fsrc;
    INT cParas;
};

struct FSTRACKDETAILS
{
    INT cParas;
};

typedef INT WINAPI (*ObjCreateContext)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsc,                        // IN:  FS context
            void* pfscbkobj,                   // IN:  callbacks (FSCBKOBJ)
            UINT ffi,                           // IN:  formatting flags
            INT idobj,                          // IN:  id of the object
            void** pfssobjc);               // OUT: object context

typedef INT WINAPI (*ObjDestroyContext)(
            void* pfssobjc);                   // IN:  object context

typedef INT WINAPI (*ObjFormatParaFinite)(
            void* pfssobjc,                    // IN:  object context
            void* pfsparaclient,               // IN:
            void* pfsobjbrk,                   // IN:  break record---use if !NULL
            INT fBreakRecordFromPreviousPage,   // IN:  break record was created on previous page
            void* nmp,                         // IN:  name of paragraph---use if break record is NULL
            INT iArea,                          // IN:  column-span area index
            void* pftnrej,                     // IN:
            void* pfsgeom,                     // IN:  pointer to geometry
            INT fEmptyOk,                       // IN:  is it OK not to add anything?
            INT fSuppressTopSpace,              // IN:  suppress empty space at the top of the page
            UINT fswdir,                        // IN:  current direction
            struct FSRECT* fsrcToFill,              // IN:  rectangle to fill
            void* pmcsclientIn,                // IN:  input margin collapsing state
            enum FSKCLEAR fskclearIn,                // IN:  clear property that must be satisfied
            enum FSKSUPPRESSHARDBREAKBEFOREFIRSTPARA fsksuppresshardbreakbeforefirstparaIn,
                                                // IN:  flags to suppress breaks
            INT fBreakInside,                   // IN:  produce vertical break inside para; needed for recursive KWN logic;
                                                //      can be set to true if during previous formatting fBreakInsidePossible output was returned as TRUE
            struct FSFMTR* fsfmtr,                  // OUT: result of formatting the paragraph
            void** pfspara,                 // OUT: pointer to the para data
            void** pbrkrecpara,             // OUT: pointer to the para break record
            INT* dvrUsed,                    // OUT: vertical space used by the para
            struct FSBBOX* fsbbox,                  // OUT: para BBox
            void** pmcsclientOut,           // OUT: margin collapsing state at the bottom
            enum FSKCLEAR* fskclearOut,           // OUT: ClearIn for the next paragraph
            INT* dvrTopSpace,                // OUT: top space due to collapsed margin
            INT* fBreakInsidePossible);      // OUT: internal vertical break possible, needed for recursive KWN logic

typedef INT WINAPI (*ObjFormatParaBottomless)(
            void* pfssobjc,                    // IN:  object context
            void* pfsparaclient,               // IN:
            void* nmp,                         // IN:  name of paragraph
            INT iArea,                          // IN:  column-span area index
            void* pfsgeom,                     // IN:  pointer to geometry
            INT fSuppressTopSpace,              // IN:  suppress empty space at the top of the page
            UINT fswdir,                        // IN:  current direction
            INT urTrack,                        // IN:  ur of bottomless rectangle to fill
            INT durTrack,                       // IN:  dur of bottomless rectangle to fill
            INT vrTrack,                        // IN:  vr of bottomless rectangle to fill
            void* pmcsclientIn,                // IN:  input margin collapsing state
            enum FSKCLEAR fskclearIn,                // IN:  clear property that must be satisfied
            INT fInterruptable,                 // IN:  formatting can be interrupted
            enum FSFMTRBL* fsfmtrbl,              // OUT: result of formatting the paragraph
            void** pfspara,                 // OUT: pointer to the para data
            INT* dvrUsed,                    // OUT: vertical space used by the para
            struct FSBBOX* fsbbox,                  // OUT: para BBox
            void** pmcsclientOut,           // OUT: margin collapsing state at the bottom
            enum FSKCLEAR* fskclearOut,           // OUT: ClearIn for the next paragraph
            INT* dvrTopSpace,                // OUT: top space due to collapsed margin
            INT* fPageBecomesUninterruptable);// OUT: interruption is prohibited from now on

typedef INT WINAPI (*ObjUpdateBottomlessPara)(
            void* pfspara,                     // IN:  pointer to the para data
            void* pfsparaclient,               // IN:
            void* nmp,                         // IN:  name of paragraph
            INT iArea,                          // IN:  column-span area index
            void* pfsgeom,                     // IN:  pointer to geometry
            INT fSuppressTopSpace,              // IN:  suppress empty space at the top of the page
            UINT fswdir,                        // IN:  current direction
            INT urTrack,                        // IN:  u of bootomless rectangle to fill
            INT durTrack,                       // IN:  du of bootomless rectangle to fill
            INT vrTrack,                        // IN:  v of bootomless rectangle to fill
            void* pmcsclientIn,                // IN:  input margin collapsing state
            enum FSKCLEAR fskclearIn,                // IN:  clear property that must be satisfied
            INT fInterruptable,                 // IN:  formatting can be interrupted
            enum FSFMTRBL* fsfmtrbl,              // OUT: result of formatting the paragraph
            INT* dvrUsed,                    // OUT: vertical space used by the para
            struct FSBBOX* fsbbox,                  // OUT: para BBox
            void** pmcsclientOut,           // OUT: margin collapsing state at the bottom
            enum FSKCLEAR* fskclearOut,           // OUT: ClearIn for the next paragraph
            INT* dvrTopSpace,                // OUT: top space due to collapsed margin
            INT* fPageBecomesUninterruptable);// OUT: interruption is prohibited from now on

typedef INT WINAPI (*ObjSynchronizeBottomlessPara)(
            void* pfspara,                     // IN:  pointer to the para data
            void* pfsparaclient,               // IN:
            void* pfsgeom,                     // IN: pointer to geometry
            UINT fswdir,                        // IN: direction
            INT dvrShift);                      // IN: shift by this value

typedef INT WINAPI (*ObjComparePara)(
            void* pfsparaclientOld,            // IN:
            void* pfsparaOld,                  // IN:  pointer to the old para data
            void* pfsparaclientNew,            // IN:
            void* pfsparaNew,                  // IN:  pointer to the new para data
            UINT fswdir,                        // IN:  track's direction
            enum FSCOMPRESULT* fscmpr,            // OUT: comparison result
            INT* dvrShifted);                // OUT: amount of shift if result is fscomprShifted

typedef INT WINAPI (*ObjClearUpdateInfoInPara)(
            void* pfspara);                    // IN:  pointer to the para data

typedef INT WINAPI (*ObjDestroyPara)(
            void* pfspara);                    // IN:  pointer to the para data

typedef INT WINAPI (*ObjDuplicateBreakRecord)(
            void* pfssobjc,                    // IN:  object context
            void* pfsbrkrecparaOrig,           // IN:  pointer to the para break record
            void** pfsbrkrecparaDup);       // OUT: pointer to the duplicate break record

typedef INT WINAPI (*ObjDestroyBreakRecord)(
            void* pfssobjc,                    // IN:  object context
            void* pfsobjbrk);                  // OUT: pointer to the para break record

typedef INT WINAPI (*ObjGetColumnBalancingInfo)(
            void* pfspara,                     // IN:  pointer to the para data
            UINT fswdir,                        // IN:  current direction
            INT* nlines,                     // OUT: number of text lines
            INT* dvrSumHeight,               // OUT: sum of all line heights
            INT* dvrMinHeight);              // OUT: minimum line height

typedef INT WINAPI (*ObjGetNumberFootnotes)(
            void* pfspara,                     // IN:  pointer to the para data
            INT* nftn);                      // OUT: number of footnotes

typedef INT WINAPI (*ObjGetFootnoteInfo)(
            void* pfspara,                     // IN:  pointer to the para data
            UINT fswdir,                        // IN:  current direction
            INT nftn,                           // IN:  size of FSFTNINFO array
            INT iftnFirst,                      // IN:  first index in FSFTNINFO array to be used by this para
            struct FSFTNINFO* pfsftninf,               // IN/OUT: array of footnote info
            INT* iftnLim);                   // OUT: lim index used by this paragraph

typedef INT WINAPI (*ObjShiftVertical)(
            void* pfspara,                     // IN:  pointer to the para data
            void* pfsparaclient,               // IN:
            void* pfsshift,                    // IN:  pointer to the shift data
            UINT fswdir,                        // IN:  wdir for bbox - the same as the one passed to formatting method
            struct FSBBOX* fsbbox);                 // OUT: output BBox

typedef INT WINAPI (*ObjTransferDisplayInfoPara)(
            void* pfsparaOld,                  // IN:  pointer to the old para data
            void* pfsparaNew);                 // IN:  pointer to the new para data

struct FSIMETHODS
{
    ObjCreateContext pfnCreateContext;
    ObjDestroyContext pfnDestroyContext;
    ObjFormatParaFinite pfnFormatParaFinite;
    ObjFormatParaBottomless pfnFormatParaBottomless;
    ObjUpdateBottomlessPara pfnUpdateBottomlessPara;
    ObjSynchronizeBottomlessPara pfnSynchronizeBottomlessPara;
    ObjComparePara pfnComparePara;
    ObjClearUpdateInfoInPara pfnClearUpdateInfoInPara;
    ObjDestroyPara pfnDestroyPara;
    ObjDuplicateBreakRecord pfnDuplicateBreakRecord;
    ObjDestroyBreakRecord pfnDestroyBreakRecord;
    ObjGetColumnBalancingInfo pfnGetColumnBalancingInfo;
    ObjGetNumberFootnotes pfnGetNumberFootnotes;
    ObjGetFootnoteInfo pfnGetFootnoteInfo;
    void* pfnGetFootnoteInfoWord;
    ObjShiftVertical pfnShiftVertical;
    ObjTransferDisplayInfoPara pfnTransferDisplayInfoPara;
};

typedef INT WINAPI (*FSkipPage)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of first section on the page
            INT* fSkip);                     // OUT: skip it due to odd/even page issue

typedef INT WINAPI (*GetPageDimensions)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of section on page
            UINT* fswdir,                    // OUT: direction of main text
            INT* fHeaderFooterAtTopBottom,   // OUT: header/footer position on the page
            INT* durPage,                    // OUT: page width
            INT* dvrPage,                    // OUT: page height
            struct FSRECT* fsrcMargin);             // OUT: rectangle within page margins

typedef INT WINAPI (*GetNextSection)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmsCur,                      // IN:  name of current section
            INT* fSuccess,                   // OUT: next section exists
            void** nmsNext);                // OUT: name of the next section

typedef INT WINAPI (*GetSectionProperties)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of section
            INT* fNewPage,                   // OUT: stop page before this section?
            UINT* fswdir,                    // OUT: direction of this section
            INT* fApplyColumnBalancing,      // OUT: apply column balancing to this section?
            INT* ccol,                       // OUT: number of columns in the main text segment
            INT* cSegmentDefinedColumnSpanAreas, // OUT:
            INT* cHeightDefinedColumnSpanAreas); // OUT:

typedef INT WINAPI (*GetJustificationProperties)(
            void* pfsclient,                   // IN:  client opaque data
            void** rgnms,                      // IN:  array of the section names on the page
            INT cnms,                           // IN:  number of sections on the page
            INT fLastSectionNotBroken,          // IN:  is last section on the page broken?
            INT* fJustify,                   // OUT: apply justification/alignment to the page?
            enum FSKALIGNPAGE* fskal,             // OUT: kind of vertical alignment for the page
            INT* fCancelAtLastColumn);       // OUT: cancel justification for the last column of the page?

typedef INT WINAPI (*GetMainTextSegment)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmsSection,                  // IN:  name of section
            void** nmSegment);              // OUT: name of the main text segment for this section

typedef INT WINAPI (*GetHeaderSegment)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of section
            void* pfsbrpagePrelim,             // IN:  ptr to page break record of main page
            UINT fswdir,                        // IN:  direction for dvrMaxHeight/dvrFromEdge
            INT* fHeaderPresent,             // OUT: is there header on this page?
            INT* fHardMargin,                // OUT: does margin increase with header?
            INT* dvrMaxHeight,               // OUT: maximum size of header
            INT* dvrFromEdge,                // OUT: distance from top edge of the paper
            UINT* fswdirHeader,              // OUT: direction for header
            void** nmsHeader);              // OUT: name of header segment

typedef INT WINAPI (*GetFooterSegment)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of section
            void* pfsbrpagePrelim,             // IN:  ptr to page break record of main page
            UINT fswdir,                        // IN:  direction for dvrMaxHeight/dvrFromEdge
            INT* fFooterPresent,             // OUT: is there footer on this page?
            INT* fHardMargin,                // OUT: does margin increase with footer?
            INT* dvrMaxHeight,               // OUT: maximum size of footer
            INT* dvrFromEdge,                // OUT: distance from bottom edge of the paper
            UINT* fswdirFooter,              // OUT: direction for footer
            void** nmsFooter);              // OUT: name of footer segment

typedef INT WINAPI (*UpdGetSegmentChange)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of the segment
            enum FSKCHANGE* fskch);               // OUT: kind of change

typedef INT WINAPI (*GetSectionColumnInfo)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of section
            UINT fswdir,                        // IN:  direction of section
            INT ncol,                           // IN:  size of the preallocated fscolinfo array
            struct FSCOLUMNINFO* fscolinfo,            // OUT: array of the colinfo structures
            INT* ccol);                      // OUT: actual number of the columns in the segment

typedef INT WINAPI (*GetSegmentDefinedColumnSpanAreaInfo)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of section
            INT cAreas,                         // IN:  number of areas - size of pre-allocated arrays
            void** rgnmSeg,                    // OUT: array of segment names for segment-defined areas
            INT* rgcColumns,                    // OUT: arrays of number of columns spanned
            INT* cAreasActual);              // OUT: actual number of segment-defined areas

typedef INT WINAPI (*GetHeightDefinedColumnSpanAreaInfo)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of section
            INT cAreas,                         // IN:  number of areas - size of pre-allocated arrays
            INT* rgdvrAreaHeight,               // OUT: array of segment names for height-defined areas
            INT* rgcColumns,                    // OUT: arrays of number of columns spanned
            INT* cAreasActual);              // OUT: actual number of height-defined areas

typedef INT WINAPI (*GetFirstPara)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of segment
            INT* fSuccessful,                // OUT: does segment contain any paragraph?
            void** nmp);                    // OUT: name of the first paragraph in segment

typedef INT WINAPI (*GetNextPara)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of segment
            void* nmpCur,                      // IN:  name of current para
            INT* fFound,                     // OUT: is there next paragraph?
            void** nmpNext);                // OUT: name of the next paragraph in section

typedef INT WINAPI (*UpdGetFirstChangeInSegment)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of segment
            INT* fFound,                     // OUT: anything changed?
            INT* fChangeFirst,               // OUT: first paragraph changed?
            void** nmpBeforeChange);        // OUT: name of paragraph before the change if !fChangeFirst

typedef INT WINAPI (*UpdGetParaChange)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmp,                         // IN:  name of the paragraph
            enum FSKCHANGE* fskch,                // OUT: kind of change
            INT* fNoFurtherChanges);         // OUT: no changes after?

typedef INT WINAPI (*GetParaProperties)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmp,                         // IN:  name of paragraph
            struct FSPAP* fspap);                   // OUT: paragraph properties

typedef INT WINAPI (*CreateParaclient)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmp,                         // IN:  name of paragraph
            void** pfsparaclient);          // OUT: opaque to PTS paragraph client

typedef INT WINAPI (*TransferDisplayInfo)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclientOld,            // IN:  opaque to PTS old paragraph client
            void* pfsparaclientNew);           // IN:  opaque to PTS new paragraph client

typedef INT WINAPI (*DestroyParaclient)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclient);              // IN:  opaque to PTS paragraph client

typedef INT WINAPI (*FInterruptFormattingAfterPara)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclient,               // IN:  opaque to PTS paragraph client
            void* nmp,                         // IN:  name of paragraph
            INT vr,                             // IN:  current v position
            INT* fInterruptFormatting);      // OUT: is it time to stop formatting?

typedef INT WINAPI (*GetEndnoteSeparators)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmsSection,                  // IN:  name of section
            void** nmsEndnoteSeparator,     // OUT: name of the endnote separator segment
            void** nmEndnoteContSeparator,  // OUT: name of endnote cont separator segment
            void** nmsEndnoteContNotice);   // OUT: name of the endnote cont notice segment

typedef INT WINAPI (*GetEndnoteSegment)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmsSection,                  // IN:  name of section
            INT* fEndnotesPresent,           // OUT: are there endnotes for this segment?
            void** nmsEndnotes);            // OUT: name of endnote segment

typedef INT WINAPI (*GetNumberEndnoteColumns)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of section
            INT* ccolEndnote);               // OUT: number of columns in endnote area

typedef INT WINAPI (*GetEndnoteColumnInfo)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of section
            UINT fswdir,                        // IN:  direction of section
            INT ncolEndnote,                    // IN:  size of preallocated fscolinfo array
            struct FSCOLUMNINFO* fscolinfoEndnote,     // OUT: array of the colinfo structures
            INT* ccolEndnote);               // OUT: actual number of the columns in footnote area

typedef INT WINAPI (*GetFootnoteSeparators)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmsSection,                  // IN:  name of section
            void** nmsFtnSeparator,         // OUT: name of the footnote separator segment
            void** nmsFtnContSeparator,     // OUT: name of the ftn cont separator segment
            void** nmsFtnContNotice);       // OUT: name of the footnote cont notice segment

typedef INT WINAPI (*FFootnoteBeneathText)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of section
            INT* fFootnoteBeneathText);      // OUT: position footnote right after text?

typedef INT WINAPI (*GetNumberFootnoteColumns)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of section
            INT* ccolFootnote);              // OUT: number of columns in footnote area

typedef INT WINAPI (*GetFootnoteColumnInfo)(
            void* pfsclient,                   // IN:  client opaque data
            void* nms,                         // IN:  name of section
            UINT fswdir,                        // IN:  direction of main text
            INT ncolFootnote,                   // IN:  size of preallocated fscolinfo array
            struct FSCOLUMNINFO* fscolinfoFootnote,    // OUT: array of the colinfo structures
            INT* ccolFootnote);              // OUT: actual number of the columns in footnote area

typedef INT WINAPI (*GetFootnoteSegment)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmftn,                       // IN:  name of footnote
            void** nmsFootnote);            // OUT: name of footnote segment

typedef INT WINAPI (*GetFootnotePresentationAndRejectionOrder)(
            void* pfsclient,                           // IN:  client opaque data
            INT cFootnotes,                             // IN:  size of all arrays
            void** rgProposedPresentationOrder,        // IN:  footnotes in proposed pres order
            void** rgProposedRejectionOrder,           // IN:  footnotes in proposed reject order
            INT* fProposedPresentationOrderAccepted, // OUT: agree with proposed order?
            void** rgFinalPresentationOrder,           // OUT: footnotes in final pres order
            INT* fProposedRejectionOrderAccepted,    // OUT: agree with proposed order?
            void** rgFinalRejectionOrder);             // OUT: footnotes in final reject order

typedef INT WINAPI (*FAllowFootnoteSeparation)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmftn,                       // IN:  name of footnote
            INT* fAllow);                    // OUT: allow separating footnote from its reference


struct FSCBKGEN
{
     FSkipPage pfnFSkipPage;
     GetPageDimensions pfnGetPageDimensions;
     GetNextSection pfnGetNextSection;
     GetSectionProperties pfnGetSectionProperties;
     GetJustificationProperties pfnGetJustificationProperties;
     GetMainTextSegment pfnGetMainTextSegment;
     GetHeaderSegment pfnGetHeaderSegment;
     GetFooterSegment pfnGetFooterSegment;
     UpdGetSegmentChange pfnUpdGetSegmentChange;
     GetSectionColumnInfo pfnGetSectionColumnInfo;
     GetSegmentDefinedColumnSpanAreaInfo pfnGetSegmentDefinedColumnSpanAreaInfo;
     GetHeightDefinedColumnSpanAreaInfo pfnGetHeightDefinedColumnSpanAreaInfo;
     GetFirstPara pfnGetFirstPara;
     GetNextPara pfnGetNextPara;
     UpdGetFirstChangeInSegment pfnUpdGetFirstChangeInSegment;
     UpdGetParaChange pfnUpdGetParaChange;
     GetParaProperties pfnGetParaProperties;
     CreateParaclient pfnCreateParaclient;
     TransferDisplayInfo pfnTransferDisplayInfo;
     DestroyParaclient pfnDestroyParaclient;
     FInterruptFormattingAfterPara pfnFInterruptFormattingAfterPara;
     GetEndnoteSeparators pfnGetEndnoteSeparators;
     GetEndnoteSegment pfnGetEndnoteSegment;
     GetNumberEndnoteColumns pfnGetNumberEndnoteColumns;
     GetEndnoteColumnInfo pfnGetEndnoteColumnInfo;
     GetFootnoteSeparators pfnGetFootnoteSeparators;
     FFootnoteBeneathText pfnFFootnoteBeneathText;
     GetNumberFootnoteColumns pfnGetNumberFootnoteColumns;
     GetFootnoteColumnInfo pfnGetFootnoteColumnInfo;
     GetFootnoteSegment pfnGetFootnoteSegment;
     GetFootnotePresentationAndRejectionOrder pfnGetFootnotePresentationAndRejectionOrder;
     FAllowFootnoteSeparation pfnFAllowFootnoteSeparation;
};

typedef INT WINAPI (*CreateParaBreakingSession)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclient,               // IN:  opaque to PTS paragraph client
            void* nmp,                         // IN:  name of paragraph
            INT iArea,                          // IN:  column-span area index
            INT fsdcpStart,                     // IN:  dcp where formatting will start
            void* pfsbreakreclineclient,       // IN:  break record for the first line
            UINT fswdir,                        // IN:  current direction
            INT urStartTrack,                   // IN:  position at the beginning of the track
            INT durTrack,                       // IN:  width of track
            INT urPageLeftMargin,               // IN:  left margin of the page
            void*** ppfsparabreakingsession, // OUT: paragraph breaking session
            INT* fParagraphJustified);       // OUT: if paragraph is justified

typedef INT WINAPI (*DestroyParaBreakingSession)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparabreakingsession);     // IN:  session to destroy

typedef INT WINAPI (*GetTextProperties)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmp,                         // IN:  name of paragraph
            INT iArea,                          // IN:  column-span area index
            struct FSTXTPROPS* fstxtprops);         // OUT: text paragraph properties

typedef INT WINAPI (*GetNumberFootnotes)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmp,                         // IN:  name of paragraph
            INT fsdcpStart,                     // IN:  dcp at the beginning of the range
            INT fsdcpLim,                       // IN:  dcp at the end of the range
            INT* nFootnote);                 // OUT: number of footnote references in the range

typedef INT WINAPI (*GetFootnotes)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmp,                         // IN:  name of paragraph
            INT fsdcpStart,                     // IN:  dcp at the beginning of the range
            INT fsdcpLim,                       // IN:  dcp at the end of the range
            INT nFootnotes,                     // IN:  size of the output array
            void** rgnmftn,                    // OUT: array of footnote names in the range
            int* rgdcp,                         // OUT: array of footnote refs in the range
            INT* cFootnotes);                // OUT: actual number of footnotes

typedef INT WINAPI (*FormatDropCap)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclient,               // IN:
            void* nmp,                         // IN:  name of paragraph
            INT iArea,                          // IN:  column-span area index
            UINT fswdir,                        // IN:  current direction
            INT fSuppressTopSpace,              // IN:  suppress empty space at the top of page
            void*** pfsdropc,                // OUT: pointer to drop cap created by client
            INT* fInMargin,                  // OUT: should it be positioned in margin or in track?
            INT* dur,                        // OUT: width of drop cap
            INT* dvr,                        // OUT: height of drop cap
            INT* cPolygons,                  // OUT: number of polygons
            INT* cVertices,                  // OUT: number of vertices
            INT* durText);                   // OUT: distance from text

typedef INT WINAPI (*GetDropCapPolygons)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsdropc,                    // IN:  pointer to drop cap
            void* nmp,                         // IN:  para name
            UINT fswdir,                        // IN:  current direction
            INT ncVertices,                    // IN:  size of array of vertex counts (= number of polygons)
            INT nfspt,                         // IN:  size of the array of all vertices
            int* rgcVertices,                   // OUT: array of vertex counts (array containing number of vertices for each polygon)
            INT* ccVertices,                 // OUT: actual number of vertex counts
            struct FSPOINT* rgfspt,                    // OUT: array of all vertices
            INT* cfspt,                      // OUT: actual total number of vertices in all polygons
            INT* fWrapThrough);              // OUT: fill text in empty areas within obstacles?

typedef INT WINAPI (*DestroyDropCap)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsdropc);                   // IN:  pointer to drop cap created by client

typedef INT WINAPI (*FormatBottomText)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclient,               // IN:
            void* nmp,                         // IN:  name of paragraph
            INT iArea,                          // IN:  column-span area index
            UINT fswdir,                        // IN:  current direction
            void* pfslineLast,                 // IN:  last formatted line
            INT dvrLine,                        // IN:  height of last line
            void*** pmcsclientOut);          // OUT: margin collapsing state at bottom of text

typedef INT WINAPI (*FormatLine)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclient,               // IN:
            void* nmp,                         // IN:  name of paragraph
            INT iArea,                          // IN:  column-span area index
            INT dcp,                            // IN:  dcp at the beginning of the line
            void* pbrlineIn,                   // IN:  client's line break record
            UINT fswdir,                        // IN:  current direction
            INT urStartLine,                    // IN:  position at the beginning of the line
            INT durLine,                        // IN:  maximum width of line
            INT urStartTrack,                   // IN:  position at the beginning of the track
            INT durTrack,                       // IN:  width of track
            INT urPageLeftMargin,               // IN:  left margin of the page
            INT fAllowHyphenation,              // IN:  allow hyphenation of the line?
            INT fClearOnLeft,                   // IN:  is clear on left side
            INT fClearOnRight,                  // IN:  is clear on right side
            INT fTreatAsFirstInPara,            // IN:  treat line as first line in paragraph
            INT fTreatAsLastInPara,             // IN:  treat line as last line in paragraph
            INT fSuppressTopSpace,              // IN:  suppress empty space at the top of page
            void** pfsline,                 // OUT: pointer to line created by client
            INT* dcpLine,                    // OUT: dcp consumed by the line
            void** ppbrlineOut,             // OUT: client's line break record
            INT* fForcedBroken,              // OUT: was line force-broken?
            enum FSFLRES* fsflres,                // OUT: result of formatting
            INT* dvrAscent,                  // OUT: ascent of the line
            INT* dvrDescent,                 // OUT: descent of the line
            INT* urBBox,                     // OUT: ur of the line's ink
            INT* durBBox,                    // OUT: dur of of the line's ink
            INT* dcpDepend,                  // OUT: number of chars after line break that were considered
            INT* fReformatNeighborsAsLastLine); // OUT: should line segments be reformatted?

typedef INT WINAPI (*FormatLineForced)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclient,               // IN:
            void* nmp,                         // IN:  name of paragraph
            INT iArea,                          // IN:  column-span area index
            INT dcp,                            // IN:  dcp at the beginning of the line
            void* pbrlineIn,                   // IN:  client's line break record
            UINT fswdir,                        // IN:  current direction
            INT urStartLine,                    // IN:  position at the beginning of the line
            INT durLine,                        // IN:  maximum width of line
            INT urStartTrack,                   // IN:  position at the beginning of the track
            INT durTrack,                       // IN:  width of track
            INT urPageLeftMargin,               // IN:  left margin of the page
            INT fClearOnLeft,                   // IN:  is clear on left side
            INT fClearOnRight,                  // IN:  is clear on right side
            INT fTreatAsFirstInPara,            // IN:  treat line as first line in paragraph
            INT fTreatAsLastInPara,             // IN:  treat line as last line in paragraph
            INT fSuppressTopSpace,              // IN:  suppress empty space at the top of page
            INT dvrAvailable,                   // IN:  available vertical space
            void*** pfsline,                 // OUT: pointer to line created by client
            INT* dcpLine,                    // OUT: dcp consumed by the line
            void*** ppbrlineOut,             // OUT: client's line break record
            enum FSFLRES* fsflres,                // OUT: result of formatting
            INT* dvrAscent,                  // OUT: ascent of the line
            INT* dvrDescent,                 // OUT: descent of the line
            INT* urBBox,                     // OUT: ur of the line's ink
            INT* durBBox,                    // OUT: dur of of the line's ink
            INT* dcpDepend);                 // OUT: number of chars after line break that were considered

typedef INT WINAPI (*FormatLineVariants)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparabreakingsession,      // IN:  current session
            INT dcp,                            // IN:  dcp at the beginning line variants
            void* pbrlineIn,                   // IN:  client's line break record
            UINT fswdir,                        // IN:  current direction
            INT urStartLine,                    // IN:  position at the beginning of the line
            INT durLine,                        // IN:  maximum width of line
            INT fAllowHyphenation,              // IN:  allow hyphenation of the line?
            INT fClearOnLeft,                   // IN:  is clear on left side
            INT fClearOnRight,                  // IN:  is clear on right side
            INT fTreatAsFirstInPara,            // IN:  treat line as first line in paragraph
            INT fTreatAsLastInPara,             // IN:  treat line as last line in paragraph
            INT fSuppressTopSpace,              // IN:  suppress empty space at the top of page
            void* lineVariantRestriction,      // IN:  line variant restriction
            INT nLineVariantsAlloc,             // IN:  size of the pre-allocated variant array
            struct FSLINEVARIANT* rgfslinevariant,     // OUT: pre-allocatedarray for line variants
            INT* nLineVariantsActual,        // OUT: actual number of variants
            INT* iLineVariantBest);          // OUT: best line variant index

typedef INT WINAPI (*ReconstructLineVariant)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclient,               // IN:
            void* nmp,                         // IN:  name of paragraph
            INT iArea,                          // IN:  column-span area index
            INT dcpStart,                       // IN:  dcp at the beginning of the line
            void* pbrlineIn,                   // IN:  client's line break record to start formatting
            INT dcpLine,                        // IN:  dcp this line should end with
            UINT fswdir,                        // IN:  current direction
            INT urStartLine,                    // IN:  position at the beginning of the line
            INT durLine,                        // IN:  maximum width of line
            INT urStartTrack,                   // IN:  position at the beginning of the track
            INT durTrack,                       // IN:  width of track
            INT urPageLeftMargin,               // IN:  left margin of the page
            INT fAllowHyphenation,              // IN:  allow hyphenation of the line?
            INT fClearOnLeft,                   // IN:  is clear on left side
            INT fClearOnRight,                  // IN:  is clear on right side
            INT fTreatAsFirstInPara,            // IN:  treat line as first line in paragraph
            INT fTreatAsLastInPara,             // IN:  treat line as last line in paragraph
            INT fSuppressTopSpace,              // IN:  suppress empty space at the top of page
            void*** pfsline,                 // OUT: pointer to line created by client
            void*** ppbrlineOut,             // OUT: client's line break record
            INT* fForcedBroken,              // OUT: was line force-broken?
            enum FSFLRES* fsflres,                // OUT: result of formatting
            INT* dvrAscent,                  // OUT: ascent of the line
            INT* dvrDescent,                 // OUT: descent of the line
            INT* urBBox,                     // OUT: ur of the line's ink
            INT* durBBox,                    // OUT: dur of of the line's ink
            INT* dcpDepend,                  // OUT: number of chars after line break that were considered
            INT* fReformatNeighborsAsLastLine);  // OUT: should line segments be reformatted?

typedef INT WINAPI (*DestroyLine)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsline);                    // IN:  pointer to line created by client

typedef INT WINAPI (*DuplicateLineBreakRecord)(
            void* pfsclient,                   // IN:  client opaque data
            void* pbrlineIn,                   // IN:  client's forced break record to duplicate
            void*** pbrlineDup);             // OUT: duplicated client's forced break record

typedef INT WINAPI (*DestroyLineBreakRecord)(
            void* pfsclient,                   // IN:  client opaque data
            void* pbrlineIn);                  // IN:  client's forced break record to duplicate

typedef INT WINAPI (*SnapGridVertical)(
            void* pfsclient,                   // IN:  client opaque data
            UINT fswdir,                        // IN:  current direction
            INT vrMargin,                       // IN:  top margin
            INT vrCurrent,                      // IN:  current vertical position
            INT* vrNew);                     // OUT: snapped vertical position

typedef INT WINAPI (*GetDvrSuppressibleBottomSpace)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclient,               // IN:
            void* pfsline,                     // IN:  pointer to line created by client
            UINT fswdir,                        // IN:  current direction
            INT* dvrSuppressible);           // OUT: empty space suppressible at the bottom

typedef INT WINAPI (*GetDvrAdvance)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclient,               // IN:
            void* nmp,                         // IN:  name of paragraph
            INT dcp,                            // IN:  dcp at the beginning of the line
            UINT fswdir,                        // IN:  current direction
            INT* dvr);                       // OUT: advance amount in tight wrap

typedef INT WINAPI (*UpdGetChangeInText)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmp,                         // IN:  name of paragraph
            INT* dcpStart,                   // OUT: start of change
            INT* ddcpOld,                    // OUT: number of chars in old range
            INT* ddcpNew);                   // OUT: number of chars in new range

typedef INT WINAPI (*UpdGetDropCapChange)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmp,                         // IN:  name of paragraph
            INT* fChanged);                  // OUT: dropcap changed?

typedef INT WINAPI (*FInterruptFormattingText)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclient,               // IN:
            void* nmp,                         // IN:  name of paragraph
            INT dcp,                            // IN:  current dcp
            INT vr,                             // IN:  current v position
            INT* fInterruptFormatting);      // OUT: is it time to stop formatting?

typedef INT WINAPI (*GetTextParaCache)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclient,               // IN:
            void* nmp,                         // IN:  name of paragraph
            INT iArea,                          // IN:  column-span area index
            UINT fswdir,                        // IN:  current direction
            INT urStartLine,                    // IN:  position at the beginning of the line
            INT durLine,                        // IN:  maximum width of line
            INT urStartTrack,                   // IN:  position at the beginning of the track
            INT durTrack,                       // IN:  width of track
            INT urPageLeftMargin,               // IN:  left margin of the page
            INT fClearOnLeft,                   // IN:  is clear on left side
            INT fClearOnRight,                  // IN:  is clear on right side
            INT fSuppressTopSpace,              // IN:  suppress empty space at the top of page
            INT* fFound,                     // OUT: is there cache for this paragrpaph?
            INT* dcpPara,                    // OUT: dcp consumed by the paragraph
            INT* urBBox,                     // OUT: ur of the para ink
            INT* durBBox,                    // OUT: dur of of the para ink
            INT* dvrPara,                    // OUT: height of the para
            enum FSKCLEAR* fskclear,              // OUT: kclear after paragraph
            void*** pmcsclientAfterPara,     // OUT: margin collapsing state after parag.
            INT* cLines,                     // OUT: number of lines in the paragraph
            INT* fOptimalLines,              // OUT: para had its lines optimized
            INT* fOptimalLineDcpsCached,     // OUT: cached dcp's for lines available
            INT* dvrMinLineHeight);          // OUT: minimal line height

typedef INT WINAPI (*SetTextParaCache)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclient,               // IN:
            void* nmp,                         // IN:  name of paragraph
            INT iArea,                          // IN:  column-span area index
            UINT fswdir,                        // IN:  current direction
            INT urStartLine,                    // IN:  position at the beginning of the line
            INT durLine,                        // IN:  maximum width of line
            INT urStartTrack,                   // IN:  position at the beginning of the track
            INT durTrack,                       // IN:  width of track
            INT urPageLeftMargin,               // IN:  left margin of the page
            INT fClearOnLeft,                   // IN:  is clear on left side
            INT fClearOnRight,                  // IN:  is clear on right side
            INT fSuppressTopSpace,              // IN:  suppress empty space at the top of page
            INT dcpPara,                        // IN:  dcp consumed by the paragraph
            INT urBBox,                         // IN:  ur of the para ink
            INT durBBox,                        // IN:  dur of of the para ink
            INT dvrPara,                        // IN:  height of the para
            enum FSKCLEAR fskclear,                  // IN:  kclear after paragraph
            void* pmcsclientAfterPara,         // IN:  margin collapsing state after paragraph
            INT cLines,                         // IN:  number of lines in the paragraph
            INT fOptimalLines,                  // IN:  paragraph has its lines optinmized
            int* rgdcpOptimalLines,             // IN:  array of dcp's of optimal lines
            INT dvrMinLineHeight);              // IN:  minimal line height

typedef INT WINAPI (*GetOptimalLineDcpCache)(
            void* pfsclient,                   // IN:  client opaque data
            INT cLines,                         // IN:  number of lines - size of pre-allocated array
            int* rgdcp);                        // OUT: array of dcp's to fill

typedef INT WINAPI (*GetNumberAttachedObjectsBeforeTextLine)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmp,                         // IN:  name of paragraph
            INT dcpFirst,                       // IN:  dcp at the beginning of the range
            INT* cAttachedObjects);          // OUT: number of attached objects

typedef INT WINAPI (*GetAttachedObjectsBeforeTextLine)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmp,                         // IN:  name of paragraph
            INT dcpFirst,                       // IN:  dcp at the beginning of the range
            INT nAttachedObjects,               // IN:  size of the object arrays
            void** rgnmpObjects,               // OUT: array of object names
            int* rgidobj,                       // OUT: array of idobj's of corresponding objects
            int* rgdcpAnchor,                   // OUT: array of dcp of the objects anchors
            INT* cObjects,                   // OUT: actual number of objects
            INT* fEndOfParagraph);           // OUT: paragraph ended after last object

typedef INT WINAPI (*GetNumberAttachedObjectsInTextLine)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsline,                     // IN:  pointer to line created by client
            void* nmp,                         // IN:  name of paragraph
            INT dcpFirst,                       // IN:  dcp at the beginning of the range
            INT dcpLim,                         // IN:  dcp at the end of the range
            INT fFoundAttachedObjectsBeforeLine,// IN:  Attached objects before line found
            INT dcpMaxAnchorAttachedObjectBeforeLine, // IN: Max dcp of anchor in objects before line
            INT* cAttachedObjects);          // OUT: number of attached objects

typedef INT WINAPI (*GetAttachedObjectsInTextLine)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsline,                     // IN:  pointer to line created by client
            void* nmp,                         // IN:  name of paragraph
            INT dcpFirst,                       // IN:  dcp at the beginning of the range
            INT dcpLim,                         // IN:  dcp at the end of the range
            INT fFoundAttachedObjectsBeforeLine,// IN:  Attached objects before line found
            INT dcpMaxAnchorAttachedObjectBeforeLine, // IN: Max dcp of anchor in objects before line
            INT nAttachedObjects,               // IN:  size of the floater arrays
            void** rgnmpObjects,               // OUT: array of floater names
            int* rgidobj,                       // OUT: array of idobj's of corresponding objects
            int* rgdcpAnchor,                   // OUT: array of dcp of the objects anchors
            INT* cObjects);                  // OUT: actual number of objects

typedef INT WINAPI (*UpdGetAttachedObjectChange)(
            void* pfsclient,                   // IN:  client opaque data
            void* nmp,                         // IN:  name of text paragraph
            void* nmpAttachedObject,           // IN:  name of object
            enum FSKCHANGE* fskchObject);         // OUT: kind of change for object

typedef INT WINAPI (*GetDurFigureAnchor)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclient,               // IN:
            void* pfsparaclientFigure,         // IN:
            void* pfsline,                     // IN:  pointer to line created by client
            void* nmpFigure,                   // IN:  figure's name
            UINT fswdir,                        // IN:  current direction
            void* pfsFmtLineIn,                // IN:  data needed to reformat the line
            INT* dur);                       // OUT: distance from the beginning of the line to the anchor

struct FSCBKTXT
{
     CreateParaBreakingSession pfnCreateParaBreakingSession;
     DestroyParaBreakingSession pfnDestroyParaBreakingSession;
     GetTextProperties pfnGetTextProperties;
     GetNumberFootnotes pfnGetNumberFootnotes;
     GetFootnotes pfnGetFootnotes;
     FormatDropCap pfnFormatDropCap;
     GetDropCapPolygons pfnGetDropCapPolygons;
     DestroyDropCap pfnDestroyDropCap;
     FormatBottomText pfnFormatBottomText;
     FormatLine pfnFormatLine;
     FormatLineForced pfnFormatLineForced;
     FormatLineVariants pfnFormatLineVariants;
     ReconstructLineVariant pfnReconstructLineVariant;
     DestroyLine pfnDestroyLine;
     DuplicateLineBreakRecord pfnDuplicateLineBreakRecord;
     DestroyLineBreakRecord pfnDestroyLineBreakRecord;
     SnapGridVertical pfnSnapGridVertical;
     GetDvrSuppressibleBottomSpace pfnGetDvrSuppressibleBottomSpace;
     GetDvrAdvance pfnGetDvrAdvance;
     UpdGetChangeInText pfnUpdGetChangeInText;
     UpdGetDropCapChange pfnUpdGetDropCapChange;
     FInterruptFormattingText pfnFInterruptFormattingText;
     GetTextParaCache pfnGetTextParaCache;
     SetTextParaCache pfnSetTextParaCache;
     GetOptimalLineDcpCache pfnGetOptimalLineDcpCache;
     GetNumberAttachedObjectsBeforeTextLine pfnGetNumberAttachedObjectsBeforeTextLine;
     GetAttachedObjectsBeforeTextLine pfnGetAttachedObjectsBeforeTextLine;
     GetNumberAttachedObjectsInTextLine pfnGetNumberAttachedObjectsInTextLine;
     GetAttachedObjectsInTextLine pfnGetAttachedObjectsInTextLine;
     UpdGetAttachedObjectChange pfnUpdGetAttachedObjectChange;
     GetDurFigureAnchor pfnGetDurFigureAnchor;
};

typedef INT WINAPI (*DuplicateMcsclient)(
            void* pfsclient,                   // IN:  client opaque data
            void* pmcsclientIn,                // IN:  margin collapsing state
            void** pmcsclientNew);          // OUT: duplicated margin collapsing state

typedef INT WINAPI (*DestroyMcsclient)(
            void* pfsclient,                   // IN:  client opaque data
            void* pmcsclient);                 // IN:  margin collapsing state to destroy

typedef INT WINAPI (*FEqualMcsclient)(
            void* pfsclient,                   // IN:  client opaque data
            void* pmcsclient1,                 // IN:  first margin collapsing state to compare
            void* pmcsclient2,                 // IN:  second margin collapsing state to compare
            INT* fEqual);                    // OUT: are MStructs equal?

typedef INT WINAPI (*ConvertMcsclient)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclient,               // IN:
            void* nmp,                         // IN:  name of paragraph
            UINT fswdir,                        // IN:  current direction
            void* pmcsclient,                  // IN:  pointer to the input margin collapsing state
            INT fSuppressTopSpace,              // IN:  suppress empty space at the top of page
            INT* dvr);                       // OUT: dvr, calculated based on margin collapsing state

struct FSCBKOBJ
{
     void* pfnNewPtr;
     void* pfnDisposePtr;
     void* pfnReallocPtr;
     DuplicateMcsclient pfnDuplicateMcsclient;
     DestroyMcsclient pfnDestroyMcsclient;
     FEqualMcsclient pfnFEqualMcsclient;
     ConvertMcsclient pfnConvertMcsclient;
     GetObjectHandlerInfo pfnGetObjectHandlerInfo;
};

typedef INT WINAPI (*GetFigureProperties)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclientFigure,         // IN:
            void* nmpFigure,                   // IN:  figure's name
            INT fInTextLine,                    // IN:  it is attached to text line
            UINT fswdir,                        // IN:  current direction
            INT fBottomUndefined,               // IN:  bottom of page is not defined
            INT* dur,                        // OUT: width of figure
            INT* dvr,                        // OUT: height of figure
            struct FSFIGUREPROPS* fsfigprops,       // OUT: figure attributes
            INT* cPolygons,                  // OUT: number of polygons
            INT* cVertices,                  // OUT: total number of vertices in all polygons
            INT* durDistTextLeft,            // OUT: distance to text from MinU side
            INT* durDistTextRight,           // OUT: distance to text from MaxU side
            INT* dvrDistTextTop,             // OUT: distance to text from MinV side
            INT* dvrDistTextBottom);         // OUT: distance to text from MaxV side

typedef INT WINAPI (*GetFigurePolygons)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclientFigure,         // IN:
            void* nmpFigure,                   // IN:  figure's name
            UINT fswdir,                        // IN:  current direction
            INT ncVertices,                     // IN:  size of array of vertex counts (= number of polygons)
            INT nfspt,                          // IN:  size of the array of all vertices
            INT* rgcVertices,                   // OUT: array of vertex counts (array containing number of vertices for each polygon)
            INT* ccVertices,                 // OUT: actual number of vertex counts
            struct FSPOINT* rgfspt,                    // OUT: array of all vertices
            INT* cfspt,                      // OUT: actual total number of vertices in all polygons
            INT* fWrapThrough);              // OUT: fill text in empty areas within obstacles?

typedef INT WINAPI (*CalcFigurePosition)(
            void* pfsclient,                   // IN:  client opaque data
            void* pfsparaclientFigure,         // IN:
            void* nmpFigure,                   // IN:  figure's name
            UINT fswdir,                        // IN:  current direction
            struct FSRECT* fsrcPage,                // IN:  page rectangle
            struct FSRECT* fsrcMargin,              // IN:  rectangle within page margins
            struct FSRECT* fsrcTrack,               // IN:  track rectangle
            struct FSRECT* fsrcFigurePreliminary,   // IN:  prelim figure rect calculated from figure props
            INT fMustPosition,                  // IN:  must find position in this track?
            INT fInTextLine,                    // IN:  it is attached to text line
            INT* fPushToNextTrack,           // OUT: push to next track?
            struct FSRECT* fsrcFlow,                // OUT: FlowAround rectangle
            struct FSRECT* fsrcOverlap,             // OUT: Overlap rectangle
            struct FSBBOX* fsbbox,                  // OUT: bbox
            struct FSRECT* fsrcSearch);             // OUT: search area for overlap

struct FSCBKFIG
{
     GetFigureProperties pfnGetFigureProperties;
     GetFigurePolygons pfnGetFigurePolygons;
     CalcFigurePosition pfnCalcFigurePosition;
};

struct FSCBKWRD
{
     void* pfnGetSectionHorizMargins;
     void* pfnFPerformColumnBalancing;
     void* pfnCalculateColumnBalancingApproximateHeight;
     void* pfnCalculateColumnBalancingStep;
     void* pfnGetColumnSectionBreak;
     void* pfnFSuppressKeepWithNextAtTopOfPage;
     void* pfnFSuppressKeepTogetherAtTopOfPage;
     void* pfnFAllowSpaceAfterOverhang;
     void* pfnFormatLineWord;
     void* pfnGetSuppressedTopSpace;
     void* pfnChangeSplatLineHeight;
     void* pfnGetDvrAdvanceWord;
     void* pfnGetMinDvrAdvance;
     void* pfnGetDurTooNarrowForFigure;
     void* pfnResolveOverlap;
     void* pfnGetOffsetForFlowAroundAndBBox;
     void* pfnGetClientGeometryHandle;
     void* pfnDuplicateClientGeometryHandle;
     void* pfnDestroyClientGeometryHandle;
     void* pfnObstacleAddNotification;
     void* pfnGetFigureObstaclesForRestart;
     void* pfnRepositionFigure;
     void* pfnFStopBeforeLr;
     void* pfnFStopBeforeLine;
     void* pfnFIgnoreCollision;
     void* pfnGetNumberOfLinesForColumnBalancing;
     void* pfnFCancelPageBreakBefore;
     void* pfnChangeVrTopLineForFigure;
     void* pfnFApplyWidowOrphanControlInFootnoteResolution;
};

struct FSCBK
{
    struct FSCBKGEN cbkgen;
    struct FSCBKTXT cbktxt;
    struct FSCBKOBJ cbkobj;
    struct FSCBKFIG cbkfig;
    struct FSCBKWRD cbkwrd;
};

struct FSCONTEXTINFO
{
     UINT version;                  // version number
     UINT fsffi;                    // compatibility flags
     INT drMinColumnBalancingStep;  // min step for col balancing algorithm
     INT cInstalledObjects;         // number of installed objects
     void* pInstalledObjects;      // array of installed objects
     void* pfsclient;              // client data for this context
     void* ptsPenaltyModule;       // Penalty module
     struct FSCBK fscbk;                   // FS client callbacks
     AssertFailed pfnAssertFailed;  // debugging callback
};

struct FSPAGEDETAILSSIMPLE
{
    struct FSTRACKDESCRIPTION trackdescr;
};

struct FSPAGEDETAILSCOMPLEX
{
    INT fTopBottomHeaderFooter;
    UINT fswdirHeader;
    struct FSTRACKDESCRIPTION trackdescrHeader;
    UINT fswdirFooter;
    struct FSTRACKDESCRIPTION trackdescrFooter;
    INT fJustified;
    enum FSKALIGNPAGE fskalpg;
    UINT fswdirPageProper;
    struct FSUPDATEINFO fsupdinfPageBody;
    struct FSRECT fsrcPageBody;
    struct FSRECT fsrcPageMarginActual;   // page margins can be recalculated because of big header/footer
    struct FSBBOX fsbboxPageBody;
    INT cSections;                 // 0 means that body is empty
    struct FSRECT fsrcFootnote;
    struct FSBBOX fsbboxFootnote;
    INT cFootnoteColumns;          // 0 means that there are no pagenotes
    struct FSTRACKDESCRIPTION trackdescrFootnoteSeparator;
};

struct FSPAGEDETAILS
{
    enum FSKUPDATE fskupd; // only fskupdNew/fskupdChangeInside/fskupdNoChange are possible
    INT fSimple;
    union
    {
        struct FSPAGEDETAILSSIMPLE pdsimple;
        struct FSPAGEDETAILSCOMPLEX pdcomplex;
    } u;
};
