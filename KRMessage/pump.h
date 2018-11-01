#pragma once

#include <KR3/util/callable.h>
#include <KR3/mt/thread.h>
#include <KR3/mt/criticalsection.h>
#include <KR3/util/keeper.h>
#include "promise.h"

namespace kr
{
	struct EventProcedure
	{
		EventHandle * event;
		void * param;
		void(*callback)(void*);
	};
	class EventPump
	{
	public:
		constexpr static dword MAXIMUM_WAIT = EventHandle::MAXIMUM_WAIT - 1;
		class Timer;

	private:
		class NodeHead
		{
			friend EventPump;
		protected:
			Timer * m_next;
		};
	public:
		class Timer :public Interface<NodeHead>
		{
			friend EventPump;
			friend Referencable<Timer>;
		public:
			Timer() noexcept;
			Timer(timepoint at) noexcept;
			virtual ~Timer() noexcept;
			bool isPosted() noexcept;
			void setTime(timepoint time) noexcept;
			void addTime(duration time) noexcept;
			timepoint getTime() const noexcept;
			void AddRef() noexcept;
			size_t Release() noexcept;
			size_t getRefCount() noexcept;

			template <typename LAMBDA>
			static Timer * create(timepoint at, LAMBDA lambda) noexcept;
		protected:
			virtual void call() = 0;

			size_t m_ref;
			Timer * m_prev;
			timepoint m_at;
		};
	public:
		void quit(int exitCode) noexcept;
		void clear() noexcept;
		void terminate() noexcept;

		// node는 내부에서 관리하게 된다.
		// return: 이미 만료 혹은 취소된 이벤트의 경우 false
		bool cancel(Timer * node) noexcept;

		// newnode는 내부에서 관리하게된다.
		void post(Timer * newnode) noexcept;

		template <typename LAMBDA>
		void postL(LAMBDA lambda) noexcept
		{
			return post(Timer::create(timepoint::now(), move(lambda)));
		}

		template <typename LAMBDA>
		void postL(timepoint at, LAMBDA lambda) noexcept
		{
			return post(Timer::create(at, move(lambda)));
		}

		template <typename LAMBDA>
		Timer* makePost(timepoint at, LAMBDA lambda) noexcept
		{
			Timer * node = Timer::create(at, move(lambda));
			node->AddRef();
			if (post(node))
			{
				return node;
			}
			node->Release();
			return nullptr;
		}

		template <typename LAMBDA>
		void postL(duration wait, LAMBDA lambda) noexcept
		{
			return postL(timepoint::now() + wait, move(lambda));
		}

		template <typename LAMBDA>
		Timer* makePost(duration wait, LAMBDA lambda) noexcept
		{
			return makePost(Timer::create(timepoint::now() + wait, move(lambda)));
		}

		// events를 대기한다.
		// return: 셋된 이벤트의 인덱스
		dword wait(View<EventHandle *> events) throw(QuitException);

		// events를 time동안 대기한다.
		// return: 셋된 이벤트의 인덱스
		dword wait(View<EventHandle *> events, duration time) throw(QuitException);

		// events를 timeto까지 대기한다.
		// return: 셋된 이벤트의 인덱스
		dword waitTo(View<EventHandle *> events, timepoint timeto) throw(QuitException);

		void processOnce() throw(QuitException);
		dword processOnce(View<EventHandle *> events) throw(QuitException);
		void processOnce(View<EventProcedure> proc) throw(QuitException);
		void processOnceWithoutMessage() throw(QuitException);
		void processOnceWithoutMessage(View<EventProcedure> proc) throw(QuitException);
		void wait(EventHandle * event) throw(QuitException);
		void wait(EventHandle * event, duration time) throw(QuitException);
		void waitTo(EventHandle * event, timepoint time) throw(QuitException);

		Promise<void> * promise(duration time) noexcept;
		Promise<void> * promiseTo(timepoint at) noexcept;

		void sleep(duration dura) throw(QuitException);
		void sleepTo(timepoint time) throw(QuitException);

		int messageLoop() noexcept;
		int messageLoopWith(View<EventProcedure> proc) noexcept;

		Callable* onAfterProcess(CallablePtr callable) noexcept;
		Callable* onFinish(CallablePtr callable) noexcept;
		static EventPump * getInstance() noexcept;

	private:
		EventPump() noexcept;
		~EventPump() noexcept;

		dword _tryProcess(EventHandle * const * events, dword count) throw(QuitException);
		TmpArray<EventHandle *> _makeEventArray(View<EventHandle *> events) noexcept;
		TmpArray<EventHandle *> _makeEventArray(View<EventProcedure> proc) noexcept;
		void _processMessage() throw(QuitException);
		dword _processTimer(dword maxSleep);
		void _fireAfterProcess() noexcept;

		CriticalSection m_timercs;
		EventHandle* m_msgevent;
		NodeHead m_start;
		Array<Callable*> m_onAfterProcess;
		Array<Callable*> m_onFinish;
		ThreadId m_threadId;
	};

	namespace _pri_
	{
		template <typename LAMBDA, size_t argn>
		struct CallWithParamIfHas
		{
			static void call(LAMBDA & lambda, EventPump::Timer * param) noexcept
			{
				lambda(param);
			}
		};
		template <typename LAMBDA>
		struct CallWithParamIfHas<LAMBDA, 0>
		{
			static void call(LAMBDA & lambda, EventPump::Timer * param) noexcept
			{
				lambda();
			}
		};
	}

	template <typename LAMBDA>
	static EventPump::Timer * EventPump::Timer::create(timepoint at, LAMBDA lambda) noexcept
	{
		struct LambdaWrap :public Timer
		{
			LAMBDA m_lambda;
			LambdaWrap(LAMBDA lambda, timepoint at) noexcept
				: Timer(at), m_lambda(move(lambda))
			{
			}
			void call() override
			{
				constexpr size_t argn = meta::function<LAMBDA>::args_t::size;
				_pri_::CallWithParamIfHas<LAMBDA, argn>::call(m_lambda, this);
			}
		};
		return _new LambdaWrap(move(lambda), at);
	}

	class EventThread
	{
	public:
		EventThread() noexcept;
		~EventThread() noexcept;

		void create() noexcept;
		void quit(int exitCode) noexcept;
		EventPump * operator ->() noexcept;

	private:
		ThreadHandle * m_thread;
		EventPump * m_pump;
	};

	using TimerEvent = EventPump::Timer;
}
