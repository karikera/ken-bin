#pragma once

#include "app.h"
#include "glutil.h"

namespace kr
{
	class WebCanvasGL: public Application
	{
	public:
		WebCanvasGL(int width, int height) noexcept;
		~WebCanvasGL() noexcept;

		void onResize(int width, int height) noexcept override;

	private:
		GLContext m_ctx;
	};

}
