#ifdef CLINKAPI_EXPORTS
#define CLINKAPI_API __declspec(dllexport)
#else
#define CLINKAPI_API __declspec(dllimport)
#endif




/**
�����ͻ��˰�ȫ�������(ֻ������һ��)
key��sdk������Կ
����150��ʾ�ɹ���������Ϊʧ��
*/
extern "C" CLINKAPI_API int clinkStart(const char * key);
/*ֹͣ ע�⣺ֹͣ��ֻ�н���������ſ��������µ���clinkStart���������ֱ�ӵ��û����*/
extern "C" CLINKAPI_API void clinkStop();

