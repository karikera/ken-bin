
//#undef WINAPI
//#undef WINBASEAPI
//#undef CP_UTF8

#ifdef _WINDOWS_
#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))
#endif
