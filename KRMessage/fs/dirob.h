#pragma once

#include <KR3/main.h>
#include <KR3/mt/thread.h>
#include <KRUtil/fs/file.h>

namespace kr
{
	class DirectoryObserver :private Threadable<DirectoryObserver>
	{
		friend Threadable<DirectoryObserver>;
	public:
		DirectoryObserver() noexcept;
		~DirectoryObserver() noexcept;
		void start(const char16 * dir); // Error
		void quit(int exitCode) noexcept;
		int thread() noexcept;
		virtual void onCreate(Text16 name) noexcept = 0;
		virtual void onDelete(Text16 name) noexcept = 0;
		virtual void onModified(Text16 name) noexcept = 0;
		virtual void onRename(Text16 oldname, Text16 newname) noexcept = 0;

	private:
		File * m_dir;
		dword m_id;
	};
}
