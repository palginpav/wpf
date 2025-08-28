#include <windows.h>

enum LsErr
{
    None                                    = 0,
    InvalidParameter                        = -1,
    OutOfMemory                             = -2,
    NullOutputParameter                     = -3,
    InvalidContext                          = -4,
    InvalidLine                             = -5,
    InvalidDnode                            = -6,
    InvalidDeviceResolution                 = -7,
    InvalidRun                              = -8,
    MismatchLineContext                     = -9,
    ContextInUse                            = -10,
    DuplicateSpecialCharacter               = -11,
    InvalidAutonumRun                       = -12,
    FormattingFunctionDisabled              = -13,
    UnfinishedDnode                         = -14,
    InvalidDnodeType                        = -15,
    InvalidPenDnode                         = -16,
    InvalidNonPenDnode                      = -17,
    InvalidBaselinePenDnode                 = -18,
    InvalidFormatterResult                  = -19,
    InvalidObjectIdFetched                  = -20,
    InvalidDcpFetched                       = -21,
    InvalidCpContentFetched                 = -22,
    InvalidBookmarkType                     = -23,
    SetDocDisabled                          = -24,
    FiniFunctionDisabled                    = -25,
    CurrentDnodeIsNotTab                    = -26,
    PendingTabIsNotResolved                 = -27,
    WrongFiniFunction                       = -28,
    InvalidBreakingClass                    = -29,
    BreakingTableNotSet                     = -30,
    InvalidModWidthClass                    = -31,
    ModWidthPairsNotSet                     = -32,
    WrongTruncationPoint                    = -33,
    WrongBreak                              = -34,
    DupInvalid                              = -35,
    RubyInvalidVersion                      = -36,
    TatenakayokoInvalidVersion              = -37,
    WarichuInvalidVersion                   = -38,
    WarichuInvalidData                      = -39,
    CreateSublineDisabled                   = -40,
    CurrentSublineDoesNotExist              = -41,
    CpOutsideSubline                        = -42,
    HihInvalidVersion                       = -43,
    InsufficientQueryDepth                  = -44,
    InvalidBreakRecord                      = -45,
    InvalidPap                              = -46,
    ContradictoryQueryInput                 = -47,
    LineIsNotActive                         = -48,
    TooLongParagraph                        = -49,
    TooManyCharsToGlyph                     = -50,
    WrongHyphenationPosition                = -51,
    TooManyPriorities                       = -52,
    WrongGivenCp                            = -53,
    WrongCpFirstForGetBreaks                = -54,
    WrongJustTypeForGetBreaks               = -55,
    WrongJustTypeForCreateLineGivenCp       = -56,
    TooLongGlyphContext                     = -57,
    InvalidCharToGlyphMapping               = -58,
    InvalidMathUsage                        = -59,
    InconsistentChp                         = -60,
    StoppedInSubline                        = -61,
    PenPositionCouldNotBeUsed               = -62,
    DebugFlagsInShip                        = -63,
    InvalidOrderTabs                        = -64,
    OutputArrayTooSmall                     = -110,
    SystemRestrictionsExceeded              = -100,
    LsInternalError                         = -1000,
    NotImplemented                          = -10000,
    ClientAbort                             = -100000,
};

enum GRPFlags
{
    /*None = 0,*/

    //  Visi flags
    fFmiVisiCondHyphens             = 0x00000001,
    fFmiVisiParaMarks               = 0x00000002,
    fFmiVisiSpaces                  = 0x00000004,
    fFmiVisiTabs                    = 0x00000008,
    fFmiVisiSplats                  = 0x00000010,
    fFmiVisiBreaks                  = 0x00000020,

    //  Advanced typography
    fFmiApplyBreakingRules          = 0x00000040,
    fFmiApplyOpticalAlignment       = 0x00000080,
    fFmiPunctStartLine              = 0x00000100,
    fFmiHangingPunct                = 0x00000200,

    //  WYSIWYG flags
    fFmiPresSuppressWiggle          = 0x00000400,
    fFmiPresExactSync               = 0x00000800,

    //  AutoNumbering flags
    fFmiAnm                         = 0x00001000,

    //  Misc.
    fFmiAutoDecimalTab              = 0x00002000,
    fFmiUnderlineTrailSpacesRM      = 0x00004000,
    fFmiSpacesInfluenceHeight       = 0x00008000,
    fFmiIgnoreSplatBreak            = 0x00010000,
    fFmiLimSplat                    = 0x00020000,
    fFmiAllowSplatLine              = 0x00040000,
    fFmiForceBreakAsNext            = 0x00080000,
    fFmiAllowHyphenation            = 0x00100000,
    fFmiDrawInCharCodes             = 0x00200000,
    fFmiTreatHyphenAsRegular        = 0x00400000,
    fFmiWrapTrailingSpaces          = 0x00800000,
    fFmiWrapAllSpaces               = 0x01000000,

    //  Compatibility flags for bugs in older versions of Word
    fFmiFCheckTruncateBefore        = 0x02000000,
    fFmiForgetLastTabAlignment      = 0x10000000,
    fFmiIndentChangesHyphenZone     = 0x20000000,
    fFmiNoPunctAfterAutoNumber      = 0x40000000,
    fFmiResolveTabsAsWord97         = 0x80000000,
};

enum LsKAlign
{
    lskalLeft,
    lskalCentered,
    lskalRight,
};

enum Plsrun
{
    CloseAnchor             = 0,
    Reverse                 = 1,
    FakeLineBreak           = 2, // simulated line break for security mitigation
    FormatAnchor            = 3,
    Hidden                  = 4,
    Text                    = 5, // run with type starting at this point is stored
    InlineObject            = 6,
    LineBreak               = 7,
    ParaBreak               = 8,

    Undefined               = 0x80000000,   // Bit 31 cannot be set without causing an overflow exception in LineServices so we reserve it as a 'undefined' Plsrun value
    IsMarker                = 0x40000000,   // Bit indicates this run is part of marker symbol
    UseNewCharacterBuffer   = 0x20000000,   // Bit indicates this run uses dynamically allocated heap buffer for run characters
    IsSymbol                = 0x10000000,   // Bit indicates run uses a non-Unicode font; don't change this value without updating unmanaged GetBreakingClasses callback
    UnmaskAll               = 0x0FFFFFFF,   // Value to unmask all the masking bits
};

// Unmanaged enum counterpart is in LSLO.H.
// == Both sides must be binary compatible ==
enum LineFlags
{
    // line break flags
    /*None                = 0,*/
    BreakClassWide      = 0x00000001,
    BreakClassStrict    = 0x00000002,
    BreakAlways         = 0x00000004,
    MinMax              = 0x00000008,
    KeepState           = 0x00000010,
};

enum LsEndRes
{
    endrNormal,
    endrHyphenated,
    endrEndPara,
    endrAltPara,
    endrSoftCR,
    endrEndColumn,
    endrEndSection,
    endrEndPage,
    endrEndParaSection,
    endrStopped,
    endrBeforeFillLineObject,
    endrAfterFillLineObject,
    endrMathUserRequiredBreak
};

enum LsBreakJust
{
    lsbrjBreakJustify,          // regular US
    lsbrjBreakWithCompJustify,  // FE & Newspaper
    lsbrjBreakThenExpand,       // Arabic
    lsbrjBreakOptimal,          // Best fit/Optimal paragraph
    lsbrjBreakThenSqueeze       // WordPerfect
};

enum LsKJust
{
    lskjFullInterWord,
    lskjFullInterLetterAligned,
    lskjFullScaled,
    lskjFullGlyphs,
    lskjFullMixed,
    lskjSnapGrid
};

enum LsKEOP
{
    lskeopEndPara1,
    lskeopEndPara2,
    lskeopEndPara12,
    lskeopEndParaAlt
};

enum LsKTab
{
    lsktLeft,
    lsktCenter,
    lsktRight,
    lsktDecimal,
    lsktChar
};

enum LsTFlow
{
    lstflowDefault = 0,
    lstflowES      = 0,
    lstflowEN,
    lstflowSE,
    lstflowSW,
    lstflowWS,
    lstflowWN,
    lstflowNE,
    lstflowNW,
};

enum LsBrkCond
{
    Never,
    Can,
    Please,
    Must
};

enum LsDevice
{
    Presentation,
    Reference,
};

enum LsExpType
{
    /*None = 0,*/
    AddWhiteSpace,
    AddInkContinuous,
    AddInkDiscrete,
};

enum LsKysr
{
    /// <summary>
    /// Normal Hyphenation
    /// </summary>
    kysrNormal,

    /// <summary>
    /// Add letter before hyphen
    /// </summary>
    kysrAddBefore,

    /// <summary>
    /// Change letter before hyphen
    /// </summary>
    kysrChangeBefore,

    /// <summary>
    /// Delete letter before hyphen
    /// </summary>
    kysrDeleteBefore,

    /// <summary>
    /// Change letter after hyphen
    /// </summary>
    kysrChangeAfter,

    /// <summary>
    /// Delete letter before the hypen and change the preceding one
    /// </summary>
    kysrDelAndChange,

    /// <summary>
    /// Add letter before the hyphen and change letter after it
    /// </summary>
    kysrAddBeforeChangeAfter,
};

enum LsHyphenQuality
{
    lshqExcellent,
    lshqGood,
    lshqFair,
    lshqPoor,
    lshqBad
};

enum LsChpFlags
{
    /*None = 0,*/
    fApplyKern              = 0x0001,
    fModWidthOnRun          = 0x0002,
    fModWidthSpace          = 0x0004,
    fModWidthPairs          = 0x0008,
    fCompressOnRun          = 0x0010,
    fCompressSpace          = 0x0020,
    fCompressTable          = 0x0040,
    fExpandOnRun            = 0x0080,
    fExpandSpace            = 0x0100,
    fExpandTable            = 0x0200,
    fGlyphBased             = 0x0400,
    // 5 bits of padding to align to next word
    fInvisible              = 0x00010000,
    fUnderline              = 0x00020000,
    fStrike                 = 0x00040000,
    fShade                  = 0x00080000,
    fBorder                 = 0x00100000,
    fSymbol                 = 0x00200000,
    fHyphen                 = 0x00400000,   // Hyphenation opportunity (YSR info)
    fCheckForReplaceChar    = 0x00800000,   // Activate the replace char mechanizm for Yen
    // 8 bits of padding
};

struct LSPOINT
{
    INT x;
    INT y;
};

struct LSRECT
{
    INT left;
    INT top;
    INT right;
    INT bottom;
};

struct LSSIZE
{
    INT width;
    INT height;
};

struct LsChp
{
    USHORT       idObj;
    USHORT       dcpMaxContent;
    UINT         effectsFlags;
    enum LsChpFlags        flags;      // bitfields
    INT          dvpPos;
};

// TextMetrics (LSTXM)
// Returned to LS by GetRunTextMetrics
struct LsTxM
{
    INT      dvAscent;
    INT      dvDescent;
    INT      dvMultiLineHeight;
    INT      fMonospaced;         // BOOL
};

struct GlyphOffset
{
    INT du;
    INT dv;
};

// Heights (heights)
// Input parameter to DrawTextRun
struct LsHeights
{
    INT      dvAscent;
    INT      dvDescent;
    INT      dvMultiLineHeight;
};

struct LsQSubInfo
{
    enum LsTFlow     lstflowSubLine;
    INT         lscpFirstSubLine;
    INT         lsdcpSubLine;
    struct LSPOINT     pointUvStartSubLine;
    struct LsHeights   lsHeightsPresSubLine;
    INT         dupSubLine;

    UINT        idobj;
    void*      plsrun;
    INT         lscpFirstRun;
    INT         lsdcpRun;
    struct LSPOINT     pointUvStartRun;
    struct LsHeights   lsHeightsPresRun;
    INT         dupRun;
    INT         dvpPosRun;
    INT         dupBorderBefore;
    INT         dupBorderAfter;

    struct LSPOINT     pointUvStartObj;
    struct LsHeights   lsHeightsPresObj;
    INT         dupObj;
};

struct LsTextCell
{
    INT       lscpStartCell;
    INT       lscpEndCell;
    struct LSPOINT   pointUvStartCell;
    INT       dupCell;
    INT       cCharsInCell;
    INT       cGlyphsInCell;
    void*    plsCellDetails;  // client-defined structure
};

struct LsLineWidths
{
    INT      upStartMarker;      // column start to marker start
    INT      upLimMarker;        // column start to marker end
    INT      upStartMainText;    // column start to main text start
    INT      upStartTrailing;    // column start to trailing space start
    INT      upLimLine;          // column start to line end
    INT      upMinStartTrailing; // the smallest upStartTrailing possible
    INT      upMinLimLine;       // the smallest upLimLine possible
};

// Presentation/Rendering device resolutions (LSDEVRES)
struct LsDevRes
{
    UINT   dxpInch;
    UINT   dypInch;
    UINT   dxrInch;
    UINT   dyrInch;
};

// Presentation/Rendering device resolutions (LSDEVRES)
struct LsLInfo
{
    INT                  dvpAscent;
    INT                  dvrAscent;
    INT                  dvpDescent;
    INT                  dvrDescent;
    INT                  dvpMultiLineHeight;
    INT                  dvrMultiLineHeight;
    INT                  dvpAscentAutoNumber;
    INT                  dvrAscentAutoNumber;
    INT                  dvpDescentAutoNumber;
    INT                  dvrDescentAutoNumber;
    INT                  cpLimToContinue;
    INT                  cpLimToStay;
    INT                  dcpDepend;
    INT                  cpFirstVis;
    enum LsEndRes             endr;
    INT                  fAdvanced;
    INT                  vaAdvance;
    INT                  fFirstLineInPara;
    INT                  fTabInMarginExLine;
    INT                  fForcedBreak;
    UINT                 EffectsFlags;
};

struct LsBreakRecSubl
{
    INT      lscpFetch;
    INT      idobj;
    void*   pbreakrecobj;
};

struct LsBreaks
{
    INT              cBreaks;                // number of total breaks
    struct LsLInfo*         plslinfoArray;          // array of LSLINFO structs, each per each break
    void*          plinepenaltyArray;      // array of unsafe handle to TSLINEPENALITYINFO struct, each per each break
    void*          pplolineArray;          // array of unsafe handle to Loline struct, each per each break
};

struct LsTbd
{
    enum LsKTab       lskt;
    INT          ur;
    WCHAR         wchTabLeader;
    WCHAR         wchCharTab;
};

struct LsTabs
{
    INT       durIncrementalTab;
    INT       iTabUserDefMac;
    void*    plsTbd;
};

struct LsULInfo
{
    UINT     kulBase;
    INT      cNumberOfLines;
    INT      dvpUnderlineOriginOffset;
    INT      dvpFirstUnderlineOffset;
    INT      dvpFirstUnderlineSize;
    INT      dvpGapBetweenLines;
    INT      dvpSecondUnderlineSize;
};

struct LsStInfo
{
    UINT     kstBase;
    INT      cNumberOfLines;
    INT      dvpLowerStrikethroughOffset;
    INT      dvpLowerStrikethroughSize;
    INT      dvpUpperStrikethroughOffset;
    INT      dvpUpperStrikethroughSize;
};

struct LsHyph
{
    enum LsKysr               kysr;
    WCHAR                 wchYsr;
    WCHAR                 wchYsr2;
    enum LsHyphenQuality      lshq;
};

struct LsNeighborInfo
{
    UINT                         fNeighborIsPresent;
    UINT                         fNeighborIsText;
    enum Plsrun                       plsrun;
    WCHAR                         wch;
    UINT                         fGlyphBased;
    USHORT                       chprop;
    USHORT                       gindex;
    UINT                         gprop;
};

struct LsGlyphRunInfo
{
    enum Plsrun                       plsrun;
    WCHAR*                        pwch;           // array of character codes
    USHORT*                      rggmap;         // wchar->glyph mapping
    USHORT*                       rgchprop;       // array of char properties as returned by GetGlyphs
    INT                          cwch;           // number of characters
    INT                          duChangeRight;  // nominal-to-ideal changes on the right side of the run
    USHORT*                      rggindex;       // glyph indices
    UINT*                        rggprop;        // array of glyph properties as returned by GetGlyphs
    INT*                         rgduWidth;      // array of glyph widths as returned by GetGlypPositions
    struct GlyphOffset*                 rggoffset;      // array of glyph offset as returned by GetGlypPositions
    INT                          cgindex;        // number of glyphs
};

struct LsCharRunInfo
{
    enum Plsrun   plsrun;
    WCHAR*    pwch;                   // array of character codes
    INT*     rgduNominalWidth;       // array of nominal widths
    INT*     rgduChangeLeft;         // array of nominal-to-ideal changes on the left side
    INT*     rgduChangeRight;        // array of nominal-to-ideal changes on the right side
    INT      cwch;                   // number of characters
};

struct LsPap
{
    INT          cpFirst;            // First CP
    INT          cpFirstContent;     // First content CP
    enum GRPFlags        grpf;
    enum LsBreakJust  lsbrj;
    enum LsKJust      lskj;
    INT          fJustify;
    INT          durAutoDecimalTab;
    enum LsKEOP       lskeop;
    enum LsTFlow      lstflow;
};

struct LsLineProps
{
    enum LsKAlign     lskal;                  // Alignment type
    INT          durLeft;                // Left line boundary
    INT          durRightBreak;          // Right linebreak boundary
    INT          durRightJustify;        // Right justification boundary
    INT          fProhibitHyphenation;   // prohibit hyphenation on this line?
    INT          durHyphenationZone;     // hyphenation zone for non-optimal breaking
};

typedef enum LsErr WINAPI (*FetchPap)(
    void*                      pols,
    INT                         lscpFetch,
    struct LsPap*                   lspap
);

typedef enum LsErr WINAPI (*FetchLineProps)(
    void*                      pols,
    INT                         lscpFetch,
    INT                         firstLineInPara,
    struct LsLineProps*             lsLineProps
);

typedef enum LsErr WINAPI (*FetchRunRedefined)(
    void*                      pols,
    INT                         lscpFetch,
    INT                         fIsStyle,       // logically boolean (see above)
    void*                      pstyle,
    WCHAR*                       pwchTextBuffer,
    INT                         cchTextBuffer,
    INT*                     fIsBufferUsed,  // logically boolean (see above)
    WCHAR**                  pwchText,
    INT*                     cchText,
    INT*                     fIsHidden,      // logically boolean (see above)
    struct LsChp*                   lschp,
    void**                  lsplsrun
);

typedef enum LsErr WINAPI (*GetRunTextMetrics )(
    void*                      pols,
    void*                      plsrun,
    enum LsDevice                    lsDevice,
    enum LsTFlow                     lstFlow,
    struct LsTxM*                   lstTextMetrics
);

typedef enum LsErr WINAPI (*GetRunCharWidths)(
    void*                      pols,
    void*                      plsrun,
    enum LsDevice                    device,
    WCHAR                        *runText,
    INT                         cchRun,
    INT                         maxWidth,
    enum LsTFlow                     textFlow,
    INT                         *charWidths,
    INT*                     totalWidth,
    INT*                     cchProcessed
);

typedef enum LsErr WINAPI (*GetDurMaxExpandRagged)(
    void*                      pols,
    enum Plsrun                      plsrun,
    enum LsTFlow                     lstFlow,
    INT*                     maxExpandRagged
);

typedef enum LsErr WINAPI (*DrawTextRun)(
    void*                      pols,
    void*                      plsrun,
    struct LSPOINT*                ptText,
    WCHAR                        *runText,
    INT                         *charWidths,
    INT                         cchText,
    enum LsTFlow                     textFlow,
    UINT                        displayMode,
    struct LSPOINT*                ptRun,
    struct LsHeights*              lsHeights,
    INT                         dupRun,
    struct LSRECT*                 clipRect
);

typedef enum LsErr WINAPI (*FInterruptShaping)(
    void*                      pols,
    enum LsTFlow                     textFlow,
    enum Plsrun                      firstPlsrun,
    enum Plsrun                      secondPlsrun,
    INT*                     fIsInterruptOk      // logically boolean (see above)
);

typedef enum LsErr WINAPI (*GetRunUnderlineInfo)(
    void*                      pols,
    enum Plsrun                      plsrun,
    struct LsHeights*              lsHeights,
    enum LsTFlow                     textFlow,
    struct LsULInfo*               ulInfo
);

typedef enum LsErr WINAPI (*GetRunStrikethroughInfo)(
    void*                      pols,
    enum Plsrun                      plsrun,
    struct LsHeights*              lsHeights,
    enum LsTFlow                     textFlow,
    struct LsStInfo*               stInfo
);

typedef enum LsErr WINAPI (*Hyphenate)(
    void*                      pols,
    INT                         fLastHyphenationFound,  // logically boolean (see above)
    INT                         lscpLastHyphenation,
    struct LsHyph*                 lastHyphenation,
    INT                         lscpBeginWord,
    INT                         lscpExceed,
    INT*                     fHyphenFound,           // logically boolean (see above)
    INT*                     lscpHyphen,
    struct LsHyph*                 plsHyph
);

typedef enum LsErr WINAPI (*GetNextHyphenOpp)(
    void*                      pols,
    INT                         lscpStartSearch,
    INT                         lsdcpSearch,
    INT*                     fHyphenFound,           // logically boolean (see above)
    INT*                     lscpHyphen,
    struct LsHyph*                 lsHyph
);

typedef enum LsErr WINAPI (*GetPrevHyphenOpp)(
    void*                      pols,
    INT                         lscpStartSearch,
    INT                         lsdcpSearch,
    INT*                     fHyphenFound,           // logically boolean (see above)
    INT*                     lscpHyphen,
    struct LsHyph*                 lsHyph
);

typedef enum LsErr WINAPI (*GetAutoNumberInfo)(
    void*                      pols,
    enum LsKAlign*               alignment,
    struct LsChp*                  lschp,
    void**                 lsplsrun,
    WCHAR*                 addedChar,              // logically char (see above)
    struct LsChp*                  lschpAddedChar,
    void**                 lsplsrunAddedChar,
    INT*                     fWord95Model,           // logically boolean (see above)
    INT*                     offset,
    INT*                     width
);

typedef enum LsErr WINAPI (*DrawUnderline)(
    void*                      pols,
    enum Plsrun                      plsrun,
    UINT                        ulType,
    struct LSPOINT*                ptOrigin,
    INT                         ulLength,
    INT                         ulThickness,
    enum LsTFlow                     textFlow,
    UINT                        displayMode,
    struct LSRECT*                 clipRect
);

typedef enum LsErr WINAPI (*DrawStrikethrough)(
    void*                      pols,
    enum Plsrun                      plsrun,
    UINT                        stType,
    struct LSPOINT*                ptOrigin,
    INT                         stLength,
    INT                         stThickness,
    enum LsTFlow                     textFlow,
    UINT                        displayMode,
    struct LSRECT*                 clipRect
);

typedef enum LsErr WINAPI (*GetGlyphsRedefined)(
    void*                      pols,
    void**                     plsplsruns,
    INT*                        pcchPlsrun,
    INT                         plsrunCount,
    WCHAR*                       pwchText,
    INT                         cchText,
    enum LsTFlow                     textFlow,
    USHORT*                     puGlyphsBuffer,
    USHORT*                     piGlyphPropsBuffer,
    INT                         cgiGlyphBuffers,
    INT*                     fIsGlyphBuffersUsed,    // logically boolean (see above)
    USHORT*                     puClusterMap,
    USHORT*                     puCharProperties,
    INT*                        pfCanGlyphAlone,
    INT*                     glyphCount
);

typedef enum LsErr WINAPI (*GetGlyphPositions)(
    void*                      pols,
    void*                      *plsplsruns,
    INT                         *pcchPlsrun,
    INT                         plsrunCount,
    enum LsDevice                    device,
    WCHAR                        *pwchText,
    USHORT                      *puClusterMap,
    USHORT                      *puCharProperties,
    INT                         cchText,
    USHORT                      *puGlyphs,
    USHORT                      *piGlyphProperties,
    INT                         glyphCount,
    enum LsTFlow                     textFlow,
    INT                         *piGlyphAdvances,
    struct GlyphOffset                 *piiGlyphOffsets
);

typedef enum LsErr WINAPI (*DrawGlyphs)(
    void*                      pols,
    void*                      plsrun,
    WCHAR                        *pwchText,
    USHORT                      *puClusterMap,
    USHORT                      *puCharProperties,
    INT                         cchText,
    USHORT                      *puGlyphs,
    INT                         *piJustifiedGlyphAdvances,
    INT                         *puGlyphAdvances,
    struct GlyphOffset                 *piiGlyphOffsets,
    UINT                        *piGlyphProperties,
    enum LsExpType                   *plsExpType,
    INT                         glyphCount,
    enum LsTFlow                     textFlow,
    UINT                        displayMode,
    struct LSPOINT*                origin,
    struct LsHeights*              lsHeights,
    INT                         runWidth,
    struct LSRECT*                 clippingRect
);

typedef enum LsErr WINAPI (*EnumText)(
    void*                      pols,
    void*                      plsrun,
    INT                         cpFirst,
    INT                         dcp,
    WCHAR                        *pwchText,
    INT                         cchText,
    enum LsTFlow                     lstFlow,
    INT                         fReverseOrder,      // logically boolean (see above)
    INT                         fGeometryProvided,  // logically boolean (see above)
    struct LSPOINT*                pptStart,
    struct LsHeights*              pheights,
    INT                         dupRun,
    INT                         glyphBaseRun,
    INT                         *charWidths,
    USHORT                      *pClusterMap,
    USHORT                      *characterProperties,
    USHORT                      *puglyphs,
    INT                         *pGlyphAdvances,
    struct GlyphOffset                 *pGlyphOffsets,
    UINT                        *pGlyphProperties,
    INT                         glyphCount
);

typedef enum LsErr WINAPI (*EnumTab)(
    void*                      pols,
    enum Plsrun                      plsrun,
    INT                         cpFirst,
    WCHAR                        *pwchText,
    WCHAR                        tabLeader,
    enum LsTFlow                     lstFlow,
    INT                         fReverseOrder,      // logically boolean (see above)
    INT                         fGeometryProvided,  // logically boolean (see above)
    struct LSPOINT*                pptStart,
    struct LsHeights*              heights,
    INT                         dupRun
);

typedef enum LsErr WINAPI (*GetCharCompressionInfoFullMixed)(
    void*                      pols,
    enum LsDevice                    device,
    enum LsTFlow                     textFlow,
    struct LsCharRunInfo               *plscharrunInfo,
    struct LsNeighborInfo              *plsneighborInfoLeft,
    struct LsNeighborInfo              *plsneighborInfoRight,
    INT                         maxPriorityLevel,
    INT                         **pplscompressionLeft,
    INT                         **pplscompressionRight
);

typedef enum LsErr WINAPI (*GetCharExpansionInfoFullMixed)(
    void*                      pols,
    enum LsDevice                    device,
    enum LsTFlow                     textFlow,
    struct LsCharRunInfo               *plscharrunInfo,
    struct LsNeighborInfo              *plsneighborInfoLeft,
    struct LsNeighborInfo              *plsneighborInfoRight,
    INT                         maxPriorityLevel,
    INT                         **pplsexpansionLeft,
    INT                         **pplsexpansionRight
);

typedef enum LsErr WINAPI (*GetGlyphCompressionInfoFullMixed)(
    void*                      pols,
    enum LsDevice                    device,
    enum LsTFlow                     textFlow,
    struct LsGlyphRunInfo              *plsglyphrunInfo,
    struct LsNeighborInfo              *plsneighborInfoLeft,
    struct LsNeighborInfo              *plsneighborInfoRight,
    INT                         maxPriorityLevel,
    INT                         **pplscompressionLeft,
    INT                         **pplscompressionRight
);

typedef enum LsErr WINAPI (*GetGlyphExpansionInfoFullMixed)(
    void*                      pols,
    enum LsDevice                    device,
    enum LsTFlow                     textFlow,
    struct LsGlyphRunInfo              *plsglyphrunInfo,
    struct LsNeighborInfo              *plsneighborInfoLeft,
    struct LsNeighborInfo              *plsneighborInfoRight,
    INT                         maxPriorityLevel,
    INT                         **pplsexpansionLeft,
    INT                         **pplsexpansionRight,
    enum LsExpType                   *plsexptype,
    INT                         *pduMinInk
);

typedef INT WINAPI (*GetObjectHandlerInfo)(
            void* pfsclient,                   // IN:  client opaque data
            INT idobj,                          // IN:  id of the object handler
            void* pobjectinfo);                // OUT: initialization information for the specified object

typedef void (*AssertFailed)(const char* arg1, const char* arg2, INT arg3, UINT arg4);

struct LsContextInfo
{
    UINT                            version;            // version number
    INT                             cInstalledHandlers; // total installed object handlers
    void*                           plsimethods;        // array of installed objects LSIMETHODS

    // Text config (LSTXTCFG)
    INT                             cEstimatedCharsPerLine;
    // used for input array size for all four FullMixed justification callbacks (cPriorityLevelMax)
    INT                             cJustPriorityLim;

    WCHAR                           wchUndef;
    WCHAR                           wchNull;
    WCHAR                           wchSpace;
    WCHAR                           wchHyphen;
    WCHAR                           wchTab;
    WCHAR                           wchPosTab;
    WCHAR                           wchEndPara1;
    WCHAR                           wchEndPara2;
    WCHAR                           wchAltEndPara;
    WCHAR                           wchEndLineInPara;
    WCHAR                           wchColumnBreak;
    WCHAR                           wchSectionBreak;
    WCHAR                           wchPageBreak;
    WCHAR                           wchNonBreakSpace;
    WCHAR                           wchNonBreakHyphen;
    WCHAR                           wchNonReqHyphen;
    WCHAR                           wchEmDash;
    WCHAR                           wchEnDash;
    WCHAR                           wchEmSpace;
    WCHAR                           wchEnSpace;
    WCHAR                           wchNarrowSpace;
    WCHAR                           wchOptBreak;
    WCHAR                           wchNoBreak;
    WCHAR                           wchFESpace;
    WCHAR                           wchJoiner;
    WCHAR                           wchNonJoiner;
    WCHAR                           wchToReplace;
    WCHAR                           wchReplace;

    WCHAR                           wchVisiNull;
    WCHAR                           wchVisiAltEndPara;
    WCHAR                           wchVisiEndLineInPara;
    WCHAR                           wchVisiEndPara;
    WCHAR                           wchVisiSpace;
    WCHAR                           wchVisiNonBreakSpace;
    WCHAR                           wchVisiNonBreakHyphen;
    WCHAR                           wchVisiNonReqHyphen;
    WCHAR                           wchVisiTab;
    WCHAR                           wchVisiPosTab;
    WCHAR                           wchVisiEmSpace;
    WCHAR                           wchVisiEnSpace;
    WCHAR                           wchVisiNarrowSpace;
    WCHAR                           wchVisiOptBreak;
    WCHAR                           wchVisiNoBreak;
    WCHAR                           wchVisiFESpace;
    WCHAR                           wchEscAnmRun;
    WCHAR                           wchPad;


    //  More LSCONTEXTINFO
    void*                           pols;  // Opaque client object

    // Memory management on unmanaged heap
    void*                           pfnNewPtr;
    void*                           pfnDisposePtr;
    void*                           pfnReallocPtr;

    // General application callbacks
    void*                           pfnFetchRun;
    GetAutoNumberInfo               pfnGetAutoNumberInfo;
    void*                           pfnGetNumericSeparators;
    void*                           pfnCheckForDigit;
    FetchPap                        pfnFetchPap;
    FetchLineProps                  pfnFetchLineProps;
    void*                           pfnFetchTabs;
    void*                           pfnReleaseTabsBuffer;
    void*                           pfnGetBreakThroughTab;
    void*                           pfnGetPosTabProps;
    void*                           pfnFGetLastLineJustification;
    void*                           pfnCheckParaBoundaries;
    GetRunCharWidths                pfnGetRunCharWidths;
    void*                           pfnCheckRunKernability;
    void*                           pfnGetRunCharKerning;
    GetRunTextMetrics               pfnGetRunTextMetrics;
    GetRunUnderlineInfo             pfnGetRunUnderlineInfo;
    GetRunStrikethroughInfo         pfnGetRunStrikethroughInfo;
    void*                           pfnGetBorderInfo;
    void*                           pfnReleaseRun;
    void*                           pfnReleaseRunBuffer;
    Hyphenate                       pfnHyphenate;
    GetPrevHyphenOpp                pfnGetPrevHyphenOpp;
    GetNextHyphenOpp                pfnGetNextHyphenOpp;
    void*                           pfnGetHyphenInfo;
    DrawUnderline                   pfnDrawUnderline;
    DrawStrikethrough               pfnDrawStrikethrough;
    void*                           pfnDrawBorder;
    void*                           pfnFInterruptUnderline;
    void*                           pfnFInterruptShade;
    void*                           pfnFInterruptBorder;
    void*                           pfnShadeRectangle;
    DrawTextRun                     pfnDrawTextRun;
    void*                           pfnDrawSplatLine;
    FInterruptShaping               pfnFInterruptShaping;
    void*                           pfnGetGlyphs;
    GetGlyphPositions               pfnGetGlyphPositions;
    DrawGlyphs                      pfnDrawGlyphs;
    void*                           pfnReleaseGlyphBuffers;
    void*                           pfnGetGlyphExpansionInfo;
    void*                           pfnGetGlyphExpansionInkInfo;
    void*                           pfnGetGlyphRunInk;
    void*                           pfnGetEms;
    void*                           pfnPunctStartLine;
    void*                           pfnModWidthOnRun;
    void*                           pfnModWidthSpace;
    void*                           pfnCompOnRun;
    void*                           pfnCompWidthSpace;
    void*                           pfnExpOnRun;
    void*                           pfnExpWidthSpace;
    void*                           pfnGetModWidthClasses;
    void*                           pfnGetBreakingClasses;
    void*                           pfnFTruncateBefore;
    void*                           pfnCanBreakBeforeChar;
    void*                           pfnCanBreakAfterChar;
    void*                           pfnFHangingPunct;
    void*                           pfnGetSnapGrid;
    void*                           pfnDrawEffects;
    void*                           pfnFCancelHangingPunct;
    void*                           pfnModifyCompAtLastChar;
    GetDurMaxExpandRagged            pfnGetDurMaxExpandRagged;
    GetCharExpansionInfoFullMixed    pfnGetCharExpansionInfoFullMixed;
    GetGlyphExpansionInfoFullMixed   pfnGetGlyphExpansionInfoFullMixed;
    GetCharCompressionInfoFullMixed  pfnGetCharCompressionInfoFullMixed;
    GetGlyphCompressionInfoFullMixed pfnGetGlyphCompressionInfoFullMixed;
    void*                           pfnGetCharAlignmentStartLine;
    void*                           pfnGetCharAlignmentEndLine;
    void*                           pfnGetGlyphAlignmentStartLine;
    void*                           pfnGetGlyphAlignmentEndLine;
    void*                           pfnGetPriorityForGoodTypography;

    EnumText                        pfnEnumText;
    EnumTab                         pfnEnumTab;
    void*                           pfnEnumPen;
    GetObjectHandlerInfo            pfnGetObjectHandlerInfo;

    // Debugging
    AssertFailed                    pfnAssertFailedPtr;

    // Even more LSCONTEXTINFO
    INT                             fDontReleaseRuns;
};

struct LscbkRedefined
{
    FetchRunRedefined        pfnFetchRunRedefined;
    GetGlyphsRedefined       pfnGetGlyphsRedefined;
    FetchLineProps           pfnFetchLineProps;
};

struct LoContext {
    struct LsContextInfo contextInfo;      // const
    struct LscbkRedefined lscbkRedef;
};

