#pragma once

#include <KR3/main.h>
#include <KR3/meta/text.h>

namespace kr
{
	template <typename C, typename LAMBDA>
	class CustomAddSlashes : public Printable<CustomAddSlashes<C, LAMBDA>, C>
	{
	private:
		const RefArray<C> m_text;
		const LAMBDA & m_lambda;

	public:
		CustomAddSlashes(RefArray<C> text, const LAMBDA & lambda) noexcept
			: m_text(text), m_lambda(lambda)
		{
		}

		template <class _Derived, typename _Info>
		void writeTo(OutStream<_Derived, C, _Info> * os) const
		{
			for (C s : m_text)
			{
				C rep = m_lambda(s);
				if (rep != (C)'\0')
				{
					*os << '\\';
					*os << rep;
				}
				else
				{
					*os << s;
				}
			}
		}
	};

	template <typename C>
	class AddSlashes : public Printable<AddSlashes<C>, C>
	{
	private:
		const RefArray<C> m_text;

	public:
		AddSlashes(RefArray<C> text) noexcept
			: m_text(text)
		{
		}

		template <class _Derived, typename _Info>
		void writeTo(OutStream<_Derived, C, _Info> * os) const
		{
			for (C s : m_text)
			{
				switch (s)
				{
				case (C)'\0': *os << '\\'; *os << '0'; break;
				case (C)'\r': *os << '\\'; *os << 'r'; break;
				case (C)'\n': *os << '\\'; *os << 'n'; break;
				case (C)'\t': *os << '\\'; *os << 't'; break;
				case (C)'\'': *os << '\\'; *os << '\''; break;
				case (C)'\"': *os << '\\'; *os << '\"'; break;
				case (C)'\\': *os << '\\'; *os << '\\'; break;
				default: *os << s; break;
				}
			}
		}
	};

	template <typename C>
	class StripSlashes : public Printable<StripSlashes<C>, C>
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
				case '0': *os << '\0'; break;
				}
			}
		}
	};

	template <typename LAMBDA>
	inline auto custom_addslashes(Text text, const LAMBDA& lambda) noexcept->CustomAddSlashes<char, LAMBDA>
	{
		return CustomAddSlashes<char, LAMBDA>(text, lambda);
	}
	template <typename LAMBDA>
	inline auto custom_addslashes(Text16 text, const LAMBDA& lambda) noexcept->CustomAddSlashes<char16, LAMBDA>
	{
		return CustomAddSlashes<char16, LAMBDA>(text, lambda);
	}
	template <typename LAMBDA>
	inline auto custom_addslashes(Text32 text, const LAMBDA& lambda) noexcept->CustomAddSlashes<char32, LAMBDA>
	{
		return CustomAddSlashes<char32, LAMBDA>(text, lambda);
	}

	inline AddSlashes<char> addslashes(Text text) noexcept { return text; }
	inline AddSlashes<char16> addslashes(Text16 text) noexcept { return text; }
	inline AddSlashes<char32> addslashes(Text32 text) noexcept { return text; }
	inline StripSlashes<char> stripslashes(Text text) noexcept { return text; }
	inline StripSlashes<char16> stripslashes(Text16 text) noexcept { return text; }
	inline StripSlashes<char32> stripslashes(Text32 text) noexcept { return text; }
}
