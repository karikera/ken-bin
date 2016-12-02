#pragma once

#ifndef WIN32
#error is not windows system
#endif

#include <KR3/wl/windows.h>
#include <KR3/wl/eventhandle.h>

#include <KR3/util/time.h>
#include <KR3/util/callable.h>
#include <KR3/math/coord.h>
#include <KR3/data/linkedlist.h>
#include <KR3/mt/thread.h>
#include <KR3/mt/criticalsection.h>

namespace kr
{
	class MessageStruct
	{
	public:
		win::Window*	window;
		uint	message;
		WPARAM	wParam;
		LPARAM	lParam;
		DWORD	time;
		ivec2	point;

#ifdef _MAC
		DWORD       lPrivate;
#endif
		LPMSG getMessageStruct() noexcept;

		ivec2 getPointFromParam() const noexcept;
		bool isDlgMessageA(HWND wnd) noexcept;
		bool isDlgMessage(HWND wnd) noexcept;

		bool get() noexcept;
		bool peek() noexcept;
		bool tryGet() noexcept;
		bool translate() const noexcept;
		LRESULT dispatch() const noexcept;
	};

	class Translator
	{
	public:
		virtual bool translate(const MessageStruct* pMsg) = 0;

	};
	
	class AcceleratorTranslator :public Node<Translator>
	{
		virtual bool translate(const MessageStruct* pMsg) noexcept override;

	public:
		AcceleratorTranslator(HWND hWnd, HACCEL hAccTable) noexcept;

		HWND m_hWnd;
		HACCEL m_hAccTable;
	};

	class BasicTranslator :public Node<Translator>
	{
		virtual bool translate(const MessageStruct* pMsg) noexcept override;
	};

	class Message;

	class MessageLoop :public Chain<Translator>
	{
	public:
		constexpr static dword MAXIMUM_WAIT = EventHandle::MAXIMUM_WAIT - 1;

		MessageLoop() noexcept;
		~MessageLoop() noexcept;

		void attachAccelerator(HWND hWnd, HACCEL hAccel) noexcept;
		void attachBasicTranslator() noexcept;
		bool get() noexcept;
		bool tryGet() noexcept;
		bool peek() noexcept;

		// Throws:
		//   QuitException: WM_QUIT �� ���Ͽ� ����Ǿ���.
		//   Etc... : 
		void dispatch();

		int messageLoop(); // QuitException
		uint getLastMessage() noexcept;
		void tryProcess(); // QuitException
		dword tryProcess(RefArray<EventHandle *> events); // QuitException
		void wait(EventHandle * event); // QuitException
		void wait(EventHandle * event, duration time); // QuitException
		void waitTo(EventHandle * event, timepoint time); // QuitException

		// events�� ����Ѵ�.
		// return: �µ� �̺�Ʈ�� �ε���
		dword wait(RefArray<EventHandle *> events); // QuitException

		// events�� time���� ����Ѵ�.
		// return: �µ� �̺�Ʈ�� �ε���
		dword wait(RefArray<EventHandle *> events, duration time); // QuitException

		// events�� timeto���� ����Ѵ�.
		// return: �µ� �̺�Ʈ�� �ε���
		dword waitTo(RefArray<EventHandle *> events, timepoint timeto); // QuitException
		void sleep(duration dura); // QuitException
		void sleepTo(timepoint time); // QuitException

		bool post(Callable * callable) noexcept;

		template <typename LAMBDA>
		bool post(LAMBDA lambda) noexcept
		{
			return post(Callable::wrap(move(lambda)));
		}

	private:
		dword _tryProcess(EventHandle * const * events, dword count); // QuitException
		TmpArray<EventHandle *> _makeEventArray(RefArray<EventHandle *> events) noexcept;
		bool _processMessage(dword index, dword count);

		CriticalSection m_cs;
		LinkedList<Keep<Callable>> m_queue;
		MessageStruct m_msg;
		ThreadId m_threadId;
		Event m_msgevent;
		
	};

	
	extern thread_local MessageLoop g_pump;

	ivec2 getPointFromParam(LPARAM lParam) noexcept;
}