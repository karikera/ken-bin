#pragma once

#ifndef WIN32
#error is not windows system
#endif

#include <KR3/main.h>

namespace kr
{
	class Process
	{
	public:
		// 0�� ��ȯ�ϸ� ����.
		static int execute(pwstr pszstr) noexcept;

		// 0�� ��ȯ�ϸ� ����.
		static int detachedExecute(pwstr pszstr) noexcept;

	};
}