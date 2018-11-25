#pragma once

#include "common.h"
#include "image.h"

template <kr::image::PixelFormat pf> 
class kr::image::MemCanvas :public ImageData
{
public:
	void point(ivec2 pt) noexcept;
	void fill(const irectwh &rect) noexcept;
	void randomFill(const irectwh &rect, Random& rand = g_random) noexcept;
	void hLine(ivec2 pt, int len) noexcept;
	void hLineXOR(ivec2 pt, int len) noexcept;
	void vLine(ivec2 pt, int len) noexcept;
	void vLineXOR(ivec2 pt, int len) noexcept;
	void line(ivec2 pt1, ivec2 pt2) noexcept;
	void rectLine(const irectwh &rect) noexcept;
	void copy(ivec2 pt, MemCanvas& img) noexcept;
	void copy(ivec2 pt, const irectwh &rect, MemCanvas& img) noexcept;
	void triangle(ivec2 pt1, ivec2 pt2, ivec2 pt3) noexcept;
	void ellipse(const irectwh &rect) noexcept;
};
template <kr::image::PixelFormat pf>
class kr::image::MemBitmap :public kr::image::MemCanvas<pf>
{
public:
	MemBitmap(nullptr_t) noexcept;
	MemBitmap(int w, int h) noexcept;
	~MemBitmap() noexcept;
	void resize(int w, int h) noexcept;
	void remove() noexcept;
	ImageData getData() const noexcept;

	bool operator ==(const MemBitmap &img) const noexcept;
	bool operator !=(const MemBitmap &img) const noexcept;
};

#define WRAP(v)	v
#define DECL(format) \
extern template kr::image::MemCanvas<kr::image::PixelFormat::format>;\
extern template kr::image::MemBitmap<kr::image::PixelFormat::format>;

#include "formatlist.h"
