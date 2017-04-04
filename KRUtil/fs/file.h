#pragma once

#include <KR3/main.h>
NEED_FILESYSTEM

#include "../uts.h"

namespace kr
{
	class File: public io::Streamable<File>
	{
	public:
		class Mapping
		{
			friend File;
		public:
			ptr point;
			dword offset;

		private:
			void* handle;
		};

#ifdef WIN32
		static constexpr uint NAMELEN = 260;
#else
		static constexpr uint NAMELEN = 512; // 적당히 큰 값을 사용하였다.
#endif

		File() = delete;
		template <typename CHR>
		static File* createT(const CHR * str); // Error
		template <typename CHR>
		static File* openT(const CHR * str); // Error
		template <typename CHR>
		static File* openAndWriteT(const CHR * str); // Error
		template <typename CHR>
		static File* createAndReadT(const CHR * str); // Error

		static File* create(const char * str) { return createT(str); }
		static File* create(const char16 * str) { return createT(str); }
		static File* open(const char * str) { return openT(str); }
		static File* open(const char16 * str) { return openT(str); }
		static File* openAndWrite(const char * str) { return openAndWriteT(str); }
		static File* openAndWrite(const char16 * str) { return openAndWriteT(str); }
		static File* createAndRead(const char * str) { return createAndReadT(str); }
		static File* createAndRead(const char16 * str) { return createAndReadT(str); }
		static void operator delete(ptr p) noexcept;

		//DefineWritablePropertySZ(File, name, GetName, CPUTF16);

		static bool exists(pcstr16 src) noexcept;
		static bool copy(pcstr16 dest, pcstr16 src) noexcept;
		static bool move(pcstr16 dest, pcstr16 src) noexcept;
		static bool copyTree(pcstr16 dest, pcstr16 src) noexcept;
		static bool toJunk(pcstr16 src) noexcept;
		static bool clearJunk() noexcept;
		static bool remove(pcstr16 str) noexcept;
		static bool isFile(pcstr16 str) noexcept;
		static bool isDirectory(pcstr16 str) noexcept;
		static bool isDirectoryModified(Text16 dir, filetime_t axis) noexcept;
		static bool createDirectory(pcstr16 str) noexcept;
		static bool createFullDirectory(Text16 str) noexcept;
		static bool removeFullDirectory(Text16 path) noexcept;
		static bool removeShell(Text16 path) noexcept;

		template <typename T>
		inline Array<T> readAll() // TooBigException
		{
			dword size = size32();
			Array<T> arr(size);
			read(arr.begin(), size);
			return arr;
		}
		template <typename T>
		inline TmpArray<T> readAllTemp() // TooBigException
		{
			dword size = size32();
			TmpArray<T> arr(size / sizeof(T));
			read(arr.begin(), size / sizeof(T) * sizeof(T));
			return arr;
		}
		template <typename T, typename CHR>
		static inline Array<T> openAsArrayT(const CHR * name) // TooBigException, Error
		{
			Must<File> file = open(name);
			return file->readAll<T>();
		}
		template <typename T>
		static inline Array<T> openAsArray(const char * name) // TooBigException, Error
		{
			return openAsArrayT<T,char>(name);
		}
		template <typename T>
		static inline Array<T> openAsArray(const char16 * name) // TooBigException, Error
		{
			return openAsArrayT<T, char16>(name);
		}
		template <typename T, typename CHR>
		static inline TmpArray<T> openAsArrayTempT(const CHR * name) // TooBigException, Error
		{
			Must<File> file = open(name);
			return file->readAllTemp<T>();
		}
		template <typename T>
		static inline TmpArray<T> openAsArrayTemp(const char * name) // TooBigException, Error
		{
			return openAsArrayTempT<T, char>(name);
		}
		template <typename T>
		static inline TmpArray<T> openAsArrayTemp(const char16 * name) // TooBigException, Error
		{
			return openAsArrayTempT<T, char16>(name);
		}
		template <typename T, typename CHR>
		static inline void writeAsArrayT(const CHR * name, RefArray<T> & arr) // TooBigException, Error
		{
			Must<File> file = create(name);
			return file->write(arr.begin(), arr.sizeBytes());
		}
		template <typename T>
		static inline void writeFromArray(const char * name, RefArray<T> & arr) // Error
		{
			return writeAsArrayT(name, arr);
		}
		template <typename T>
		static inline void writeFromArray(const char16 * name, RefArray<T> & arr) // Error
		{
			return writeAsArrayT(name, arr);
		}
		filesize_t getPointer() noexcept;
		void movePointerToEnd(int offset) noexcept;
		void movePointerToEnd(int64_t offset) noexcept;
		void movePointer(int Move) noexcept;
		void movePointer(int64_t Move) noexcept;
		void setPointer(int Move) noexcept;
		void setPointer(int64_t Move) noexcept;
		void toBegin() noexcept;
		void toEnd() noexcept;
		void skip(int64_t skip) noexcept;
		bool testSignature(dword signature) noexcept;
		dword findChunk(dword signature); // EofException
		ptr allocAll(size_t *pSize) noexcept;
		Mapping beginMapping(filesize_t off, size_t read); // Error
		void endMapping(const Mapping& map) noexcept;
		void readStructure(ptr value, uintptr_t size); // EofException
		void readStructure(ptr value, uintptr_t size, uintptr_t srcsize); // EofException

		// 사용 후 파일이 닫힌다.
		void md5All(byte _dest[16]); // Error

		// 지정된 크기만큼 md5 해시를 생성하고 파일 포인터를 읽기 전 위치로 돌려놓는다.
		void md5(size_t sz, byte _dest[16]); // Error

		dword size32(); // TooBigException
		filesize_t size() noexcept;
		void writeImpl(cptr buff, size_t len); // Error
		size_t readImpl(ptr buff, size_t len); // EofException

		static filetime_t getLastModifiedTime(pcstr16 filename); // Error
		filetime_t getLastModifiedTime() noexcept;
		filetime_t getCreationTime() noexcept;
		bool setModifyTime(filetime_t t) noexcept;
		bool setCreationTime(filetime_t t) noexcept;
		template <typename T> void readStructure(T *value) // EofException
		{
			readStructure(value, sizeof(T));
		}
		template <typename T> filesize_t find(T chr) // EofException
		{
			filesize_t len=0;
			dword readed;
			T temp[1024];
			int64_t from=0;
			filesize_t out;
			pcstr find;
			do
			{
				readed=1024*sizeof(T);
				read(temp, readed);
				find=(pcstr)mem::find(temp, chr, readed);
				from-=readed;

				if(!readed)
				{
					out=~(filesize_t)0;
					goto label_failed;
				}
			}
			while(!find);

			out=(find-temp-readed)-from;
		label_failed:
			movePointer(from);
			return out;
		}

	private:
#ifdef WIN32
		static File * _createFile(pcstr str, dword Access, dword ShareMode, dword Disposition); // Error
		static File * _createFile(pcstr16 str, dword Access, dword ShareMode, dword Disposition); // Error
		void _movePointer(dword Method, int Move) noexcept;
		void _movePointer(dword Method, int64_t Move) noexcept;
#endif
	};

	template <typename Component>
	class ModuleName :public Bufferable<ModuleName<Component>, BufferInfo<Component, true, true, false>>
	{
	private:
		BArray<Component, File::NAMELEN> m_buffer;

	public:
		ModuleName(const Component * name = nullptr) noexcept;
		Component * begin() noexcept
		{
			return m_buffer.begin();
		}
		Component * end() noexcept
		{
			return m_buffer.end();
		}
		const Component * begin() const noexcept
		{
			return m_buffer.begin();
		}
		const Component * end() const noexcept
		{
			return m_buffer.end();
		}
		size_t size() const noexcept
		{
			return m_buffer.size();
		}
	};

	class FindFile
	{
	public:
		FindFile() noexcept;
		~FindFile() noexcept;
		FindFile(pcstr16 file) noexcept;
		FindFile(FindFile && o) noexcept;
		FindFile& operator =(FindFile && o) noexcept;

		bool exists() noexcept;
		const char16 * getFileName() noexcept;
		bool isDirectory() noexcept;
		bool next() noexcept;
		filetime_t getLastModifiedTime() noexcept;

		class Iterator
		{
		public:
			Iterator(FindFile * ff) noexcept;
			Iterator& operator ++() noexcept;
			const char16 * operator *() noexcept;
			bool operator == (const Iterator & o) noexcept;
			bool operator != (const Iterator & o) noexcept;
		private:
			FindFile * m_ff;
		};

		Iterator begin() noexcept;
		Iterator end() noexcept;

	private:
		bool _isDotDir() noexcept;
		byte m_buffer[1024 - sizeof(void*)];

		void * m_handle;
	};

	class MappedFile:public Bufferable<MappedFile, BufferInfo<void, true, false, false, Empty>>
	{
	public:
		MappedFile(File * file); // Error, TooBigException
		MappedFile(const char16 * filename); // Error, TooBigException
		~MappedFile() noexcept;
		size_t size() const noexcept;
		void * begin() noexcept;
		void * end() noexcept;
		const void * begin() const noexcept;
		const void * end() const noexcept;

	private:
		Must<File> m_file;
		size_t m_size;
		File::Mapping m_map;

	};


	class DirectoryScanner
	{
	public:
		DirectoryScanner() noexcept;
		template <typename LAMBDA>
		void scan(Text16 path, const LAMBDA &lambda) noexcept;
		template <typename LAMBDA>
		void scanWithThis(Text16 path, const LAMBDA &lambda) noexcept;
		pcstr16 getSzName() noexcept;
		Text16 getPathText() noexcept;
		Text16 getRelativeText(Text16 path) noexcept;
		pcstr16 getRelativeSzName(Text16 path) noexcept;

	private:
		template <typename LAMBDA>
		void _scanOpen(Text16 path, const LAMBDA &lambda) noexcept;

		BText16<File::NAMELEN> m_path;
		pcstr16 m_dircut;
	};

	template <typename LAMBDA>
	void DirectoryScanner::scan(Text16 path, const LAMBDA &lambda) noexcept
	{
		pcstr16 olddircut = m_dircut;
		_scanOpen(path, lambda);
		m_dircut = olddircut;
	}

	template <typename LAMBDA>
	void DirectoryScanner::scanWithThis(Text16 path, const LAMBDA &lambda) noexcept
	{
		pcstr16 olddircut = m_dircut;
		_scanOpen(path, lambda);
		m_path.cut_self(m_dircut - 1);
		lambda(m_path);
		m_dircut = olddircut;
	}

	template <typename LAMBDA>
	void DirectoryScanner::_scanOpen(Text16 path, const LAMBDA &lambda) noexcept
	{
		m_path.cut_self(m_dircut);
		m_path << path;

		m_dircut = m_path.end() + 1;
		m_path << u"/*.*" << nullterm;
		Temp<FindFile> file(m_path.begin());

		if (file->exists()) do
		{
			if (file->isDirectory())
			{
				scanWithThis((Text16)file->getFileName(), lambda);
			}
			else
			{
				m_path.cut_self(m_dircut);
				m_path << (Text16)file->getFileName();
				lambda(m_path);
			}
		}
		while (file->next());
	}

}
