#pragma once

#include "message.h"

#include <KR3/util/callable.h>
#include <KR3/mt/thread.h>
#include <KR3/mt/criticalsection.h>

namespace kr
{
	class EventPump
	{
	public:
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
			Timer(timepoint at) noexcept;
			virtual ~Timer() noexcept;
			bool isPosted() noexcept;
			timepoint getTime() const noexcept;
			void AddRef() noexcept;
			size_t Release() noexcept;

			template <typename LAMBDA>
			static Timer * create(timepoint at, LAMBDA lambda) noexcept
			{
				struct LambdaWrap :public Timer
				{
					LAMBDA m_lambda;
					LambdaWrap(LAMBDA lambda, timepoint at) noexcept
						: Timer(at), m_lambda(move(lambda))
					{
					}
					void call() noexcept override
					{
						m_lambda(this);
					}
				};
				return _new LambdaWrap(move(lambda), at);
			}
		protected:
			virtual void call() noexcept = 0;

			size_t m_ref;
			Timer * m_prev;
			timepoint m_at;
		};
		class Event
		{
			friend EventPump;
		public:
			Event() noexcept;

			template <typename LAMBDA>
			static Event * create(LAMBDA lambda) noexcept
			{
				class LambdaEvent :public Event
				{
				public:
					LambdaEvent(LAMBDA lambda) noexcept
						: m_lambda(move(lambda))
					{
					}
					void call() noexcept override
					{
						m_lambda(this);
					}

				private:
					LAMBDA m_lambda;
				};
				return _new LambdaEvent(move(lambda));
			}

		protected:
			virtual void call() noexcept = 0;

		private:
			~Event() noexcept;

			size_t m_index;
		};
	private:

	public:
		EventPump() noexcept;
		~EventPump() noexcept;
		
		ThreadHandle * createThread() noexcept;

		void reserveThread() noexcept;
		int start() noexcept;
		void quit(int exitCode) noexcept;
		void clear() noexcept;

		// 이벤트 등록
		bool regist(EventHandle * event, Event * callback) noexcept;

		// 이벤트 제거
		void unregist(Event * callback);

		// node는 내부에서 관리하게 된다.
		// return: 이미 만료 혹은 취소된 이벤트의 경우 false
		bool cancel(Timer * node) noexcept;

		// newnode는 내부에서 관리하게된다.
		// return: 이벤트 펌프가 아직 준비되지 않았거나, 없어졌을 경우 false
		bool post(Timer * newnode) noexcept;

		// 이벤트 등록
		template <typename LAMBDA>
		Event * registLambda(EventHandle * event, LAMBDA lambda) noexcept
		{
			Event * ev = Event::create(move(lambda));
			if (regist(event, ev)) return ev;
			delete ev;
			return nullptr;
		}

		// return: 이벤트 펌프가 아직 준비되지 않았거나, 없어졌을 경우 false
		template <typename LAMBDA>
		bool postLambda(LAMBDA lambda) noexcept
		{
			return post(Timer::create(timepoint::now(), move(lambda)));
		}

		// return: 이벤트 펌프가 아직 준비되지 않았거나, 없어졌을 경우 false
		template <typename LAMBDA>
		bool postLambda(timepoint at, LAMBDA lambda) noexcept
		{
			return post(Timer::create(at, move(lambda)));
		}

		// return: 이벤트 펌프가 아직 준비되지 않았거나, 없어졌을 경우 nullptr
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

		// return: 이벤트 펌프가 아직 준비되지 않았거나, 없어졌을 경우 false
		template <typename LAMBDA>
		bool postLambda(duration wait, LAMBDA lambda) noexcept
		{
			return post(timepoint::now() + wait, move(lambda));
		}

		// return: 이벤트 펌프가 아직 준비되지 않았거나, 없어졌을 경우 false
		template <typename LAMBDA>
		bool makePost(duration wait, LAMBDA lambda) noexcept
		{
			return makePost(Timer::create(timepoint::now() + wait, move(lambda)));
		}

	private:
		int _process() noexcept;
		int _thread() noexcept;
		void _regist(EventHandle * event, Event * callback) noexcept;
		void _unregist(Event * event) noexcept;

		CriticalSection m_timercs;
		EventList<MessageLoop::MAXIMUM_WAIT> m_events;
		Event* m_evdata[MessageLoop::MAXIMUM_WAIT-1];
		NodeHead m_start;
		ThreadId m_threadId;
	};

	class EventThread:public EventPump
	{
	public:
		EventThread() noexcept;
		~EventThread() noexcept;

	private:
		ThreadHandle * m_thread;
	};
}
