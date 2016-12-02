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
			timepoint getTime() const noexcept;
			void AddRef() noexcept;
			size_t Release() noexcept;

			template <typename LAMBDA>
			static Timer * create(timepoint at, LAMBDA lambda) noexcept
			{
				return _new LambdaTimer<LAMBDA>(move(lambda), at);
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

		protected:
			virtual void call() noexcept = 0;

		private:
			~Event() noexcept;

			size_t m_index;
		};
	private:
		template <typename LAMBDA>
		class LambdaTimer :public Timer
		{
		public:
			LambdaTimer(LAMBDA lambda, timepoint at) noexcept
				: Timer(at), m_lambda((LAMBDA&&)lambda)
			{
			}
			~LambdaTimer() noexcept override
			{
				m_prev = nullptr;
			}
			void call() noexcept override
			{
				m_lambda(this);
			}
		private:
			LAMBDA m_lambda;
		};
		template <typename LAMBDA>
		class LambdaEvent :public Event
		{
		public:
			LambdaEvent(LAMBDA lambda) noexcept
				:m_lambda(move(lambda))
			{
			}
			void call() noexcept override
			{
				m_lambda(this);
			}

		private:
			LAMBDA m_lambda;
		};

	public:
		EventPump() noexcept;
		~EventPump() noexcept;
		int start() noexcept;
		void quit(int exitCode) noexcept;
		void clear() noexcept;

		// �̺�Ʈ ���
		bool regist(EventHandle * event, Event * callback) noexcept;

		// �̺�Ʈ ����
		void unregist(Event * callback);

		// node�� ���ο��� �����ϰ� �ȴ�.
		// return: �̹� ���� Ȥ�� ��ҵ� �̺�Ʈ�� ��� false
		bool cancel(Timer * node) noexcept;

		// newnode�� ���ο��� �����ϰԵȴ�.
		// return: �̺�Ʈ ������ ���� �غ���� �ʾҰų�, �������� ��� false
		bool post(Timer * newnode) noexcept;

		// �̺�Ʈ ���
		template <typename LAMBDA>
		Event * registLambda(EventHandle * event, LAMBDA lambda) noexcept
		{
			Event * ev = _new LambdaEvent<LAMBDA>(move(lambda));
			if (regist(event, ev))
				return ev;
			delete ev;
			return nullptr;
		}

		// return: �̺�Ʈ ������ ���� �غ���� �ʾҰų�, �������� ��� false
		template <typename LAMBDA>
		bool postLambda(LAMBDA lambda) noexcept
		{
			return post(Timer::create(timepoint::now(), lambda));
		}

		// return: �̺�Ʈ ������ ���� �غ���� �ʾҰų�, �������� ��� false
		template <typename LAMBDA>
		bool postLambda(timepoint at, LAMBDA lambda) noexcept
		{
			return post(Timer::create(at, lambda));
		}

		// return: �̺�Ʈ ������ ���� �غ���� �ʾҰų�, �������� ��� nullptr
		template <typename LAMBDA>
		Timer* makePost(timepoint at, LAMBDA lambda) noexcept
		{
			Timer * node = _new LambdaTimer<LAMBDA>(move(lambda), at);
			node->AddRef();
			if (post(node))
			{
				return node;
			}
			node->Release();
			return nullptr;
		}

		// return: �̺�Ʈ ������ ���� �غ���� �ʾҰų�, �������� ��� false
		template <typename LAMBDA>
		bool postLambda(duration wait, LAMBDA lambda) noexcept
		{
			return post(timepoint::now() + wait, move(lambda));
		}

		// return: �̺�Ʈ ������ ���� �غ���� �ʾҰų�, �������� ��� false
		template <typename LAMBDA>
		bool makePost(duration wait, LAMBDA lambda) noexcept
		{
			return makePost(Timer::create(timepoint::now() + wait, move(lambda)));
		}

	private:
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

		int _thread() noexcept;
	};
}