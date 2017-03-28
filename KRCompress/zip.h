#pragma once

#include <KR3/main.h>

struct tm_unz_s;

namespace kr
{
	class Unzipper
	{
	public:
		Unzipper();
		virtual ~Unzipper();

		bool open(pcstr16 szFileName);
		int extract(pcstr16 szDestination);
		void close();

	private:
		int _extractCurrentFile(pcstr16 dest);
		int _writeFile(File * file) noexcept;
		void _changeFileDate(pcstr16 filename, dword dosdate, const tm_unz_s* tmu_date);

	private:
		void * m_uzFile;
	};
}
