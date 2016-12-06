#pragma once

#include <KR3/main.h>
#include <KR3/meta/text.h>

namespace kr
{
	template <typename C>
	class AddSlashes: public Bufferable<AddSlashes<C>,BufferInfo<C>>
	{
	private:
		const RefArray<C> m_text;
		size_t m_size;

	public:
		AddSlashes(RefArray<C> text) noexcept
			:m_text(text)
		{
			static const auto SLASH_TARGET = meta::literal_as<C>("\r\n\t\'\"");
			m_size = text.count_y(SLASH_TARGET) + text.size();
		}

		size_t copyTo(C * dest) const noexcept
		{
			C * d = dest;
			for (C s : m_text)
			{
				switch (s)
				{
				case (C)'\r': *d++ = '\\'; *d++ = 'r'; break;
				case (C)'\n': *d++ = '\\'; *d++ = 'n'; break;
				case (C)'\t': *d++ = '\\'; *d++ = 't'; break;
				case (C)'\'': *d++ = '\\'; *d++ = '\''; break;
				case (C)'\"': *d++ = '\\'; *d++ = '\"'; break;
				default: *d++ = s; break;
				}
			}
			return d - dest;
		}
		size_t size() const noexcept
		{
			return m_size;
		}
	};

	template <typename C>
	class StripSlashes : public Printable<StripSlashes<C>,C>
	{
	private:
		const RefArray<C> m_text;

	public:
		StripSlashes(RefArray<C> text) noexcept
			: m_text(text)
		{
		}

		template <class _Derived, typename _Info>
		void writeTo(OutStream<_Derived, Component, _Info> * os) const
		{
			RefArray<C> text = m_text;
			while (!text.empty())
			{
				if (*text != '\\')
				{
					*os << *text++;
					continue;
				}
				text++;
				if (text.empty()) break;
				switch (*text++)
				{
				case 'R':
				case 'r': *os << '\r'; break;
				case 'N':
				case 'n': *os << '\n'; break;;
				case 'T':
				case 't': *os << '\t'; break;
				case '\\': *os << '\\'; break;
				case '\'': *os << '\''; break;
				case '\"': *os << '\"'; break;
				}
			}
		}
	};

	inline AddSlashes<char> addslashes(Text text) noexcept { return text; }
	inline AddSlashes<wchar> addslashes(TextW text) noexcept { return text; }
	inline AddSlashes<char16> addslashes(Text16 text) noexcept { return text; }
	inline AddSlashes<char32> addslashes(Text32 text) noexcept { return text; }
	inline StripSlashes<char> stripslashes(Text text) noexcept { return text; }
	inline StripSlashes<wchar> stripslashes(TextW text) noexcept { return text; }
	inline StripSlashes<char16> stripslashes(Text16 text) noexcept { return text; }
	inline StripSlashes<char32> stripslashes(Text32 text) noexcept { return text; }
}
