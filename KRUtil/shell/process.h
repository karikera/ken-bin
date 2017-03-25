#pragma once

#ifndef WIN32
#error is not windows system
#endif

#include <KR3/main.h>
#include <KR3/wl/windows.h>
#include <KR3/io/bufferedstream.h>

namespace kr
{
	class Process: public InStream<Process, char>
	{
	public:
		enum Shell_t { Shell };
		enum ShellMove_t { ShellMove };
		Process() noexcept;
		Process(Shell_t, TextW command) noexcept;
		Process(pcwstr fileName, pwstr parameter) noexcept;
		~Process() noexcept;

		void close() noexcept;
		void shell(TextW command, pcwstr curdir = nullptr) noexcept;
		void exec(pcwstr fileName, pwstr parameter, pcwstr curdir = nullptr) noexcept;
		size_t readImpl(char * dest, size_t sz);

		DWORD getExitCode() noexcept;

	private:
		HANDLE m_process;
		HANDLE m_stdout_read;

	};
}
