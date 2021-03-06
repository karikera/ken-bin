#pragma once

#include "../container.h"
#include "membuffer.h"
#include <string>

namespace kr
{
	namespace buffer
	{
		namespace _pri_
		{
			template <typename Component>
			struct StaticText
			{
				static const Component WHITE_SPACE[5];
			};
			template <typename Component>
			const Component StaticText<Component>::WHITE_SPACE[5] = {
				(Component)' ',
				(Component)'\t',
				(Component)'\r',
				(Component)'\n',
				(Component)'\0',
			};
		}

		template <class Derived, class Info, class Next>
		class TextBuffer : public Next
		{
			CLASS_HEADER(TextBuffer, Next);
		public:
			INHERIT_COMPONENT();

			using Super::Super;
			using Super::data;
			using Super::begin;
			using Super::end;
			using Super::size;
			using Super::find_ny;
			using Super::find_nry;

			static const typename Info::Component (&WHITE_SPACE)[5];

			friend std::basic_ostream<Component>& operator <<(std::basic_ostream<Component>& os, const TextBuffer& method)
			{
				os.write(method.begin(),method.size());
				return os;
			}

			uint64_t to_uint64_x(uint _radix, size_t _len) const noexcept
			{
				KR_DEFINE_MMEM();
				return memm::template toint<uint64_t>(begin(),_len,_radix);
			}
			int64_t to_int64_x(uint _radix, size_t _len) const noexcept
			{
				KR_DEFINE_MMEM();
				return memm::template toint<int64_t>(begin(), _len, _radix);
			}
			uint32_t to_uint_x(uint _radix, size_t _len) const noexcept
			{
				KR_DEFINE_MMEM();
				return memm::template toint<uint32_t>(begin(),_len,_radix);
			}
			int32_t to_int_x(uint _radix, size_t _len) const noexcept
			{
				KR_DEFINE_MMEM();
				return memm::template toint<int32_t>(begin(), _len, _radix);
			}
			uintptr_t to_uintp_x(uint _radix, size_t _len) const noexcept
			{
				KR_DEFINE_MMEM();
				return memm::template toint<uintptr_t>(begin(), _len, _radix);
			}
			intptr_t to_intp_x(uint _radix, size_t _len) const noexcept
			{
				KR_DEFINE_MMEM();
				return memm::template toint<intptr_t>(begin(), _len, _radix);
			}
			float to_ufloat_x(size_t _len) const noexcept
			{
				KR_DEFINE_MMEM();
				const ComponentRef * beg = begin();
				const ComponentRef * finded = memm::find(beg, '.', _len);
				if (finded == nullptr) return (float)to_uint_x(10, _len);
				size_t len2 = finded - beg;
				_len = _len - len2 - 1;
				float res = (float)to_uint_x(10, len2);
				res += (float)(memm::template toint<dword>(finded + 1, _len, 10)) / math::pow((size_t)10, _len);
				return res;
			}
			float to_float_x(size_t _len) const noexcept
			{
				if (!_len) return 0;
				if (*begin() == (Component)'-')
					return -((*this + 1).to_ufloat_x(_len - 1));
				else return to_ufloat_x(_len);
			}
			bool numberonly_x(size_t _len) const noexcept
			{
				KR_DEFINE_MMEM();
				return (memm::numberonly(begin(), _len));
			}

			uint64_t to_uint64_limit_x(uint _radix, size_t _len) const throws(OutOfRangeException)
			{
				KR_DEFINE_MMEM();
				return memm::template toint_limit<uint64_t>(data(), _len, _radix);
			}
			int64_t to_int64_limit_x(uint _radix, size_t _len) const throws(OutOfRangeException)
			{
				KR_DEFINE_MMEM();
				return memm::template toint_limit<int64_t>(data(), _len, _radix);
			}
			uint32_t to_uint_limit_x(uint _radix, size_t _len) const throws(OutOfRangeException)
			{
				KR_DEFINE_MMEM();
				return memm::template toint_limit<uint32_t>(data(), _len, _radix);
			}
			int32_t to_int_limit_x(uint _radix, size_t _len) const throws(OutOfRangeException)
			{
				KR_DEFINE_MMEM();
				return memm::template toint_limit<int32_t>(data(), _len, _radix);
			}
			uintptr_t to_uintp_limit_x(uint _radix, size_t _len) const throws(OutOfRangeException)
			{
				KR_DEFINE_MMEM();
				return memm::template toint_limit<uintptr_t>(data(), _len, _radix);
			}
			intptr_t to_intp_limit_x(uint _radix, size_t _len) const throws(OutOfRangeException)
			{
				KR_DEFINE_MMEM();
				return memm::template toint_limit<intptr_t>(data(), _len, _radix);
			}

			uint64_t to_uint64(uint _radix = 10) const noexcept
			{
				return to_uint64_x(_radix, size());
			}
			int64_t to_int64(uint _radix = 10) const noexcept
			{
				return to_int64_x(_radix, size());
			}
			uint32_t to_uint(uint _radix = 10) const noexcept
			{
				return to_uint_x(_radix, size());
			}
			int32_t to_int(uint _radix = 10) const noexcept
			{
				return to_int_x(_radix, size());
			}
			uintptr_t to_uintp(uint _radix = 10) const noexcept
			{
				return to_uintp_x(_radix, size());
			}
			intptr_t to_intp(uint _radix = 10) const noexcept
			{
				return to_intp_x(_radix, size());
			}
			float to_ufloat() const noexcept
			{
				return to_ufloat_x(size());
			}
			float to_float() const noexcept
			{
				return to_float_x(size());
			}

			uint64_t to_uint64_limit(uint _radix = 10) const
			{
				return to_uint64_limit_x(_radix, size());
			}
			int64_t to_int64_limit(uint _radix = 10) const
			{
				return to_uint64_limit_x(_radix, size());
			}
			uint32_t to_uint_limit(uint _radix = 10) const
			{
				return to_uint_limit_x(_radix, size());
			}
			int32_t to_int_limit(uint _radix = 10) const
			{
				return to_int_limit_x(_radix, size());
			}
			uintptr_t to_uintp_limit(uint _radix = 10) const
			{
				return to_uintp_limit_x(_radix, size());
			}
			intptr_t to_intp_limit(uint _radix = 10) const
			{
				return to_intp_limit_x(_radix, size());
			}

			uint64_t to_qword_l(size_t _len, uint _radix) const noexcept
			{
				return to_uint64_x(_radix, math::min(_len, size()));
			}
			int64_t to_llong_l(size_t _len, uint _radix) const noexcept
			{
				return to_int64_x(_radix, math::min(_len, size()));
			}
			uint32_t to_uint_l(size_t _len, uint _radix) const noexcept
			{
				return to_uint_x(_radix,math::min(_len, size()));
			}
			int32_t to_int_l(size_t _len, uint _radix) const noexcept
			{
				return to_int_x(_radix,math::min(_len, size()));
			}
			uintptr_t to_uintp_l(size_t _len, uint _radix) const noexcept
			{
				return to_uintp_x(_radix, math::min(_len, size()));
			}
			intptr_t to_intp_l(size_t _len, uint _radix) const noexcept
			{
				return to_intp_x(_radix, math::min(_len, size()));
			}
			float to_ufloat_l(size_t _len) const noexcept
			{
				return to_ufloat_x(math::min(_len, size()));
			}
			float to_float_l(size_t _len) const noexcept
			{
				return to_float_x(math::min(_len, size()));
			}
			bool numberonly() const noexcept
			{
				return numberonly_x(size());
			}

			intptr_t compare(Ref _str) const noexcept
			{
				KR_DEFINE_MMEM();
				size_t len1 = size();
				size_t len2 = _str.size();

				int order = memm::compare(begin(), _str.begin(), math::min(len1, len2));
				if (order == 0) return len1 - len2;
				return order;
			}
			bool equals_i(Ref _v) const noexcept
			{
				return equals_ix(_v, size());
			}
			bool equals_ix(Ref _v, size_t _len) const noexcept
			{
				KR_DEFINE_MMEM();
				size_t len2 = _v.size();
				if (_len != len2) return false;
				else return memm::equals_i(begin(), _v.begin(), _len);
			}

			Ref trim() const noexcept
			{
				Ref beg = find_ny(WHITE_SPACE);
				if (beg == nullptr)
					return Ref(end(), end());
				Ref end = find_nry(WHITE_SPACE);
				return beg.cut(end+1);
			}

			constexpr TextBuffer() noexcept = default;
			constexpr TextBuffer(const TextBuffer&) = default;
			constexpr TextBuffer(TextBuffer&&) = default;
			explicit TextBuffer(const Component* str) noexcept
				:Super(str, memt<sizeof(InternalComponent)>::pos(str, 0))
			{
			}

			explicit operator std::basic_string<InternalComponent>() noexcept
			{
				return std::basic_string<InternalComponent>(begin(), size());
			}
		};

		template <class Derived, class Info, class Next>
		class WTextBuffer:public TextBuffer<Derived, Info, Next>
		{
			CLASS_HEADER(WTextBuffer, TextBuffer<Derived, Info, Next>);
		public:
			INHERIT_COMPONENT();

			using Super::Super;
			using Super::begin;
			using Super::end;
			using Super::size;

			void toLowerCase() noexcept
			{
				static_assert(std::is_trivially_default_constructible<Component>::value, "Need to use non class component");
				KR_DEFINE_MMEM();
				memm::tolower(begin(), size());
			}
			void toUpperCase() noexcept
			{
				static_assert(std::is_trivially_default_constructible<Component>::value, "Need to use non class component");
				KR_DEFINE_MMEM();
				memm::toupper(begin(), size());
			}
		};

		template <class Derived, class Info, typename Next>
		const typename Info::Component (&TextBuffer<Derived, Info, Next>::WHITE_SPACE)[5] = _pri_::StaticText<typename Info::Component>::WHITE_SPACE;
	}
}