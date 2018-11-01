#pragma once

#include <KR3/main.h>
#include <KR3/util/time.h>
#ifdef WIN32
#include <KRMessage/pump.h>
#endif
#include <emscripten.h>
#include <EGL/egl.h>

namespace kr
{
	class Application;

	namespace _pri_
	{
#ifdef __EMSCRIPTEN__
		void emOndrawCallback() noexcept;
#endif
		void setApplication(Application * proc) noexcept;
	}

	class Timeout
#ifdef WIN32
		: private EventPump::Timer
#endif
	{
	public:
		Timeout() noexcept;
		bool post(duration to) noexcept;
		bool cancel() noexcept;

	protected:
#ifdef WIN32
		virtual void call() noexcept override = 0;
#elif defined(__EMSCRIPTEN__)
		virtual void call() noexcept = 0;
#endif

	private:
#ifdef __EMSCRIPTEN__
		int m_timeoutId;
#endif

	public:
		template <typename LAMBDA>
		static Timeout * create(LAMBDA lambda) noexcept
		{
			struct LambdaWrap :public Timeout
			{
				LAMBDA m_lambda;
				LambdaWrap(LAMBDA lambda) noexcept
					: m_lambda(move(lambda))
				{
				}
				void call() noexcept override
				{
					m_lambda(this);
				}
			};
			return _new LambdaWrap(move(lambda));
		}
	};

	class Interval
	{
	public:
		Interval() noexcept;
		bool start(duration interval) noexcept;
		bool stop() noexcept;

	protected:
		virtual void call() noexcept = 0;

	private:
#ifdef WIN32
		class Wrapper : public EventPump::Timer
		{
		public:
			using EventPump::Timer::Timer;
			bool start(duration interval) noexcept;

		protected:
			void call() noexcept override;

		private:
			duration m_interval;
		};
		Wrapper m_wrapper;
#endif
#ifdef __EMSCRIPTEN__
		int m_intervalId;
#endif

	public:
		template <typename LAMBDA>
		static Interval * create(LAMBDA lambda) noexcept
		{
			struct LambdaWrap :public Interval
			{
				LAMBDA m_lambda;
				LambdaWrap(LAMBDA lambda) noexcept
					: m_lambda(move(lambda))
				{
				}
				void call() noexcept override
				{
#ifdef WIN32
					_repost();
					m_lambda(this);
#elif defined(__EMSCRIPTEN__)
					m_lambda(this);
#endif
				}
			};
			return _new LambdaWrap(move(lambda));
		}
	};

	template <typename LAMBDA>
	Interval* setInterval(LAMBDA lambda, int interval) noexcept
	{
		return setInterval(move(lambda), (duration)interval);
	}

	template <typename LAMBDA>
	Interval * setInterval(LAMBDA lambda, duration interval) noexcept
	{
		Interval * obj = Interval::create(move(lambda));
		obj->start(interval);
		return obj;
	}

	template <typename LAMBDA>
	Timeout* setTimeout(LAMBDA lambda, int dura) noexcept
	{
		return setTimeout(move(lambda), (duration)dura);
	}

	template <typename LAMBDA>
	Timeout* setTimeout(LAMBDA lambda, duration dura) noexcept
	{
		Timeout * obj = Timeout::create(move(lambda));
		obj->post(dura);
		return obj;
	}

	class Application
	{
	public:
		Application(int width, int height) noexcept;
		~Application() noexcept;
		int getWidth() noexcept;
		int getHeight() noexcept;

		virtual void swap() noexcept;
		virtual void onDraw() noexcept;
		virtual void onResize(int width, int height) noexcept;
		virtual void onKeyDown(int key, bool repeat) noexcept;
		virtual void onKeyUp(int key) noexcept;
		virtual void onMouseMove(int x, int y) noexcept;
		virtual void onMouseDown(int x, int y, int button) noexcept;
		virtual void onMouseUp(int x, int y, int button) noexcept;

	protected:
		int m_width, m_height;
		EGLNativeWindowType m_window;
		EGLNativeDisplayType m_display;
	};

	void openDevTools() noexcept;
}

#ifdef __EMSCRIPTEN__

#define main_loop(app) { \
		::kr::_pri_::setApplication(app); \
		emscripten_set_main_loop(::kr::_pri_::emOndrawCallback, 60, true); \
	} while(0,0);

#elif defined(WIN32)

#define main_loop(app) { \
		::kr::_pri_::setApplication(app); \
	} while(0,0);

#endif