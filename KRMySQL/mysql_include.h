#pragma once

#ifdef __MYSQL_REFERENCE_H
#error include before mysql_ref.h 
#endif

#include <math.h>
#include <time.h>

#include <KR3/wl/prepare.h>
#pragma warning(push)
#pragma warning(disable:4996)
#undef _WIN32_WINNT
#include <my_global.h>
#include <mysql.h>
#pragma warning(pop)
#include <KR3/wl/clean.h>
