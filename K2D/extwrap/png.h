#pragma once

#include <KR3/main.h>
#include "../image.h"

namespace kr
{
	namespace image
	{
		class Png;
	}
}

class kr::image::Png
{
public:
	static ImageData load(FILE * fp) noexcept;
	static ImageData load(io::VIStream<void> buffer) noexcept;
	static bool save(const ImageData & image, pcstr16 filename) noexcept;
};