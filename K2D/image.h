#pragma once

#include <KR3/main.h>

#include "common.h"
#include "reformatter.h"

class kr::gl::ImageData
{
public:
	ImageData() noexcept = default;
	ImageData(nullptr_t) noexcept;

	ImageData & operator = (nullptr_t) noexcept;

	void * allocate(PixelFormat pf, int w, int h) noexcept;
	void * allocate(PixelFormat pf, int w, int h, int pitch) noexcept;
	void allocate(ImageData * _img, int _x, int _y, int _w, int _h) noexcept;
	void allocate(ImageData * _src, PixelFormat _pf) noexcept;
	void copyBits(const void * data) noexcept;
	void copyBitsReverseX(const void * data) noexcept;
	void copyBitsReverseY(const void * data) noexcept;
	void copyBitsReverseXY(const void * data) noexcept;
	void copyBits(const void * data, int _srcPitch) noexcept;
	void copyBitsReverseX(const void * data, int _srcPitch) noexcept;
	void copyBitsReverseY(const void * data, int _srcPitch) noexcept;
	void copyBitsReverseXY(const void * data, int _srcPitch) noexcept;
	ImageData subimage(int _x, int _y, int _w, int _h) noexcept;
	void copy(ImageData * _img, int _dstX, int _dstY, int _srcX, int _srcY, int _srcW, int _srcH) noexcept;

	// _src: �ȼ� ������ this�� ���ƾ� �Ѵ�.
	void imageProcessing(const ImageData * _src, const float * _weightTable, int _weightX, int _weightY, int _weightWidth, int _weightHeight) noexcept;

	// _src: �ȼ� ������ this�� ���ƾ� �Ѵ�.
	void blur(const ImageData * _src) noexcept;

	// _alpha: A8 ������ �̹���, ũ�Ⱑ ���ƾ��Ѵ�.
	void setAlpha(const ImageData * _alpha) noexcept;

	void fill(color _color) noexcept;

	// _alpha: A8 ������ �̹���, ũ�Ⱑ ���ƾ��Ѵ�.
	void recolor(color _color, const ImageData * _mask) noexcept;

	void recolor(color _color) noexcept;

	void free() noexcept;
	bool exists() const noexcept;
	int getWidth() const noexcept;
	int getHeight() const noexcept;
	int getPitch() const noexcept;
	void attach(PixelFormat pf, ptr nimg, int p, int w, int h) noexcept;
	ptr getBits() noexcept;
	cptr getBits() const noexcept;
	PixelFormat getPixelFormat() const noexcept;
	const FormatInfo * getPixelInfo() const noexcept;
	int getPixelSize() const noexcept;

	bool loadFromTgaFile(FILE * file, Palette * palette) noexcept;
	bool loadFromBmpFile(FILE * file, Palette * palette) noexcept;
	bool loadFromPngFile(FILE * file) noexcept;
	bool loadFromJpgFile(FILE * file) noexcept;
	bool loadFromTgaFile(pcwstr filename, Palette * palette) noexcept;
	bool loadFromBmpFile(pcwstr filename, Palette * palette) noexcept;
	bool loadFromPngFile(pcwstr filename) noexcept;
	bool loadFromJpgFile(pcwstr filename) noexcept;
	bool saveToBmpFile(pcwstr filename, Palette * palette) noexcept;
	bool saveToTgaFile(pcwstr filename, Palette * palette) noexcept;
	bool saveToPngFile(pcwstr filename) noexcept;
	bool saveToJpgFile(pcwstr filename) noexcept;

	static bvec4 color;

protected:
	ptr m_image;
	PixelFormat m_pf;
	int m_pitch;
	int m_width, m_height;
};