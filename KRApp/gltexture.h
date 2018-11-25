#pragma once

#include <KRMessage/promise.h>
#include <K2D/image.h>

#include "globject.h"

namespace kr
{
	namespace gl
	{
		class Texture:public Object
		{
		public:
			static Promise<Texture>* load(Text16 filename) noexcept;
			Texture() noexcept = default;
			Texture(nullptr_t) noexcept;
			void generate() noexcept;
			void remove() noexcept;
		};

		template <GLenum type>
		class TextureTarget
		{
		public:
			Texture operator =(Texture texture) noexcept
			{
				glBindTexture(type, texture.getId());
				return texture;
			}

			void image2D(const image::ImageData * img) noexcept
			{
				image::ImageData temp = nullptr;
				const image::ImageData * result = image::reformat(&temp, img, image::PixelFormat::ARGB8);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, result->getWidth(), result->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, result->getBits());
				temp.free();
			}

			void parameteri(GLenum name, GLenum value) noexcept
			{
				glTexParameteri(type, name, value);
			}

			void minFilter(GLenum value) noexcept
			{
				parameteri(GL_TEXTURE_MIN_FILTER, value);
			}

			void magFilter(GLenum value) noexcept
			{
				parameteri(GL_TEXTURE_MAG_FILTER, value);
			}
		};

		static TextureTarget<GL_TEXTURE_2D> &texture2D = nullref;
	}
}
