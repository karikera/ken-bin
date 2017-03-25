#pragma once

#include "../main.h"
#include "../io/bufferedstream.h"

namespace kr
{
	class Process : public InStream<Process, char>
	{
	public:
		enum Shell_t { Shell };
		Process() noexcept;
		Process(Shell_t, TextW command) noexcept;
		Process(pcwstr fileName, pwstr parameter) noexcept;
		~Process() noexcept;

		void close() noexcept;
		void shell(TextW command, pcwstr curdir = nullptr);
		void exec(pcwstr fileName, pwstr parameter, pcwstr curdir = nullptr);
		size_t readImpl(char * dest, size_t sz);

		int getExitCode() noexcept;

#ifdef WIN32
		void executeOpen(pcwstr path) noexcept;
#else
		void executeOpen(pcstr path) noexcept;
#endif

		// 0을 반환하면 성공.
		static int execute(pwstr pszstr) noexcept;

		// 0을 반환하면 성공.
		static int detachedExecute(pwstr pszstr) noexcept;

	private:
#ifdef WIN32
		void* m_process;
		void* m_stdout_read;
#endif

	};
}
