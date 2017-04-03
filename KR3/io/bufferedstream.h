#pragma once

#include "filterstream.h"

namespace kr
{
	namespace io
	{
		template <typename Base, bool autoClose, size_t BUFFER_SIZE>
		class BufferedIStream 
			:public FilterIStream<BufferedIStream<Base, autoClose, BUFFER_SIZE>, Base, autoClose>
		{
		private:
			using Super = FilterIStream<BufferedIStream<Base, autoClose, BUFFER_SIZE>, Base, autoClose>;
			using Component = typename Base::Component;
			using m = memt<sizeof(Component)>;
			using Ref = RefArray<Component>;
			using TSZ = TempSzText<Component>;
			using AText = Array<Component>;

			void _refill() // EofException
			{
				m_filled = m_read = m_buffer;
				m_filled += base()->read(m_buffer, BUFFER_SIZE);
			}
			void _remainFill() // EofException
			{
				size_t remaining = m_filled - m_read;
				if (remaining == 0)
				{
					return _refill();
				}
				if (m_read == m_buffer)
				{
					m_filled += base()->read(m_filled, BUFFER_SIZE - remaining);
				}
				else
				{
					mema::copy(m_buffer, m_read, remaining);
					m_read = m_buffer;
					m_filled = m_buffer + remaining;
					m_filled += base()->read(m_filled, BUFFER_SIZE - remaining);
				}
			}
		public:
			using Super::read;
			using Super::base;

			BufferedIStream(Base* p) noexcept 
				: Super(p)
			{
				m_filled = m_read = m_buffer;
			}
			BufferedIStream(nullptr_t) noexcept 
				: BufferedIStream((Base*)nullptr)
			{
				m_filled = m_read = m_buffer;
			}
			BufferedIStream(typename Base::BaseStream* p) noexcept
				: BufferedIStream(p->template retype<Component>())
			{
			}

			void resetStream(Base * p) noexcept
			{
				m_filled = m_read = m_buffer;
				Super::resetStream(p);
			}
			void resetStream(nullptr_t) noexcept
			{
				resetStream((Base*)nullptr);
			}
			void resetStream(typename Base::BaseStream* p) noexcept
			{
				resetStream(p->template retype<Component>());
			}

			void clearBuffer()
			{
				m_filled = m_read = m_buffer;
			}

			Component peek()
			{
				for (;;)
				{
					if (m_filled > m_read)
					{
						return *m_read;
					}

					_refill();
				}
			}
			Ref peek(size_t count) // TooBigException
			{
				if (count > BUFFER_SIZE) throw TooBigException();
				for (;;)
				{
					if (m_filled >= m_read + count)
					{
						return Ref(m_read, count);
					}

					_remainFill();
				}
			}
			bool nextIs(const Component & comp)
			{
				if (comp != peek()) return false;
				skip(1);
				return true;
			}
			bool nextIs(Ref comps)
			{
				if (comps != peek(comps.size())) return false;
				skip(comps.size());
				return true;
			}
			void must(const Component & comp)
			{
				if (!nextIs(comp)) throw InvalidSourceException();
			}
			void must(Ref comps)
			{
				TmpArray<Component> tmp((size_t)0, comps.size());
				read(&tmp, comps.size());
				if (tmp != comps) throw InvalidSourceException();
			}

			void skip(size_t sz = 1)
			{
				Component * line;
				for (;;)
				{
					line = m_read + sz;
					if (line <= m_filled) break;
					sz = line - m_filled;
					_refill();
				}

				m_read = line;
			}

			template <typename _Derived, typename _Info>
			size_t readLine(OutStream<_Derived, Component, _Info> * dest)
			{
				size_t readed = 0;
				Component * line;
				for (;;)
				{
					size_t remain = m_filled - m_read;
					line = m::find(m_read, (Component)'\n', remain);
					if (line != nullptr) break;
					if (remain != 0)
					{
						Component * last = m_filled;
						Component lastchr = last[-1];
						if (lastchr == (Component)'\r')
						{
							remain--;
							m_filled = m_buffer + 1;
							dest->write(m_read, remain);
							*m_buffer = lastchr;
						}
						else
						{
							m_filled = m_buffer;
							dest->write(m_read, remain);
						}
						readed += remain;
						m_read = m_buffer;
					}

					try
					{
						size_t res = base()->read(m_filled, m_buffer + BUFFER_SIZE - m_filled);
						m_filled += res;
					}
					catch (EofException&)
					{
						if(readed == 0) throw;
						return readed;
					}
				}

				size_t len = line - m_read;
				size_t copylen = len;
				if (line != m_read && line[-1] == '\r') copylen--;
				dest->write(m_read, copylen);
				m_read += len + 1;
				return readed + len;
			}
			TSZ readLine()
			{
				TSZ tsz;
				readLine(&tsz);
				return tsz;
			}
			size_t skipLine()
			{
				size_t readed = 0;
				Component * line;
				for (;;)
				{
					size_t remain = m_filled - m_read;
					line = m::find(m_read, (Component)'\n', remain);
					if (line != nullptr) break;
					if (remain != 0)
					{
						Component * last = m_filled;
						Component lastchr = last[-1];
						if (lastchr == (Component)'\r')
						{
							remain--;
							m_filled = m_buffer + 1;
							*m_buffer = lastchr;
						}
						else
						{
							m_filled = m_buffer;
						}
						readed += remain;
						m_read = m_buffer;
					}

					try
					{
						size_t res = base()->read(m_filled, m_buffer + BUFFER_SIZE - m_filled);
						m_filled += res;
					}
					catch (EofException&)
					{
						if (readed == 0) throw;
						return readed;
					}
				}

				size_t len = line - m_read;
				m_read += len + 1;
				return readed + len;
			}

			template <typename _Derived, typename _Info, typename LAMBDA>
			size_t readto_F(OutStream<_Derived, Component, _Info> * dest, const LAMBDA &lambda)
			{
				size_t totalReaded = 0;
				Component * line;
				for (;;)
				{
					size_t size = m_filled - m_read;
					if (size != 0)
					{
						line = (Component*)lambda(RefArray<Component>(m_read, size));
						if (line != nullptr) break;
						dest->write(m_read, size);
						totalReaded += size;
					}

					_refill();
				}

				size_t len = line - m_read;
				dest->write(m_read, len);
				m_read = line;
				return totalReaded + len;
			}
			template <typename LAMBDA>
			TSZ readto_F(const LAMBDA &lambda)
			{
				TSZ tsz;
				readto_F(&tsz, lambda);
				return tsz;
			}
			template <typename LAMBDA>
			size_t skipto_F(const LAMBDA &lambda)
			{
				size_t readlen = 0;
				Component * line;
				for (;;)
				{
					size_t size = m_filled - m_read;
					if (size != 0)
					{
						line = (Component*)lambda(RefArray<Component>(m_read, size));
						if (line != nullptr) break;
						Component * last = m_filled;
						readlen += size;
					}

					_refill();
				}

				size_t copylen = line - m_read;
				m_read = line;
				return readlen + copylen;
			}

			template <typename _Derived, typename _Info>
			size_t readto(OutStream<_Derived, Component, _Info> * dest, const Component &needle) // TooBigException
			{
				return readto_F(dest, [&](Ref text) {
					return text.find(needle).begin();
				});
			}
			TSZ readto(const Component &chr) // TooBigException
			{
				TSZ tsz;
				return readto(&tsz, chr);
			}
			size_t skipto(const Component &needle)
			{
				return skipto_F([&](Ref text) {
					return text.find(needle).begin();
				});
			}

			template <typename _Derived, typename _Info>
			size_t readto(OutStream<_Derived, Component, _Info> * dest, Ref needle) // TooBigException
			{
				size_t totalReaded = 0;
				size_t needlesize = needle.size();
				if (needlesize >= BUFFER_SIZE / 2) throw TooBigException();
				Component * beg = m_read;
				try
				{
					Component * finded = mem::find_callback([&]{
						while (m_read == m_filled)
						{
							size_t readed = m_filled - beg;
							if (readed > needlesize)
							{
								readed -= needlesize;
								dest->write(beg, readed);
								totalReaded += readed;
								m_read = m_filled - needlesize;
							}
							_remainFill();
							beg = m_buffer;
						}
						return m_read++;
					}, needle.data(), needlesize);

					m_read = finded - needlesize;
					size_t readsize = finded - beg;
					dest->write(beg, readsize);
					return totalReaded + readsize;
				}
				catch (EofException&)
				{
					if (totalReaded == 0) throw;
					dest->write(beg, m_filled - beg);
					clearBuffer();
					return totalReaded;
				}
			}
			AText readto(Ref needle) // TooBigException
			{
				AText text;
				readto(&text, needle);
				return text;
			}
			size_t skipto(Ref needle)
			{
				size_t totalReaded = 0;
				size_t needlesize = needle.size();
				if (needlesize >= BUFFER_SIZE / 2) throw TooBigException();
				Component * beg = m_read;
				try
				{
					Component * finded = mem::find_callback([&]{
						while (m_read == m_filled)
						{
							size_t readed = m_filled - beg;
							if (readed > needlesize)
							{
								totalReaded += readed - needlesize;
								m_read = m_filled - needlesize;
							}
							_remainFill();
							beg = m_buffer;
						}
						return m_read++;
					}, needle.data(), needlesize);

					m_read = finded - needlesize;
					return totalReaded + finded - beg;
				}
				catch (EofException&)
				{
					if (totalReaded == 0) throw;
					clearBuffer();
					return totalReaded;
				}
			}

			template <typename _Derived, typename _Info>
			size_t readwith(OutStream<_Derived, Component, _Info> * dest, const Component &chr) // TooBigException
			{
				size_t sz = readto(dest, chr);
				skip(1);
				return sz;
			}
			TSZ readwith(const Component &chr) // TooBigException
			{
				TSZ tsz;
				readwith(&tsz, chr);
				return tsz;
			}
			size_t skipwith(const Component &chr) // TooBigException
			{
				size_t sz = skipto(chr);
				skip(1);
				return sz;
			}

			template <typename _Derived, typename _Info>
			size_t readwith(OutStream<_Derived, Component, _Info> * dest, Ref needle) // TooBigException
			{
				size_t sz = readto(dest, needle);
				skip(needle.size());
				return sz;
			}
			TSZ readwith(Ref needle) // TooBigException
			{
				TSZ tsz;
				return readwith(&tsz, needle);
			}
			size_t skipwith(Ref needle) // TooBigException
			{
				size_t sz = skipto(needle);
				skip(needle.size());
				return sz;
			}

			template <typename _Derived, typename _Info>
			size_t readto_y(OutStream<_Derived, Component, _Info> * dest, Ref chr)
			{
				return readto_F(dest, [chr](Ref text) {
					return text.find_y(chr).begin();
				});
			}
			TSZ readto_y(Ref chr)
			{
				TSZ tsz;
				readto_y(&tsz, chr);
				return tsz;
			}
			size_t skipto_y(Ref chr)
			{
				return skipto_F([chr](Ref text) {
					return text.find_y(chr).begin();
				});
			}

			template <typename _Derived, typename _Info>
			size_t readto_n(OutStream<_Derived, Component, _Info> * dest, const Component & chr)
			{
				return readto_F(dest, [&](Ref text) {
					return text.find_n(chr).begin();
				});
			}
			TSZ readto_n(const Component &chr)
			{
				TSZ tsz;
				return readto_n(&tsz, chr);
			}
			size_t skipto_n(Component chr)
			{
				return skipto_F([chr](Ref text) {
					return text.find_n(chr).begin();
				});
			}

			template <typename _Derived, typename _Info>
			size_t readto_ny(OutStream<_Derived, Component, _Info> * dest, Ref chr)
			{
				return readto_F(dest, [chr](Ref text) {
					return text.find_ny(chr).begin();
				});
			}
			TSZ readto_ny(Ref chr)
			{
				TSZ tsz;
				readto_ny(&tsz, chr);
				return tsz;
			}
			size_t skipto_ny(Ref chr)
			{
				return skipto_F([chr](Ref text) {
					return text.find_ny(chr).begin();
				});
			}

			RefArray<Component> getBuffer() noexcept
			{
				return RefArray<Component>(m_read, m_filled);
			}
			size_t readImpl(Component * dest, size_t need)
			{
				{
					Component* to = m_read + need;
					if (m_filled >= to)
					{
						mema::copy(dest, m_read, need);
						m_read = to;
						return need;
					}
				}

				size_t left = m_filled - m_read;
				mema::copy(dest, m_read, left);
				dest += left;
				size_t neednext = need - left;
				if (neednext >= BUFFER_SIZE)
				{
					m_filled = m_read = m_buffer;
					size_t readed = base()->read(dest, BUFFER_SIZE);
					return left + readed;
				}

				try
				{
					size_t readed = base()->read(m_buffer, BUFFER_SIZE);
					if (readed >= neednext)
					{
						mema::copy(dest, m_buffer, neednext);
						m_read = m_buffer + neednext;
						m_filled = m_buffer + readed;
						return need;
					}
					else
					{
						mema::copy(dest, m_buffer, readed);
						m_filled = m_read = m_buffer;
						return left + readed;
					}
				}
				catch (EofException&)
				{
					if (left == 0) throw;
					return left;
				}
			}

		private:
			Component m_buffer[BUFFER_SIZE];
			Component* m_read;
			Component* m_filled;
		};
		
		template <typename Base, bool endFlush, bool autoClose, size_t BUFFER_SIZE>
		class BufferedOStream 
			:public FilterOStream<BufferedOStream<Base, endFlush, autoClose, BUFFER_SIZE>, Base, autoClose>
		{
			CLASS_HEADER(BufferedOStream, FilterOStream<BufferedOStream<Base, endFlush, autoClose, BUFFER_SIZE>, Base, autoClose>);
		public:
			INHERIT_COMPONENT();

			using Super::base;

			BufferedOStream(Base* p) noexcept 
				: Super(p)
			{
				m_filled = m_buffer;
			}
			BufferedOStream(nullptr_t) noexcept
				: BufferedOStream((Base*)nullptr)
			{
			}
			BufferedOStream(typename Base::BaseStream * p) noexcept 
				: BufferedOStream(p->template retype<Component>())
			{
			}
			~BufferedOStream() noexcept
			{
				if (endFlush)
					flush();
			}

			void resetStream(nullptr_t) noexcept
			{
				m_filled = m_buffer;
				Super::resetStream(nullptr);
			}
			void resetStream(Base * p) noexcept
			{
				m_filled = m_buffer;
				Super::resetStream(p);
			}
			void resetStream(typename Base::BaseStream * p) noexcept
			{
				m_filled = m_buffer;
				Super::resetStream(p);
			}
			void writeImpl(const Component* data, size_t sz)
			{
				const InternalComponent* src = (InternalComponent*)data;
				size_t left = m_buffer + BUFFER_SIZE - m_filled;
				if (left > sz)
				{
					mema::copy(m_filled, src, sz);
					m_filled += sz;
					return;
				}

				const InternalComponent* srcto = src + sz;
				mema::copy(m_filled, src, left);
				src += left;

				base()->write(m_buffer, BUFFER_SIZE);
				while (src + BUFFER_SIZE <= srcto)
				{
					base()->write(src, BUFFER_SIZE);
					src += BUFFER_SIZE;
				}
				left = srcto - src;
				mema::copy(m_buffer, src, left);
				m_filled = m_buffer + left;
				src = srcto;
			}
			void flush()
			{
				InternalComponent* ptr = (InternalComponent*)m_buffer;
				base()->write((Component*)m_buffer, m_filled - ptr);
				m_filled = m_buffer;
			}

		private:
			InternalComponent m_buffer[BUFFER_SIZE];
			InternalComponent* m_filled;
		};

	}
}
