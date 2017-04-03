#pragma once

#include "../container.h"

namespace kr
{
	namespace _pri_
	{
		template <class Derived, typename C, class Info>
		class IStream_cmpAccessable<Derived, C, StreamInfo<false, Info>>
			: public Container<C, true, StreamInfo<false, Info>>
		{
			CLASS_HEADER(IStream_cmpAccessable, Container<C, true, StreamInfo<false, Info>>);
		public:
			INHERIT_COMPONENT();
			using Super::Super;
			using TSZ = TempSzText<C>;

			inline size_t read(Component * dest, size_t sz)
			{
				return static_cast<Derived*>(this)->readImpl(dest, sz);
			}
			inline InternalComponent read()
			{
				InternalComponent out;
				read((Component*)&out, 1);
				return out;
			}
			inline size_t skip(size_t sz)
			{
				return static_cast<Derived*>(this)->skipImpl(sz);
			}
			template <class _Derived, class _Parent>
			inline void readAll(OutStream<_Derived, Component, StreamInfo<true, _Parent>> * os) noexcept
			{
				try
				{
					for (;;)
					{
						static_cast<Derived*>(this)->read(os, 4096);
					}
				}
				catch (EofException&)
				{
				}
			}
			inline Alc readAll() noexcept
			{
				Alc out;
				readAll(&out);
				return out;
			}
			inline TSZ read(size_t size)
			{
				TSZ tsz;
				static_cast<Derived*>(this)->read(&tsz, size);
				return tsz;
			}

			template <typename T> T readas() // EofException
			{
				T value;
				size_t dwLen = read(&value, sizeof(T));
				if (dwLen != sizeof(T)) throw EofException();
				return move(value);
			}
		};

		template <class Derived, typename C, class Info>
		class IStream_cmpAccessable<Derived, C, StreamInfo<true, Info>> :
			public Bufferable<Derived, BufferInfo<C, true, false, !Info::writable, Container<C, true, StreamInfo<true, Info>>>>
		{
			CLASS_HEADER(IStream_cmpAccessable, Bufferable<Derived, BufferInfo<C, true, false, !Info::writable, Container<C, true, StreamInfo<true, Info>>>>);
		public:
			INHERIT_COMPONENT();
			using Super::Super;
			using Super::size;
			using Super::begin;
			using Super::end;
			using Super::find;

		private:
			inline Derived* derived() noexcept
			{
				return static_cast<Derived*>(this);
			}

		public:
			inline ComponentRef * read(size_t * psize) // EofException
			{
				return derived()->readImpl(psize);
			}
			inline size_t skip(size_t sz) // EofException
			{
				read(&sz);
				return sz;
			}
			inline size_t read(Component * dest, size_t sz) // EofException
			{
				const Component * src = read(&sz);
				mema::subs_copy((InternalComponent*)dest, (InternalComponent*)src, sz);
				return sz;
			}

			// 예외 검사 없이, 강제로 읽는다, 
			inline InternalComponent readForce() noexcept
			{
				const InternalComponent * p = (InternalComponent*)begin();
				derived()->addBegin(1);
				return *p;
			}
			inline InternalComponent read() // EofException
			{
				if (size() == 0)
					throw EofException();
				const InternalComponent * p = (InternalComponent*)begin();
				derived()->addBegin(1);
				return *p;
			}
			inline Ref read(size_t _len) // EofException
			{
				if (size() == 0) throw EofException();
				_len = math::min(_len, size());
				Ref out(begin(), _len);
				derived()->addBegin(_len);
				return out;
			}
			template <typename T>
			inline T readas() // EofException
			{
				Ref ref = read((sizeof(T) + sizeof(Component) - 1) / sizeof(Component));
				return *(T*)ref.begin();
			}
			inline Ref readto(Ref _idx) noexcept
			{
				const Component * b = begin();
				const Component * e = end();

				const Component * p = _idx.begin();
				_assert(b <= p && p <= e);
				Ref out(b, p);
				derived()->setBegin(p);
				return out;
			}
			inline Ref readto(Ref _idx, size_t skip) noexcept
			{
				const Component * p = _idx.begin();
				const Component * b = begin();
				const Component * e = end();
				_assert(b <= p && p + skip <= e);
				Ref out(b, p);
				derived()->setBegin(p + skip);
				return out;
			}
			inline Ref readto_p(Ref _idx) noexcept
			{
				return _idx == nullptr ? (Ref)nullptr : readto(_idx);
			}
			inline Ref readto_p(Ref _idx, size_t _skip) noexcept
			{
				return _idx == nullptr ? (Ref)nullptr : readto(_idx, _skip);
			}
			inline Ref readto_e(Ref _idx) noexcept
			{
				return _idx == nullptr ? readAll() : readto(_idx);
			}
			inline Ref readto_e(Ref _idx, size_t _skip) noexcept
			{
				return _idx == nullptr ? readAll() : readto(_idx, _skip);
			}
			template <typename LAMBDA>
			inline Ref readto_L(const LAMBDA &lambda)
			{
				return readto_p(find_L(move(lambda)));
			}
			template <typename LAMBDA>
			inline Ref readto_eL(const LAMBDA &lambda)
			{
				return readto_e(find_L(lambda));
			}
			inline Ref readwith(const InternalComponent &_cut) noexcept
			{
				return readto_p(find(_cut), 1);
			}
			inline Ref readwith_e(const InternalComponent &_cut) noexcept
			{
				return readto_e(find(_cut), 1);
			}
			inline Ref readwith(Ref _cut) noexcept
			{
				return readto_p(find(_cut), _cut.size());
			}
			inline Ref readwith_e(Ref _cut) noexcept
			{
				return readto_e(find(_cut), _cut.size());
			}
			inline Ref readwith_y(Ref _cut) noexcept
			{
				return readto_p(find_y(_cut), 1);
			}
			inline Ref readwith_ye(Ref _cut) noexcept
			{
				return readto_e(find_y(_cut), 1);
			}
			inline Ref readwith_n(const InternalComponent &_cut) // EofException
			{
				return readto_p(find_n(_cut));
			}
			inline Ref readwith_ny(Ref _cut) // EofException
			{
				return readto_p(find_ny(_cut));
			}
			template <typename LAMBDA>
			inline Ref readwith_L(const LAMBDA & lambda)
			{
				return readto_p(find_L(move(lambda)), 1);
			}
			template <typename LAMBDA>
			inline Ref readwith_eL(const LAMBDA & lambda)
			{
				return readto_e(find_L(move(lambda)), 1);
			}
			template <class _Derived, class _Parent>
			inline void readAll(OutStream<_Derived, Component, StreamInfo<true, _Parent>> * os) noexcept
			{
				static_cast<Derived*>(this)->read(os, size());
			}
			inline Ref readAll() noexcept
			{
				Ref out(begin(), end());
				derived()->setBegin(end());
				return out;
			}
			inline size_t readwith_a(Alc *_v, const InternalComponent &_cut) noexcept
			{
				return _v->alloc(readwith(_cut));
			}

			inline size_t readwith_ay(Alc *_v, Ref _cut) noexcept
			{
				return _v->alloc(readwith(_cut));
			}
			inline size_t read_str(Wri& _v) noexcept
			{
				size_t _len = _v.copy(*this);
				derived()->addBegin(_len);
				return _len;
			}
			inline size_t readwith_str(Wri& _v, const InternalComponent &_cut) noexcept
			{
				return _v.copy(readwith(_cut));
			}
			inline size_t readwith_str_y(Wri& _v, Ref _cut) noexcept
			{
				return _v.copy(readwith(_cut));
			}

			inline int read_enumchar(Ref list) // EofException
			{
				if (size() == 0)
					throw EofException();
				size_t i = memm::pos(list.begin(), *begin(), list.size());
				if (i != -1)
					derived()->addBegin(1);
				return i;
			}

		};
	}

	template <class Derived, typename Component, typename Info>
	class InStream
		: public _pri_::IStream_cmpAccessable<Derived, Component, Info>
	{
		CLASS_HEADER(InStream, _pri_::IStream_cmpAccessable<Derived, Component, Info>);
	public:
		INHERIT_COMPONENT();
		using Super::Super;
		using Super::read;

		template <class _Derived, class _Parent>
		inline size_t read(OutStream<_Derived, Component, StreamInfo<true, _Parent>> * os, size_t size)
		{
			size_t sz = read(os->padding(size), size);
			os->commit(sz);
			return sz;
		}
	};
}
