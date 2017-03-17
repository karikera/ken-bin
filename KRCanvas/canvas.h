#pragma once

#ifdef WIN32
#include <KR3/wl/windows.h>
#endif

#include <EGL/egl.h>

namespace kr
{
	class WebCanvas
	{
	public:
		WebCanvas(int width, int height) noexcept;
		~WebCanvas() noexcept;
		int getWidth() noexcept;
		int getHeight() noexcept;

		virtual void onResize(int width, int height) noexcept;
		virtual void onKeyDown(int key, bool repeat) noexcept;
		virtual void onKeyUp(int key) noexcept;

	protected:
		EGLNativeWindowType m_window;
		EGLNativeDisplayType m_display;
		int m_width, m_height;
	};
}

