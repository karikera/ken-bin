#pragma once

#include <KR3/main.h>

#define GL_GLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <GLES2/gl2.h>

namespace kr
{
	namespace gl
	{
		class Object
		{
		public:
			GLuint getId() noexcept;

		protected:
			GLuint m_id;
		};

		class Buffer:public Object
		{
		public:
			Buffer() = default;
			void generate() noexcept;
			void generate(kr::Buffer buffer) noexcept;
			void remove() noexcept;
			void bind() noexcept;
		};
	}
}
