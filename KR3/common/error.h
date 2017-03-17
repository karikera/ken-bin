#pragma once

#ifdef WIN32

typedef __success(return >= 0) long HRESULT;

#else

#include <stdint.h>
typedef int32_t HRESULT;

#endif

namespace kr
{
	class ErrorCode:public Bufferable<ErrorCode, BufferInfo<AutoComponent, false, true> >
	{
	public:
		ErrorCode(HRESULT error) noexcept;
		operator HRESULT() const noexcept;
		void print() noexcept;
		static ErrorCode getLast() noexcept;

		template <typename C>
		size_t copyTo(C * dest) const noexcept;

		template <typename C>
		size_t sizeAs() const noexcept;

	private:
		HRESULT m_error;

	};

	template <> size_t ErrorCode::copyTo<char>(char * dest) const noexcept;
	template <> size_t ErrorCode::copyTo<wchar>(wchar * dest) const noexcept;

	template <> size_t ErrorCode::sizeAs<char>() const noexcept;
	template <> size_t ErrorCode::sizeAs<wchar>() const noexcept;
	
	void errorBox(pcwstr str) noexcept;

}