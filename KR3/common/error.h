#pragma once

#ifdef WIN32

typedef __success(return >= 0) long HRESULT;

#else

#include <stdint.h>
typedef int32_t HRESULT;

#endif

namespace kr
{
	class ErrorCode
	{
	public:
		ErrorCode(HRESULT error) noexcept;
		operator HRESULT() const noexcept;
		void print() noexcept;
		static ErrorCode getLast() noexcept;

		template <typename C>
		size_t getMessage(C * dest, size_t bufferSize) const noexcept;
		template <> size_t getMessage<char>(char * dest, size_t bufferSize) const noexcept;
		template <> size_t getMessage<wchar>(wchar * dest, size_t bufferSize) const noexcept;

	private:
		HRESULT m_error;

	};
	
	void errorBox(pcwstr str) noexcept;

}