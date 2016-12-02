#pragma once

namespace kr
{
	namespace ary
	{
		namespace method
		{
			template <class Parent> class BufferIMethod : 
				public Bufferable<Parent, BufferInfo<typename Parent::Component, true, false, !Parent::writable, Parent>>
			{
				CLASS_HEADER(BufferIMethod, Bufferable<Parent, BufferInfo<typename Parent::Component, true, false, !Parent::writable, Parent>>);
			public:
				INHERIT_ARRAY();

				using Super::Super;
				using Super::size;
				using Super::begin;
				using Super::end;
				using Super::empty;

				constexpr BufferIMethod() noexcept = default;
				constexpr BufferIMethod(const BufferIMethod&) = default;
				constexpr BufferIMethod(BufferIMethod&&) = default;

				template <class _Derived, bool a, bool b, class _Parent>
				BufferIMethod(Bufferable<_Derived, BufferInfo<Component, true, a, b, _Parent>> & data) noexcept
					:Super(data.begin(), data.end())
				{
				}
				template <class _Derived, bool a, bool b, class _Parent>
				BufferIMethod(const Bufferable<_Derived, BufferInfo<Component, true, a, b, _Parent>> & data) noexcept
					:Super(data.begin(), data.end())
				{
				}
				template <typename _Traits, typename _Alloc>
				BufferIMethod(std::basic_string<Component, _Traits, _Alloc> & str) noexcept
					: Super(str.data(), str.size())
				{
				}
				template <typename _Traits, typename _Alloc>
				BufferIMethod(const std::basic_string<Component, _Traits, _Alloc> & str) noexcept
					:Super(str.data(), str.size())
				{
				}

				Ref operator +(intp n) const noexcept
				{
					return Ref(begin() + n, end());
				}
				Ref operator -(intp n) const noexcept
				{
					return Ref(begin() - n, end());
				}
				intp operator -(const Component * ptr) const noexcept
				{
					return begin() - ptr;
				}
				friend intp operator -(const Component * ptr, const BufferIMethod& ori) noexcept
				{
					return ptr - ori.begin();
				}
				template <typename T> intp operator -(const BufferIMethod<T>& ptr) const noexcept
				{
					return begin() - ptr.begin();
				}
			};
		}
	}
}
