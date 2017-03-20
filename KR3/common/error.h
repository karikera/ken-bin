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
		template <typename C>
		TempSzText<C> getMessage() const noexcept;
		static ErrorCode getLast() noexcept;

	private:
		HRESULT m_error;

	};

	template <> TempSzText<char> ErrorCode::getMessage<char>() const noexcept;
	template <> TempSzText<wchar> ErrorCode::getMessage<wchar>() const noexcept;
	
	void errorBox(pcwstr str) noexcept;

}