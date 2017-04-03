#pragma once

#include "../main.h"
#include "../meta/number.h"

namespace kr
{
	struct ThrowAbort {};

	struct Exception {};
	struct DuplicateException {};
	struct NotFoundException:Exception {};
	struct NotEnoughSpaceException:Exception {};
	struct EofException:Exception {};
	struct OutOfRangeException:Exception {};
	struct InvalidSourceException:Exception {};
	struct TooBigException:Exception {};

	struct Error:Exception {};
	struct QuitException:Exception
	{
		int exitCode;
		inline QuitException(int code) noexcept
		{
			exitCode = code;
		}
	};
	struct UnsupportedException:Exception {};

	struct DisconnectException:Exception {};

	ATTR_NORETURN void quit(int exitCode); // QuitException
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