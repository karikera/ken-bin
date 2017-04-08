#pragma once

#include <KR3/common/type.h>
#include <KR3/common/autovar/autoptr.h>

#ifndef NDEBUG
#define KR_MEMORY_OBSERVER
#endif

#ifdef KR_MEMORY_OBSERVER
#define _new new(__FILE__ , __LINE__)

void * CT_CDECL operator new(size_t size, const char * file, int line) noexcept;
void * CT_CDECL operator new[](size_t size, const char * file, int line) noexcept;
void CT_CDECL operator delete(void * data, const char * file, int line) noexcept;
void CT_CDECL operator delete[](void * data, const char * file, int line) noexcept;

inline void * CT_FASTCALL getAllocatedPointer(void * value) noexcept
{
	return value;
}

namespace kr
{
	namespace _pri_
	{
		void CT_FASTCALL reline_new_impl(void * ptr, const char * file, int line) noexcept;
		void CT_FASTCALL depend_new_impl(void * parent, void * child) noexcept;
		template <typename T>
		T& reline_new_pass_impl(T& ptr, const char * file, int line) noexcept
		{
			reline_new_impl(ptr, file, line);
			return ptr;
		}
		template <typename T>
		T* reline_new_pass_impl(T* ptr, const char * file, int line) noexcept
		{
			reline_new_impl(ptr, file, line);
			return ptr;
		}
	}
}
#define reline_new(ptr) (::kr::_pri_::reline_new_pass_impl(ptr, __FILE__, __LINE__))
#define depend_new(parent, child) (::kr::_pri_::depend_new_impl(\
		getAllocatedPointer(parent), \
		getAllocatedPointer(child)))

#else

#define _new new
#define _quiet_new new
#define reline_new(ptr)	(ptr)
#define depend_new(parent, child) do {} while(0,0)

#endif

void * CT_CDECL operator new(size_t size);
void * CT_CDECL operator new[](size_t size);
void CT_CDECL operator delete(void * data) noexcept;
void CT_CDECL operator delete[](void * data) noexcept;

namespace kr
{
	namespace _pri_
	{
		ATTR_CHECK_RETURN bool CT_FASTCALL expand_impl(void * data, size_t nsize) noexcept;
		void CT_FASTCALL free_impl(void * data) noexcept;
		ATTR_CHECK_RETURN size_t CT_FASTCALL msize_impl(const void * ptr) noexcept;

		ATTR_CHECK_RETURN void* CT_FASTCALL aligned_alloc_impl(size_t sz, size_t aligned) noexcept;
		ATTR_CHECK_RETURN void* CT_FASTCALL aligned_alloc_impl(size_t sz, size_t aligned, size_t offset) noexcept;
		ATTR_CHECK_RETURN bool CT_FASTCALL aligned_expand_impl(void * data, size_t nsize, size_t aligned, size_t offset = 0) noexcept;
		ATTR_CHECK_RETURN size_t CT_FASTCALL aligned_msize_impl(const void * ptr, size_t aligned, size_t offset = 0) noexcept;
		void CT_FASTCALL aligned_free_impl(void * ptr) noexcept;
		
		template <size_t alignment, size_t offset>
		struct alloc_base
		{
			ATTR_CHECK_RETURN static void* CT_FASTCALL allocate(size_t sz) noexcept
			{
				if (offset == 0) return aligned_alloc_impl(sz, alignment);
				else return aligned_alloc_impl(sz, alignment, offset);
			}
			ATTR_CHECK_RETURN static void CT_FASTCALL free(void * p) noexcept
			{
				return aligned_free_impl(p);
			}
			ATTR_CHECK_RETURN static bool CT_FASTCALL expand(void * p, size_t sz) noexcept
			{
				return aligned_expand_impl(p, sz, alignment, offset);
			}
			ATTR_CHECK_RETURN static size_t CT_FASTCALL msize(const void * data) noexcept
			{
				return aligned_msize_impl(p, alignment, offset);
			}
		};
		template <>
		struct alloc_base<0, 0>
		{
			ATTR_CHECK_RETURN static void* CT_FASTCALL allocate(size_t sz) noexcept;
			ATTR_CHECK_RETURN static void CT_FASTCALL free(void * data) noexcept;
			ATTR_CHECK_RETURN static bool CT_FASTCALL expand(void * data, size_t nsize) noexcept;
			ATTR_CHECK_RETURN static size_t CT_FASTCALL msize(const void * data) noexcept;
		};
	}

	template <size_t alignment = 0, size_t offset = 0>
	struct alloc : _pri_::alloc_base <
		offset % alignment == 0 && alignment <= alignof(max_align_t) ? 0 : alignment,
		offset % alignment >
	{
		static_assert(((alignment - 1) & alignment) == 0, "Alignment must power of 2");
	};

	// 메모리 테스트의 지점을 정한다.
	// 디버그 모드에서만 동작한다.
	void CT_FASTCALL memtest_set_axis() noexcept;

	// 정한 지점만큼 메모리가 돌아와있는지 테스트 한다.
	// 디버그 모드에서만 동작한다.
	void CT_FASTCALL memtest() noexcept;

	// 할당된 메모리를 전부 지운다.
	// 디버그 모드에서만 동작한다.
	// 특수한 경우 외에는 사용하지 말 것.
	void CT_FASTCALL memdestroy() noexcept;

	// 전체 메모리를 오염되었는지 확인한다.
	void CT_FASTCALL memcheck() noexcept;

	struct MemoryTest
	{
		inline MemoryTest() noexcept{ memtest_set_axis(); }
		inline ~MemoryTest() noexcept{ memtest(); }
	};

	template <typename T, typename ... ARGS> T * newAligned(const ARGS &... args)
	{
		T* data = (T*)alloc<alignof(T)>::allocate(sizeof(T));
		return new(data) T(args...);
	}
	template <typename T, typename ... ARGS> T * newAlignedArray(size_t sz)
	{
		if (std::is_class<T>::value)
		{
			static_assert(sizeof(T) % alignof(T) == 0, "sizeof & alignment unmatch!!");
			size_t * cnt = (size_t*)alloc<alignof(T), sizeof(size_t)>::allocate(sizeof(T) * sz);
			*cnt = sz;
			T * data = (T*)(cnt + 1);
			T * end = data + sz;
			for (T * beg = data; beg != end; beg++)
			{
				new (beg) T;
			}
			return data;
		}
		else
		{
			return (T*)alloc<alignof(T)>::allocate(sizeof(T) * sz);
		}
	}
	template <typename T, typename ... ARGS> T * newAlignedArrayNC(size_t sz)
	{
		static_assert(sizeof(T) % alignof(T) == 0, "sizeof & alignment unmatch!!");
		return (T*)alloc<alignof(T)>::allocate(sizeof(T) * sz);
	}
	template <typename T> void deleteAligned(T * ptr) noexcept
	{
		if (ptr == nullptr) return;
		ptr->~T();
		alloc<alignof(T)>::free(ptr);
	}
	template <typename T> void deleteAlignedND(T * ptr) noexcept
	{
		if (ptr == nullptr) return;
		return alloc<alignof(T)>::free(ptr);
	}
	template <typename T> void deleteAlignedArray(T * ptr) noexcept
	{
		if (ptr == nullptr) return;
		if (std::is_class<T>::value)
		{
			size_t * cnt = (size_t*)ptr - 1;
			
			T * end = ptr + *cnt;
			for (; ptr != end; ptr++)
			{
				ptr->~T();
			}
			alloc<alignof(T), sizeof(size_t)>::free(cnt);
		}
		else
		{
			alloc<alignof(T)>::free(ptr);
		}
	}
}


#ifdef KR_MEMORY_OBSERVER
#define kr_alloc(sz, ...)			((::kr::autoptr)reline_new(::kr::alloc<__VA_ARGS__>::allocate(sz)))
#else
#define kr_alloc(sz, ...)			((::kr::autoptr)::kr::alloc<__VA_ARGS__>::allocate(sz)))
#endif
#define kr_free(ptr, ...)			(::kr::alloc<__VA_ARGS__>::free(ptr))
#define kr_expand(ptr, sz, ...)		(::kr::alloc<__VA_ARGS__>::expand(ptr,sz))
#define kr_msize(ptr, ...)			(::kr::alloc<__VA_ARGS__>::msize(ptr))

#define _newAligned(T, ...)			new((T*)::kr::alloc<alignof(T)>::allocate(sizeof(T))) T(__VA_ARGS__)
#define _newAlignedArray(T, cnt)	reline_new(::kr::_pri_::newAlignedArray<T>(cnt))
#define _newAlignedArrayNC(T, cnt)	reline_new(::kr::_pri_::newAlignedArrayNC<T>(cnt))