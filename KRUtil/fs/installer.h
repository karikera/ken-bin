#pragma once

namespace kr
{
	class Installer
	{
	public:
		Installer(TextW dest, TextW src) noexcept;

		void all() noexcept;
		void filter(pcwstr filterText, TextW dirname) noexcept;
		void dir(TextW dirname) noexcept;
		void file(TextW filename) noexcept;
		void copy(pcwstr dest, pcwstr src, TextW msg) noexcept;

	private:
		ATextW m_src;
		ATextW m_dest;
	};
}
