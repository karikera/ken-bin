#pragma once

#include <KR3/main.h>

namespace kr
{
	class CurrentDirectory : public Bufferable<CurrentDirectory, BufferInfo<AutoComponent, false, true>>
	{
	public:
		template <typename CHR>
		bool set(const CHR * text) const noexcept;
		template <typename CHR>
		size_t copyTo(CHR * dest) const noexcept;
		template <typename CHR>
		size_t sizeAs() const noexcept;
	};

	static CurrentDirectory &currentDirectory = nullref;

	class Path
	{
	public:
		constexpr static size_t MAX_LEN = 512;

		Path() noexcept;
		Text16 get(Text16 filename) throws(NotEnoughSpaceException);
		pcstr16 getsz(Text16 filename) throws(NotEnoughSpaceException);
		char16 * enter(Text16 name) throws(NotEnoughSpaceException);
		void leave(char16 * path) noexcept;
		pcstr16 getCurrentDirectorySz() noexcept;

	private:
		char16 * m_path;
		char16 m_buffer[MAX_LEN];
	};

	template <typename C>
	class path_t
	{
	private:
#ifdef WIN32
		static const C SEPERATOR[3];
#endif
	public:
#ifdef WIN32
		static constexpr C sep = (C)'\\';
#else
		static constexpr C sep = (C)'/';
#endif

		using Text = View<C>;
		using TSZ = TempSzText<C>;

		// '/' -> true
		// '\\' -> true (if win32)
		static bool isSeperator(C chr) noexcept
		{
			switch (chr)
			{
			case sep: return true;
#ifdef WIN32
			case '/': return true;
#endif
			default: return false;
			}
		}

		// "/dirname" -> true
		// "\\dirname" -> true (if win32)
		// "dirname" -> false
		static bool startsWithSeperator(Text text) noexcept
		{
			if (text.empty()) return false;
			return isSeperator(text.front());
		}

		// "dirname/" -> true
		// "dirname\\" -> true (if win32)
		// "dirname" -> false
		static bool endsWithSeperator(Text text) noexcept
		{
			if (text.empty()) return false;
			return isSeperator(text.back());
		}

		// "dir/name/basename.ext" -> "dir/name/"
		// "basename.ext" -> ""
		static Text dirname(Text text) noexcept
		{
			return text.cut(basename(text));
		}

		// "dir/name/basename.ext.name" -> "basename"
		// "dir/name/.ext.name" -> ""
		static Text filenameOnly(Text text) noexcept
		{
			Text name = basename(text);
			Text find = text.find_e('.');
			return name.cut(find);
		}

		// "dir/name/basename.ext" -> "basename.ext"
		// "basename.ext" -> "basename.ext"
		static Text basename(Text text) noexcept
		{
			size_t pos;
#ifdef WIN32
			pos = text.pos_ry(SEPERATOR);
#else
			pos = text.pos_r(sep);
#endif
			return text.subarr(pos + 1);
		}

		// "dirname/basename.ext.name" -> ".ext.name"
		// "dirname/basename" -> ""
		static Text extname(Text text) noexcept
		{
			return basename(text).find_e((C)'.');
		}

		template <typename ... T>
		static void joinAppend(TSZ * dest, const T & ... inputs) noexcept
		{
			std::initializer_list<Text> texts = { inputs ... };

			const Text * iter = texts.begin();
			const Text * end = texts.end();
			if (iter == end) return;

			Text text = *iter;
			if (startsWithSeperator(text))
			{
				text++;
				*dest << sep;
			}
			bool sep_end = false;
			if ((sep_end = endsWithSeperator(text)))
			{
				text.cut_self(text.end() - 1);
			}
			*dest << text;
			iter++;
			for (; iter != end; iter++)
			{
				text = *iter;
				if (startsWithSeperator(text))
				{
					text++;
				}
				*dest << sep;
				if ((sep_end = endsWithSeperator(text)))
				{
					text.cut_self(text.end() - 1);
				}
				*dest << text;
			}
			if (sep_end) *dest << sep;
		}

		// ("dir/", "/name" ,"basename.ext") -> "dir/name/basename.ext"
		// ("/dir/", "/name" ,"nextdir/") -> "/dir/name/nextdir/"
		template <typename ... T>
		static TSZ join(const T & ... inputs) noexcept
		{
			TSZ dest;
			joinAppend(&dest, inputs...);
			return dest;
		}

		static void resolveAppend(TSZ * dest, Text path) noexcept
		{
			switch (path.size())
			{
			case 0: *dest << currentDirectory; break;
			case 1:
				switch (path[0])
				{
				case sep:
#ifdef WIN32
				case (C)'/': return;
#endif
				case (C)'.':
					*dest << currentDirectory;
					break;
				default:
					*dest << currentDirectory << sep << path[0];
					break;
				}
				break;
			default:
				Text read = path;
				if (isSeperator(*read))
				{
					read++;
				}
#ifdef WIN32
				else if (path[1] == (C)':')
				{
					*dest << path[0] << (C)':';
					read += 2;
					if (read.empty()) break;
					if (isSeperator(*read))
						read++;
				}
#endif
				else
				{
					*dest << currentDirectory;
				}
				while (!read.empty())
				{
					Text dir;
#ifdef WIN32
					dir = read.readwith_ye(SEPERATOR);
#else
					dir = read.readwith_e(sep);
#endif
					switch (dir.size())
					{
					case 0: continue;
					case 1:
						if (dir[0] == '.') continue;
						break;
					case 2:
						if (dir[0] == '.' && dir[1] == '.')
						{
							Text parent = dest->find_r(sep);
							if (parent != nullptr) dest->cut_self(parent);
							continue;
						}
						break;
					}
					*dest << sep << dir;
				}
				break;
			}
		}

		// "dirname/../../basename.ext" -> "/absolute/path/basename.ext"
		static TSZ resolve(Text path) noexcept
		{
			TSZ dest;
			resolveAppend(&dest, path);
			return dest;
		}
	};

#ifdef WIN32
	template <typename C>
	const C path_t<C>::SEPERATOR[3] = { (C)'\\', (C)'/', (C)'\0' };
#endif

	extern template class path_t<char>;
	extern template class path_t<char16>;
	static path_t<char> & path = nullref;
	static path_t<char16> & path16 = nullref;
}

