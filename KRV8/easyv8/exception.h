#pragma once

namespace kr
{
	// �ڹٽ�ũ��Ʈ ���� ������ ������ �ִ� Ŭ�����̴�.
	// �� �ʵ�� ���ܰ� �߻��� ��ġ�� ���� �����̴�.
	class JsException
	{
		friend V8Object;
	public:
		// ���� ��
		JsString fileName;
		// ���� ����
		int lineNumber;
		// ���� ��ġ ����
		int startColumn;
		// ���� ��ġ ��
		int endColumn;
		// �ش� ������ �ҽ�
		JsString sourceLine;
		// �Լ� ȣ�� ����
		JsString stackTrace;
		// ������ ���� ����
		JsString message;

		CBS_EASYV8_DLLEXPORT JsException(v8::TryCatch * trycatch);
		inline JsException(JsString message)
		{
			this->message = move(message);
			lineNumber = startColumn = endColumn = 0;
		}
		inline JsException(Text16 message)
		{
			this->message = JsString(message);
			lineNumber = startColumn = endColumn = 0;
		}
		inline JsException(JsString strName, int line, int startcolumn, int endcolumn, JsString sourceline, JsString stacktrace, JsString message)
		{
			fileName = move(strName);
			endColumn = endcolumn;
			startColumn = startcolumn;
			lineNumber = line;
			this->message = move(message);
			sourceLine = move(sourceline);
			stackTrace = move(stacktrace);
		}
		inline JsException(const JsException& _copy)
		{
			fileName = _copy.fileName;
			lineNumber = _copy.lineNumber;
			startColumn = _copy.startColumn;
			endColumn = _copy.endColumn;
			sourceLine = _copy.sourceLine;
			stackTrace = _copy.stackTrace;
			message = _copy.message;
		}
		inline JsException(JsException&& _move)
		{
			fileName = move(_move.fileName);
			lineNumber = _move.lineNumber;
			startColumn = _move.startColumn;
			endColumn = _move.endColumn;
			sourceLine = move(_move.sourceLine);
			stackTrace = move(_move.stackTrace);
			message = move(_move.message);
		}
		inline JsException& operator =(const JsException& _copy)
		{
			this->~JsException();
			new(this) JsException(_copy);
			return *this;
		}
		inline JsException& operator =(JsException&& _move)
		{
			this->~JsException();
			new(this) JsException(move(_move));
			return *this;
		}
	};
}