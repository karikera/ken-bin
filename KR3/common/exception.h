#pragma once

#include "../main.h"
#include "../meta/number.h"
#include <exception>

typedef struct st_mysql_stmt MYSQL_STMT;
typedef struct st_mysql MYSQL;

namespace kr
{
	enum class HttpStatus :uint;

	using std::current_exception;
	using std::exception_ptr;
	using std::make_exception_ptr;

	struct Exception {};
	struct UnknownException : Exception {};
	struct ThrowAbort : Exception {};
	struct ThrowRetry : Exception {};
	struct ThrowAllocate : Exception {};
	struct DuplicateException : Exception {};
	struct NotFoundException : Exception {};
	struct NotEnoughSpaceException : Exception {};
	struct EofException : Exception {};
	struct OutOfRangeException : Exception {};
	struct InvalidSourceException : Exception {};
	struct TooBigException : Exception {};
	struct UnsupportedException : Exception {};
	struct DisconnectException : Exception {};
	struct HttpException : Exception {
		HttpStatus status;
		inline HttpException(HttpStatus value) noexcept :status(value) {}
	};
	struct QuitException : Exception {
		int exitCode;
		inline QuitException(int value) noexcept :exitCode(value) {}
	};
	struct Error : Exception {};
	struct SqlException : Exception {};
	struct CurlError : Error {
		uint value;
		inline CurlError(uint value) noexcept :value(value) {}
	};
	struct MciError : Error {
		uint value;
		inline MciError(uint value) noexcept :value(value) {}
	};
	struct ZlibError :Error {
		int value;
		inline ZlibError(int value) noexcept :value(value) {}
	};

	ATTR_NORETURN void quit(int exitCode) throw(QuitException);
	ATTR_NORETURN void error(const char * strMessage, ...) noexcept;
	void warning(const char * strMessage, ...) noexcept;
	template <typename T> bool intactTest(uintptr_t v) noexcept
	{
		if (sizeof(T) > sizeof(uintptr_t)) return true;
		if (sizeof(T) == sizeof(uintptr_t) && std::is_unsigned<T>::value) return true;
		return v < maxof(T);
	}
	template <typename T> T intact(uintptr_t v) noexcept
	{
		_assert(intactTest<T>(v));
		return (T)v;
	}
}

#define shouldbe(cmp)	((cmp) ? true : (::kr::warning(#cmp), false))
#define mustbe(cmp)		{ if(!(cmp)) ::kr::error(#cmp); }