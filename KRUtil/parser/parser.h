#pragma once

#include <KR3/main.h>
#include <KR3/io/bufferedstream.h>

namespace kr
{
	class Parser
	{
	public:
		Parser(io::VIStream<char> is) noexcept;

		bool isNextWhiteSpace();
		void skipWhiteSpace();
		void mustWhiteSpace(); // InvalidSourceException
		int getLine() noexcept;
		template <typename _Derived, typename _Info>
		size_t readToSpace(OutStream<_Derived, char, _Info> * os);
		TSZ readToSpace();

		template <typename _Derived, typename _Info>
		void readWithUnslash(OutStream<_Derived, char, _Info> * dest, char needle)
		{
			for (;;)
			{
				m_is.readto_F(dest, [&](Text text) {
					const char needles[] = { (char)'\\', needle };
					Text finded = text.find_y(Text(needles, countof(needles)));
					if (finded != nullptr)
						m_line += (int)text.cut(finded).count('\n');
					return finded.begin();
				});
				char findedchr = m_is.read();

				if (findedchr == '\\')
				{
					char chr = m_is.read();
					switch (chr)
					{
					case 'a': dest->write('\a'); break;
					case 'b': dest->write('\b'); break;
					case 'f': dest->write('\f'); break;
					case 'r': dest->write('\r'); break;
					case 'n': dest->write('\n'); break;
					case 't': dest->write('\t'); break;
					case '\"':
					case '\'':
					case '\\':
					default:
						dest->write(chr);
						break;
					}
				}
				else return;
			}
		}
		TSZ readWithUnslash(char needle);
		void skipWithUnslash(char needle);

	protected:
		io::BufferedIStream<io::VIStream<char>, true> m_is;
		int m_line;
	};

	template <typename _Derived, typename _Info>
	size_t Parser::readToSpace(OutStream<_Derived, char, _Info> * os)
	{
		return m_is.readto_y(os, Text::WHITE_SPACE);
	}
}