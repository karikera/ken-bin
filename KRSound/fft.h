#pragma once

#include "complex.h"
#include "sound.h"

typedef struct fftwf_plan_s * fftwf_plan;

namespace kr
{
	namespace sound
	{
		class SoundCaptureSampler :private SoundCapture
		{
		public:
			SoundCaptureSampler() noexcept;

			void create(word channel, uint samplingSize, uint delta) noexcept;
			void remove() noexcept;

			const sample * read() noexcept;

		private:
			Array<byte> m_buffer;
			uint m_samplingSize;
			Locked m_locked;
			uint m_lockRead;
			uint m_delta;
			uint m_deltaLeft;
		};

		class DFT
		{
		public:
			DFT() noexcept;
			~DFT() noexcept;

			void create(uint samplingCount) noexcept;
			void remove() noexcept;

			const complex* process(const sample * input) noexcept;

		private:
			complex * m_result;
			uint m_samplingCount;

		};

		class FFT
		{
		public:
			FFT() noexcept;
			~FFT() noexcept;

			void create(uint samplingCount) noexcept;
			void remove() noexcept;

			void put(const sample* input) noexcept;
			const complex * fft() noexcept;

		private:
			byte * m_buffer;
			complex * m_trifunc;
			complex * m_result;
			complex * m_temp;
			uint m_rounds;
			uint m_samplingCount;

			void _split(complex * even, complex * odd, const complex * src, uint count) noexcept;
			void _fft(complex * src, uint count) noexcept;

		};

		class FFTW
		{
		public:
			FFTW() noexcept;
			~FFTW() noexcept;

			void create(uint samplingCount) noexcept;
			void createInverse(uint samplingCount) noexcept;
			void remove() noexcept;

			void put(const sample* input) noexcept;
			void put(const float* input) noexcept;
			void put(const complex* input) noexcept;
			const complex * fft() noexcept;
			const complex * scale(float scale) noexcept;
			const complex * get() noexcept;
			complex * getInput() noexcept;
			uint getSamplingCount() noexcept;

		private:
			void _alloc(uint samplingCount, int sign) noexcept;
			fftwf_plan m_plan;
			byte * m_buffer;
			complex * m_in, * m_out;
			uint m_samplingCount;
		};

		class STFTCapture
		{
		public:
			STFTCapture() noexcept;
			~STFTCapture() noexcept;

			void create(uint samplingCount, uint delta) noexcept;
			void remove() noexcept;
			bool skip() noexcept;
			const complex * process() noexcept;

		private:
			SoundCaptureSampler m_sampler;
			FFTW m_fft;
		};
	}
}
