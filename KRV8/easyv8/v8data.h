#pragma once

namespace kr
{
	template <>
	class V8Handle<v8::Data>
	{
		friend V8Function;
		friend _pri_::InternalTools;
	public:
		CBS_EASYV8_DLLEXPORT V8Handle();

		CBS_EASYV8_DLLEXPORT ~V8Handle();

		CBS_EASYV8_DLLEXPORT V8Handle(const V8Handle & value);

		CBS_EASYV8_DLLEXPORT V8Handle(V8Handle && value);

		// ��ü�� ���ϴ�.
		CBS_EASYV8_DLLEXPORT void CT_FASTCALL setEmpty();

		// �� ��ü�� �ִ��� Ȯ���մϴ�.
		CBS_EASYV8_DLLEXPORT bool CT_FASTCALL isEmpty() const;

		// �� ������ �����־, ��ü�� GC�� ���Ͽ� ������ �� �ְ� ����ϴ�.
		CBS_EASYV8_DLLEXPORT void CT_FASTCALL setWeak();

		template <typename LAMBDA>
		void setWeak(LAMBDA lambda);

		// setWeak�� ���Ͽ�, GC�� ���ؼ� ������ �� �ְ� �� ���¸� �������� �ٲߴϴ�.
		CBS_EASYV8_DLLEXPORT void CT_FASTCALL clearWeak();

		// �ٸ� ��ü�� ���� �񱳸� �մϴ�.
		CBS_EASYV8_DLLEXPORT bool CT_FASTCALL equals(const V8Handle& o) const;

		inline V8Handle & operator =(const V8Handle & value);
		inline bool operator ==(const V8Handle & o) const;
		inline bool operator !=(const V8Handle & o) const;
	protected:
		v8::Persistent<v8::Data> m_value;

	private:
		// �� ������ �����־, ��ü�� GC�� ���Ͽ� ������ �� �ְ� ����ϴ�.
		// ��ü�� ������ �� callback �� ȣ��˴ϴ�.
		CBS_EASYV8_DLLEXPORT void CT_FASTCALL _setWeak(JsLambda callback);
	};

	template <>
	class V8Handle<v8::Private> :public V8Data
	{
		friend V8Object;
		friend _pri_::InternalTools;
	public:
		CBS_EASYV8_DLLEXPORT V8Handle();
		CBS_EASYV8_DLLEXPORT ~V8Handle();
		inline V8Handle(nullptr_t)
			:V8Data()
		{
		}
		inline V8Handle(const V8Handle & value)
			:V8Data(value)
		{
		}


	private:
		inline v8::Persistent<v8::Private>& _handle();
		inline const v8::Persistent<v8::Private>& _handle() const;
	};
	template <>
	class V8Handle<v8::Value>:public V8Data
	{
		friend _pri_::InternalTools;
		friend JsString;
	public:
		// ���� Ÿ�� Ȯ��
		template <typename T> bool is() const;

		// ��ü���� Ȯ��
		template <> CBS_EASYV8_DLLEXPORT bool CT_FASTCALL is<v8::Object>() const;

		// �Լ����� Ȯ��
		template <> CBS_EASYV8_DLLEXPORT bool CT_FASTCALL is<v8::Function>() const;

		// �迭���� Ȯ��
		template <> CBS_EASYV8_DLLEXPORT bool CT_FASTCALL is<v8::Array>() const;

		// ��Ʈ������ �޾ƿ���
		CBS_EASYV8_DLLEXPORT JsString CT_FASTCALL toString() const;

	protected:
		template <typename T>
		inline void _set(const V8Handle & any);
	};

	// V8 ���� ��ü�� ����
	// V8 ��ü
	template <>
	class V8Handle<v8::Object> : public V8Handle<v8::Value>
	{
		friend _pri_::InternalTools;
		friend V8Function;
		friend JsAny;
		friend V8Array;
		friend V8Class;
		friend JsContext;
		friend NativeObject;

	public:
		inline V8Handle();
		inline V8Handle(nullptr_t);
		inline V8Handle(const V8Handle &obj);
		
		inline explicit V8Handle(const V8Value & any);

		// ��ü �Ҵ�
		CBS_EASYV8_DLLEXPORT static V8Handle<v8::Object> CT_FASTCALL newInstance();

		// ��ü�� Ŭ������ ����ֽ��ϴ�
		// ����: JsException
		CBS_EASYV8_DLLEXPORT void CT_FASTCALL setClass(Text16 name, V8Handle<v8::FunctionTemplate> * value);

		// ��ü�� ������ ����ֽ��ϴ�
		CBS_EASYV8_DLLEXPORT void CT_FASTCALL set(Text16 name, JsAny value);

		// ��ü�� ������ �����ɴϴ�
		CBS_EASYV8_DLLEXPORT JsAny CT_FASTCALL get(Text16 name) const;

		// ��ü���� ������ ����ϴ�.
		CBS_EASYV8_DLLEXPORT bool CT_FASTCALL remove(Text16 name) const;

		// ���� ������ ����ֽ��ϴ�
		CBS_EASYV8_DLLEXPORT void CT_FASTCALL set(const V8Private & p, JsAny value);

		// ���� ������ �����ɴϴ�
		CBS_EASYV8_DLLEXPORT JsAny CT_FASTCALL get(const V8Private & p) const;

		// ���� ������ ����ϴ�.
		CBS_EASYV8_DLLEXPORT bool CT_FASTCALL remove(const V8Private & p) const;

		// �迭�� ������ ����ֽ��ϴ�
		CBS_EASYV8_DLLEXPORT void CT_FASTCALL set(uint32_t idx, JsAny value);

		// �迭�� ������ �����ɴϴ�
		CBS_EASYV8_DLLEXPORT JsAny CT_FASTCALL get(uint32_t idx) const;
		
		CBS_EASYV8_DLLEXPORT void CT_FASTCALL setInternal(int idx, JsAny value) const;
		CBS_EASYV8_DLLEXPORT JsAny CT_FASTCALL getInternal(int idx) const;
		CBS_EASYV8_DLLEXPORT int CT_FASTCALL internalFieldCount() const;

		// ����Ƽ�� ��ü�� �����ɴϴ�
		// ����Ƽ�� ��ü�� �ƴϸ�, NULL�� ���ɴϴ�
		CBS_EASYV8_DLLEXPORT NativeObject * CT_FASTCALL getNativeObject() const;

		// ���� ������ ����ֽ��ϴ�
		template <typename T>
		inline void set(const JsField<T> & p, T value)
		{
			return setInternal(p.getIndex(), (JsAny)value);
		}

		// ���� ������ �����ɴϴ�
		template <typename T>
		inline T get(const JsField<T> & p) const
		{
			return getInternal(p.getIndex()).cast<T>();
		}

		// ����Ƽ�� ��ü�� Ÿ���� ���ؼ� �����ɴϴ�
		// �ش� Ÿ�԰� ���� ������ NULL�� ���ɴϴ�
		template <typename T>
		inline T * getNativeObject() const
		{
			return dynamic_cast<T*>(getNativeObject());
		}

		// ��ü�� �Լ��� ȣ���մϴ�
		// JsAny �����͸� �̿��Ͽ� �Ķ���͸� �ѱ�ϴ�
		inline JsAny callRaw(Text16 name, JsArgumentsIn args) const;

		// ��ü�� Ŭ������ ����ֽ��ϴ�
		template <typename Class> inline void setClass();

		// ��ü�� �Լ��� ����ֽ��ϴ�
		// const JsArguments & �� �̿��Ͽ� �Ķ���͸� �޽��ϴ�
		template <typename LAMBDA> inline void setFunctionRaw(Text16 name, LAMBDA lambda);

		// ��ü�� �Լ��� ȣ���մϴ�.
		template <typename RET, typename ... ARGS> inline RET call(Text16 name, const ARGS & ... args) const;

		// ��ü�� �Լ��� ����ֽ��ϴ�.
		// ���ο��� �Ķ���� Ÿ�Կ� ���߾� ĳ���� �˴ϴ�.
		template <typename LAMBDA> inline void setFunction(Text16 name, LAMBDA lambda);
	};


	// V8 ���� ��ü�� ����
	// V8 �迭
	template <>
	class V8Handle<v8::Array> : public V8Object
	{
		friend _pri_::InternalTools;
		friend JsAny;
		friend V8Object;
		friend V8Class;
		friend JsContext;
		friend NativeObject;
		
	public:
		inline V8Handle();
		inline V8Handle(std::nullptr_t);
		inline V8Handle(const V8Handle &obj);
		inline explicit V8Handle(const V8Value & any);

		// �迭 �Ҵ�
		CBS_EASYV8_DLLEXPORT static V8Handle<v8::Array> CT_FASTCALL newInstance(uint32_t length = 0);

		// �迭 ����
		CBS_EASYV8_DLLEXPORT uint32_t CT_FASTCALL getLength() const;

		// ��ü�� �Լ��� ȣ���մϴ�.
		// JsAny �����͸� �̿��Ͽ� �Ķ���͸� �ѱ�ϴ�.
		inline JsAny call(Text16 name, JsArgumentsIn args) const;

		// ��ü�� Ŭ������ ����ֽ��ϴ�.
		template <typename Class> inline void setClass();

		// ��ü�� �Լ��� ����ֽ��ϴ�.
		// const JsArguments & �� �̿��Ͽ� �Ķ���͸� �޽��ϴ�.
		template <typename LAMBDA> inline void setFunctionRaw(Text16 name, LAMBDA lambda);

		// ��ü�� �Լ��� ȣ���մϴ�.
		template <typename RET, typename ... ARGS> inline RET call(Text16 name, const ARGS & ... args) const;

		// ��ü�� �Լ��� ����ֽ��ϴ�.
		// ���ο��� �Ķ���� Ÿ�Կ� ���߾� ĳ���� �˴ϴ�.
		template <typename LAMBDA> inline void setFunction(Text16 name, LAMBDA lambda);
	};

	// V8 ���� ��ü�� ����
	// V8 �Լ�
	template <>
	class V8Handle<v8::Function> : public V8Object
	{
		friend _pri_::InternalTools;
		friend JsAny;
		friend V8Object;

	public:
		inline V8Handle();
		inline V8Handle(std::nullptr_t);
		inline V8Handle(const V8Handle &obj);
		inline explicit V8Handle(const V8Value & any);
		
		CBS_EASYV8_DLLEXPORT JsAny CT_FASTCALL callRaw(const V8Object &_this, JsArgumentsIn args) const;
		CBS_EASYV8_DLLEXPORT V8Object CT_FASTCALL callNewRaw(JsArgumentsIn args) const;

		// ��ü�� �Լ��� ȣ���մϴ�.
		template <typename RET, typename T, typename ... ARGS> inline RET call(T* _this, const ARGS & ... args) const;
		// ��ü�� �Լ��� ȣ���մϴ�.
		// ��ü�� ��������� undefined�� ��ȯ�մϴ�.
		template <typename RET, typename T, typename ... ARGS> inline RET safeCall(T* _this, const ARGS & ... args) const;
		// ��ü�� �Լ��� ȣ���մϴ�.
		template <typename ... ARGS> inline V8Object callNew(const ARGS & ... args) const;
	};
}