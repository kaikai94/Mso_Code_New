#ifndef GLOBAL_FUNC_DLL
#define GLOBAL_FUNC_DLL extern "C" __declspec(dllexport)
#endif

GLOBAL_FUNC_DLL void __stdcall LinkToSortLink(SORT_LINK** ppHead,SORT_LINK** ppTail,SORT_LINK* pNew);

GLOBAL_FUNC_DLL void __stdcall LinkToSortLinkFIFO(SORT_LINK** ppHead,SORT_LINK** ppTail,SORT_LINK* pNew);

GLOBAL_FUNC_DLL void __stdcall UnLinkFromSortLink(SORT_LINK** ppHead,SORT_LINK** ppTail,SORT_LINK* pDel);
