#include "loservice.h"

struct EscStringInfo
{
    SHORT*   szParaSeparator;
    SHORT*   szLineSeparator;
    SHORT*   szHidden;
    SHORT*   szNbsp;
    SHORT*   szObjectTerminator;
    SHORT*   szObjectReplacement;
};

void WINAPI LoGetEscString(struct EscStringInfo* escStringInfo)
{
    static SHORT esc_strings[] = { 0x2029, 0x2028, 0xffff, 0x00a0, 0xfffb, 0xfffc };

    escStringInfo->szParaSeparator = esc_strings + 0;
    escStringInfo->szLineSeparator = esc_strings + 1;
    escStringInfo->szHidden = esc_strings + 2;
    escStringInfo->szNbsp = esc_strings + 3;
    escStringInfo->szObjectTerminator = esc_strings + 4;
    escStringInfo->szObjectReplacement = esc_strings + 5;
}

enum LsErr WINAPI LoCreateContext(struct LsContextInfo* contextInfo, struct LscbkRedefined* lscbkRedef, struct LoContext** ploc)
{
    struct LoContext *loc = *ploc = malloc(sizeof(*loc));
    loc->contextInfo = *contextInfo;
    loc->lscbkRedef = *lscbkRedef;

    return None;
}

enum LsErr WINAPI LoSetDoc(struct LoContext* ploc, INT isDisplay, INT isReferencePresentationEqual, struct LsDevRes* deviceInfo)
{
    return None;
}

enum LsErr WINAPI LoSetBreaking(struct LoContext* ploc, INT strategy)
{
    return None;
}

enum LsErr WINAPI LoSetTabs(struct LoContext* ploc, INT durIncrementalTab, INT tabCount, struct LsTbd* pTabs)
{
    return None;
}

enum LsErr LoDestroyContext(struct LoContext* ploc)
{
    free(ploc);

    return None;
}

