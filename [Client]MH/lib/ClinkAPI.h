#ifdef CLINKAPI_EXPORTS
#define CLINKAPI_API __declspec(dllexport)
#else
#define CLINKAPI_API __declspec(dllimport)
#endif




/**
启动客户端安全接入组件(只能启动一次)
key：sdk配置密钥
返回150表示成功，其它的为失败
*/
extern "C" CLINKAPI_API int clinkStart(const char * key);
/*停止 注意：停止后只有进程重启后才可以再重新调用clinkStart函数，如果直接调用会出错*/
extern "C" CLINKAPI_API void clinkStop();

