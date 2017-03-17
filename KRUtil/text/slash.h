#pragma once

#include <KR3/main.h>
#include <KR3/meta/text.h>

namespace kr
{
	template <typename C>
	class AddSlashes: public Printable<AddSlashes<C>,C>
	{
	private:
		const RefArray<C> m_text;

	public:
		AddSlashes(RefArray<C> text) noexcept
			:m_text(text)
		{
		}

		template <class _Derived, typename _Info>
		void writeTo(OutStream<_Derived, C, _Info> * os) const
		{
			for (C s : m_text)
			{
				switch (s)
				{
				case (C)'\r': *os << '\\'; *os << 'r'; break;
				case (C)'\n': *os << '\\'; *os << 'n'; break;
				case (C)'\t': *os << '\\'; *os << 't'; break;
				case (C)'\'': *os << '\\'; *os << '\''; break;
				case (C)'\"': *os << '\\'; *os << '\"'; break;
				default: *os << s; break;
				}
			}
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
		void writeTo(OutStream<_Derived, C, _Info> * os) const
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
