#pragma once

#include "globject.h"

namespace kr
{
	namespace gl
	{
		class Shader:public Object
		{
		public:
			Shader() = default;
			Shader(GLenum type, const GLchar * shaderSrc) noexcept;
			void create(GLenum type, const GLchar * shaderSrc) noexcept;
			void remove() noexcept;
		};

		class VertexShader :public Shader
		{
		public:
			VertexShader() = default;
			VertexShader(const GLchar * shaderSrc) noexcept;
			void create(const GLchar * shaderSrc) noexcept;
		};

		class FragmentShader :public Shader
		{
		public:
			FragmentShader() = default;
			FragmentShader(const GLchar * shaderSrc) noexcept;
			void create(const GLchar * shaderSrc) noexcept;
		};

		class AttribLocation
		{
		public:
			AttribLocation() = default;
			AttribLocation(GLint id) noexcept;
			AttribLocation & operator =(GLint id) noexcept;
			void enable() noexcept;
			void pointer(GLint size, GLenum type, GLboolean normalized, GLsizei stride, uintptr_t offset) noexcept;
			GLint getId() noexcept;
			operator GLint() noexcept;
		
		private:
			GLint m_id;
		};

		class Program :public Object
		{
		public:
			Program() = default;
			Program(Shader vs, Shader fs) noexcept;
			static Program create() noexcept;
			void create(Shader vs, Shader fs) noexcept;
			void remove() noexcept;
			void attach(Shader shader) noexcept;
			void bindAttribLocation(GLuint index, const GLchar * name) noexcept;
			void link() noexcept;
			void use() noexcept;
			GLint getInt(GLenum pname) noexcept;
			GLint getLinkState() noexcept;
			GLint getInfoLogLength() noexcept;
			void getInfoLog(GLsizei bufSize, GLsizei * length, GLchar * infoLog) noexcept;
			AText getInfoLog() noexcept;
			GLint getAttribLocation(const GLchar * name) noexcept;
		};
	}
}
