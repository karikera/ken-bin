#pragma once

#define GL_GLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <GLES2/gl2.h>

namespace kr
{

	class GLContext
	{
	public:
		GLContext(EGLNativeDisplayType display, EGLNativeWindowType window) noexcept;
		~GLContext() noexcept;
		void swap() noexcept;

	private:
		EGLContext m_eglContext;
		EGLSurface m_eglSurface;
		EGLDisplay m_eglDisplay;
	};


	namespace gl
	{
		class Shader
		{
		public:
			Shader() = default;
			Shader(GLenum type, const GLchar * shaderSrc) noexcept;
			void remove() noexcept;
			GLuint getId() noexcept;

		private:
			GLuint m_id;
		};

		class Program
		{
		public:
			Program() = default;
			static Program create() noexcept;
			void remove() noexcept;
			GLuint getId() noexcept;
			void attach(Shader shader) noexcept;
			void bindAttribLocation(GLuint index, const GLchar * name) noexcept;
			void link() noexcept;
			void use() noexcept;
			GLint getInt(GLenum pname) noexcept;
			GLint getLinkState() noexcept;
			GLint getInfoLogLength() noexcept;
			void getInfoLog(GLsizei bufSize, GLsizei * length, GLchar * infoLog) noexcept;
			AText getInfoLog() noexcept;

		private:
			GLuint m_id;
		};


	}

}
