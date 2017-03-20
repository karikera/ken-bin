#pragma once

namespace kr
{
	class Installer
	{
	public:
		Installer(TextW dest, TextW src) noexcept;

		pcwstr getSrcSz(TextW name) noexcept;
		pcwstr getDestSz(TextW name) noexcept;
		TextW getSrc(TextW name) noexcept;
		TextW getDest(TextW name) noexcept;
		void all() noexcept;
		void filter(TextW filterText) noexcept;
		void dir(TextW dirname) noexcept;
		void file(TextW filename) noexcept;
		void copy(TextW dest, TextW src, TextW msg) noexcept;

		size_t m_copyCount;
		size_t m_skipCount;
		size_t m_errorCount;

	private:
		ATextW m_src;
		ATextW m_dest;
	};
}
