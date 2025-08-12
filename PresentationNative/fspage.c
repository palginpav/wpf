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
        void** pInstalledObjects, INT* cInstalledObjects)
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

enum LsErr WINAPI FsCreatePageBottomless(struct DocContext* pfscontext, void* fsnmsect, enum FSFMTRBL *pfsfmtrbl, void **ppfspage)
{
    return None;
}

enum LsErr WINAPI DestroyDocContext(struct DocContext* pfscontext)
{
    free(pfscontext);

    return None;
}

enum LsErr WINAPI DestroyInstalledObjectsInfo(void* pInstalledObjects)
{
    free(pInstalledObjects);

    return None;
}

