#include <Windows.h>
#include <string>

#ifdef UNICODE
typedef std::wstring ustring;
#else
typedef std::string ustring;
#endif

////////////////////////////////////
// uprintf
#ifdef PRINT_CMD
#undef PRINT_LOG
#undef PRINT_NOPRINT
#define uprintf printf
#endif

#ifdef PRINT_LOG
#undef PRINT_NOPRINT
#define uprintf
#endif

#ifndef PRINT_CMD
#ifndef PRINT_LOG
#define uprintf
#endif
#endif
///////////////////////////////////