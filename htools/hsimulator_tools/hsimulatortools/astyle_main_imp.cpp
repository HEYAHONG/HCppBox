#include "HCPPBox.h"
#include "astyle.h"
#include "astyle_main.h"


/*
 * 导入Astyle主程序
 */
#ifdef main
#undef main
#endif // main
#define main astyle_main
#ifdef _
#undef _
#endif
#define _(X) (X)
#ifdef _WIN32
#ifdef UNICODE
#undef UNICODE
#endif // UNICODE
#ifdef GetFileAttributes
#undef GetFileAttributes
#endif // GetFileAttributes
#define GetFileAttributes GetFileAttributesA
#ifdef  FormatMessage
#undef  FormatMessage
#endif //  FormatMessage
#define  FormatMessage  FormatMessageA
#ifdef SHELLEXECUTEINFO
#undef SHELLEXECUTEINFO
#endif // SHELLEXECUTEINFO
#define SHELLEXECUTEINFO SHELLEXECUTEINFOA
#ifdef ShellExecuteEx
#undef ShellExecuteEx
#endif // ShellExecuteEx
#define ShellExecuteEx ShellExecuteExA
#endif
#include "astyle_main.cpp"

