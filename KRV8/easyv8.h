#pragma once

#include <KR3/main.h>

#include <stdint.h>
#include <vector>
#include <string>
#include <new>

#include "easyv8/ref.h"
#include "easyv8/exports.h"
#include "easyv8/external.h"
#include "easyv8/meta.h"
#include "easyv8/jsvar.h"
#include "easyv8/v8data.h"
#include "easyv8/private.h"
#include "easyv8/class.h"
#include "easyv8/nativefunction.h"
#include "easyv8/nativeclass.h"
#include "easyv8/exception.h"
#include "easyv8/arguments.h"

#include "easyv8/meta.inl"
#include "easyv8/jsvar.inl"
#include "easyv8/private.inl"
#include "easyv8/class.inl"
#include "easyv8/v8data.inl"

namespace kr
{
	// �����ϵ� �ڹٽ�ũ��Ʈ
	class JsCode
	{
		friend JsContext;
	public:
		// �ڹٽ�ũ��Ʈ�� ������ ��Ų��.
		// �Ķ���� fileName: ���ܰ� �߻��ϸ�, �߻� ���� �̸��� �־���� ���ڿ�
		// �Ķ���� source: �ҽ� ���ڿ�
		CBS_EASYV8_DLLEXPORT JsCode() noexcept;
		CBS_EASYV8_DLLEXPORT JsCode(Text16 fileName, Text16 source);
		CBS_EASYV8_DLLEXPORT ~JsCode() noexcept;
		CBS_EASYV8_DLLEXPORT JsCode(const JsCode &_copy) noexcept;
		CBS_EASYV8_DLLEXPORT bool CT_FASTCALL empty() const noexcept;
		inline JsCode& operator =(const JsCode &_copy) noexcept
		{
			this->~JsCode();
			new (this) JsCode(_copy);
			return *this;
		}

	private:
		v8::Persistent<v8::Script> m_script;
	};

	// ���� V8 ������ ����� �� �ִ� Ŭ�����̴�.
	// execute �� ȣ���Ͽ� �����Ѵ�.
	class JsContext
	{
		friend JsCode;
		friend V8Object;
	public:
		CBS_EASYV8_DLLEXPORT JsContext() noexcept;
		CBS_EASYV8_DLLEXPORT JsContext(const JsContext & _ctx) noexcept;
		CBS_EASYV8_DLLEXPORT ~JsContext() noexcept;
		
		// �ڹٽ�ũ��Ʈ�� �����Ѵ�.
		// ����: JsException
		CBS_EASYV8_DLLEXPORT JsAny CT_FASTCALL run(const JsCode & code);
		CBS_EASYV8_DLLEXPORT V8Object CT_FASTCALL global();
		CBS_EASYV8_DLLEXPORT void CT_FASTCALL enter();
		CBS_EASYV8_DLLEXPORT void CT_FASTCALL exit();

		inline JsCode compile(Text16 source) // JsException
		{
			return JsCode(u"[untitled]", source);
		}
		inline JsCode compile(Text16 fileName, Text16 source) // JsException
		{
			return JsCode(fileName, source);
		}
		inline JsAny run(Text16 fileName, Text16 source) // JsException
		{
			return run(compile(fileName, source));
		}
		inline JsAny run(Text16 source) // JsException
		{
			return run(compile(source));
		}

		CBS_EASYV8_DLLEXPORT static void CT_FASTCALL gc() noexcept;
		CBS_EASYV8_DLLEXPORT static void CT_FASTCALL terminate() noexcept;
		
	private:
		v8::Persistent<v8::Context> m_context;
	};
	class JsContextScope
	{
	public:
		inline JsContextScope(const JsContext & ctx) noexcept;
		inline ~JsContextScope() noexcept;

	private:
		JsContext m_ctx;
	};
	inline JsContextScope::JsContextScope(const JsContext & ctx) noexcept
		:m_ctx(ctx)
	{
		m_ctx.enter();
	}
	inline JsContextScope::~JsContextScope() noexcept
	{
		m_ctx.exit();
	}
}
