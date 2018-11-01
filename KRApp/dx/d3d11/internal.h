#pragma once

#include <KRUtil/wl/com.h>

#include <KR3/wl/prepare.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#include "../dxcommon.h"
#include DXGI_HEADER
#include <KR3/wl/clean.h>

namespace kr
{
	namespace d3d11
	{
		extern Com<ID3D11Device> s_d3d11;
		extern Com<IDXGIAdapter> s_adapter;
		extern Com<IDXGIDevice> s_dxgiDevice;
		extern Com<IDXGIFactoryT> s_dxFactory;

		bool isSupportMipAutogen(DXGI_FORMAT format) noexcept;
	}
}
