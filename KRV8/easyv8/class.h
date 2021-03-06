#pragma once

#include <KR3/main.h>
#include <KR3/meta/array.h>

namespace kr
{

	// 자바스크립트 네이티브 클래스
	template <>
	class V8Handle<v8::FunctionTemplate> : public V8Data
	{
		friend V8Object;
		friend V8Function;
		friend JsContext;
		friend NativeObject;
		friend _pri_::InternalTools;
	private:
		typedef NativeObject *(*CTOR)(const JsArguments &);

		inline v8::Handle<v8::FunctionTemplate> _initClass(Text16 _name, int internalFieldCount, CTOR ctor);
		KR_EASYV8_DLLEXPORT void _init(Text16 _name, int internalFieldCount, CTOR ctor);
		KR_EASYV8_DLLEXPORT void _init(Text16 _name, V8Class *parent, CTOR ctor);
		KR_EASYV8_DLLEXPORT V8Class _createChild(Text16 _name, CTOR ctor) const;
		
		inline v8::Persistent<v8::FunctionTemplate>& _handle();
		inline const v8::Persistent<v8::FunctionTemplate>& _handle() const;

	public:
		KR_EASYV8_DLLEXPORT V8Handle();
		KR_EASYV8_DLLEXPORT V8Handle(const V8Handle & _copy);
		KR_EASYV8_DLLEXPORT ~V8Handle();
		template <typename T>
		inline void setClass(Text16 _name, int internalFieldCount);
		template <typename T>
		inline void setClass(Text16 _name, V8Class * parent);
		template <typename T>
		inline V8Class createChild(Text16 _name = nullptr);

		KR_EASYV8_DLLEXPORT V8Function getFunction();
		
		// 클래스 객체의 내부 변수 개수를 정한다.
		KR_EASYV8_DLLEXPORT void setInternalFieldCount(int count);
		KR_EASYV8_DLLEXPORT int getInternalFieldCount();

		// 클래스의 멤버 변수를 설정한다
		// 예외: JsException (이름이 중복될 시 발생)
		KR_EASYV8_DLLEXPORT void set(Text16 _name, JsAny v);

		KR_EASYV8_DLLEXPORT void makeField(Text16 _name, JsFilter _filter);
		KR_EASYV8_DLLEXPORT void makeReadOnlyField(Text16 name, int index);
		KR_EASYV8_DLLEXPORT void setAccessor(Text16 _name, JsAccessor _accessor);
		KR_EASYV8_DLLEXPORT void setReadOnlyAccessor(Text16 _name, JsAccessor _accessor);
		KR_EASYV8_DLLEXPORT void setIndexAccessor(JsIndexAccessor _accessor);
		KR_EASYV8_DLLEXPORT void setReadOnlyIndexAccessor(JsIndexAccessor _accessor);

		// 클래스의 멤버 변수를 설정한다
		// 예외: JsException (이름이 중복될 시 발생)
		KR_EASYV8_DLLEXPORT void setStatic(Text16 _name, JsAny v);

		KR_EASYV8_DLLEXPORT void setStaticAccessor(Text16 _name, JsAccessor _accessor);
		KR_EASYV8_DLLEXPORT void setStaticReadOnlyAccessor(Text16 _name, JsAccessor _accessor);
		
		template <typename P>
		void setStaticAccessor(Text16 _name, P * value)
		{
			setStaticAccessor(_name,
				[value](V8Object)->JsAny { return *value; },
				[value](V8Object, JsAny nv) { *value = nv.get<P>(); }
			);
		}

		template <typename P>
		void setStaticReadOnlyAccessor(Text16 _name, P * value)
		{
			setStaticReadOnlyAccessor(_name,
				[value](V8Object)->JsAny { return *value; }
			);
		}

		template <typename LAMBDA>
		void setMethod(Text16 _name, LAMBDA _lambda)
		{
			set(_name, JsFunction(_lambda));
		}

		template <typename GET, typename SET>
		void setAccessor(Text16 _name, GET get, SET set)
		{
			setAccessor(_name, JsAccessor(get, set));
		}

		template <typename GET>
		void setReadOnlyAccessor(Text16 _name, GET get)
		{
			setReadOnlyAccessor(_name, JsAccessor(get));
		}

		template <typename GET, typename SET>
		void setStaticAccessor(Text16 _name, GET get, SET set)
		{
			setStaticAccessor(_name, JsAccessor(get, set));
		}

		template <typename GET>
		void setStaticReadOnlyAccessor(Text16 _name, GET get)
		{
			setStaticReadOnlyAccessor(_name, JsAccessor(get));
		}

		// 객체를 생성합니다
		// 예외: JsException
		KR_EASYV8_DLLEXPORT V8Object newInstanceRaw(JsArgumentsIn args) const;

		// 객체를 생성합니다
		// 기본적으로 Weak 상태로 생성되어, GC에 의하여 지워질 수 있습니다
		// 예외: JsException
		KR_EASYV8_DLLEXPORT NativeObject* newInstanceRawPtr(JsArgumentsIn args) const;

		template <typename LAMBDA>
		inline void setStaticMethodRaw(Text16 _name, LAMBDA _lambda);

		template <typename T, typename P>
		inline void setMethodAccessor(Text16 _name, P(T::*get)());
		template <typename T, typename P>
		inline void setMethodAccessor(Text16 _name, P(T::*get)(), void(T::*set)(P));

		template <typename T, typename RET, typename ... ARGS>
		inline void setMethod(Text16 _name, RET(T::* func)(ARGS ...));

		template <typename LAMBDA>
		inline void setStaticMethod(Text16 _name, LAMBDA _lambda);

	};

	// 자바스크립트 네이티브 클래스
	// C++ 클래스로 부터 생성
	// C++ 클래스는 initMethods 함수를 구현해야한다.
	template <typename T> class JsClass :public V8Class
	{
	public:
		inline JsClass(Text16 _name, int internalFieldCount);
		inline JsClass(Text16 _name, V8Class * parent);

		inline V8Class createChild();

		template <typename P>
		inline void setAccessor(Text16 _name, P(T::*v));

		template <typename P>
		inline void setReadOnlyAccessor(Text16 _name, P(T::*v));

		template <typename P>
		inline void setMethodAccessor(Text16 _name, P(T::*get)());

		template <typename P>
		inline void setMethodAccessor(Text16 _name, P(T::*get)(), void(T::*set)(P));

		template <typename P>
		inline void setNullableMethodAccessor(Text16 _name, P(T::*get)());

		template <typename P>
		inline void setNullableMethodAccessor(Text16 _name, P(T::*get)(), void(T::*set)(P));

		inline void setMethodRaw(Text16 _name, JsAny(T::* func)(const JsArguments &));
		
		template <typename P>
		inline void setIndexAccessor(P(T::* get)(uint32_t));

		template <typename P>
		inline void setIndexAccessor(P(T::* get)(uint32_t), void(T::* set)(uint32_t, P));

		template <typename RET, typename ... ARGS>
		inline void setMethod(Text16 _name, RET(T::* func)(ARGS ...));
		
		// 객체를 생성합니다
		// 기본적으로 Weak 상태로 생성되어, GC에 의하여 지워질 수 있습니다.
		template <typename ... ARGS> T* newInstance(const ARGS & ... args) const;
	};
	
}