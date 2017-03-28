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
		Process(Shell_t, Text16 command) noexcept;
		Process(pcstr16 fileName, pstr16 parameter) noexcept;
		~Process() noexcept;

		void close() noexcept;
		void shell(Text16 command, pcstr16 curdir = nullptr);
		void exec(pcstr16 fileName, pstr16 parameter, pcstr16 curdir = nullptr);
		size_t readImpl(char * dest, size_t sz);

		int getExitCode() noexcept;

#ifdef WIN32
		void executeOpen(pcstr16 path) noexcept;
#else
		void executeOpen(pcstr path) noexcept;
#endif

		// 0을 반환하면 성공.
		static int execute(pstr16 pszstr) noexcept;

		// 0을 반환하면 성공.
		static int detachedExecute(pstr16 pszstr) noexcept;

	private:
#ifdef WIN32
		void* m_process;
		void* m_stdout_read;
#endif

	};
}
