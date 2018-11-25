#pragma once

#include <KR3/main.h>
#include "../image.h"

namespace kr
{
	namespace image
	{
		class Jpeg;
	}
}

class kr::image::Jpeg
{
public:
	static constexpr int QUALITY_MAX = 100;
	static ImageData load(io::VIStream<void> is) noexcept;
	static ImageData load(FILE * infile) noexcept;
	static bool save(const ImageData & data, pcstr16 filename, int quality) noexcept;
};