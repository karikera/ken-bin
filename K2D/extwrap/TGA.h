#pragma once

#include <KR3/main.h>
#include "../image.h"

namespace kr
{
	namespace image
	{
		class Tga
		{
		public:
			static image::ImageData load(io::VIStream<void> is, image::Palette * _palette) noexcept;
			static image::ImageData load(FILE * _fp, image::Palette * _palette) noexcept;
			static bool save(const char * sz_filename, bool blCompress) noexcept;
		};
	}
}
