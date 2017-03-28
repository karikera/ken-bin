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

	template <> TSZ ErrorCode::getMessage<char>() const noexcept;
	template <> TSZ16 ErrorCode::getMessage<char16>() const noexcept;
	
	void errorBox(pcstr16 str) noexcept;

}