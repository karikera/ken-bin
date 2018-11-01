#pragma once

#include <KR3/main.h>

struct IDirectSoundBuffer;
struct IDirectSoundCaptureBuffer;
struct IDirectSoundNotify;
struct OggVorbis_File;

typedef struct tWAVEFORMATEX WAVEFORMATEX;

namespace kr
{

	namespace sound
	{
		constexpr dword secSample = 88200;
		constexpr word sampleBit = 16;
		constexpr word sampleBytes = sampleBit / 8;
		constexpr dword secByte = secSample * sampleBytes;
		constexpr dword maxVolume = (((dword)1) << (sampleBit - 1)) - 1;
		using sample = int_sz_t<sampleBytes>;

		inline dword secToByte(dword x) noexcept
		{
			return x * secByte;
		}

		class Sound;
		class SoundStreamer;
		class SoundCapture;
		class PrimarySound;

		class SoundNotify:public Keep<IDirectSoundNotify>
		{
		public:
			SoundNotify(IUnknown * obj) noexcept;
			bool makeEvent(EventHandle ** out, View<dword> offsets) noexcept;
			bool registEvent(View<EventHandle*> events, View<dword> offsets) noexcept;

			using Keep::Keep;
			using Keep::operator =;
		};

		class Sound:public Keep<IDirectSoundBuffer>
		{
		public:
			struct Init
			{
				Init() noexcept;
				~Init() noexcept;
			};

			using Keep::Keep;
			using Keep::operator =;

			class Locked
			{
			public:
				Locked() noexcept;
				Locked(Locked && other) noexcept;
				Locked(IDirectSoundBuffer * sound, byte * buffer1, dword size1, byte * buffer2, dword size2) noexcept;
				~Locked() noexcept;
				void unlock() noexcept;
				WRefArray<sample> array1() noexcept;
				WRefArray<sample> array2() noexcept;

				operator bool() noexcept;
				bool operator !() noexcept;
				Locked & operator =(Locked && other) noexcept;

				byte * buffer1;
				dword size1;
				byte * buffer2;
				dword size2;

				class Iterator
				{
				public:
					Iterator(sample * ptr, sample *next) noexcept;
					Iterator & operator ++() noexcept;
					const Iterator operator ++(int) noexcept;
					bool operator ==(const Iterator & other) noexcept;
					bool operator !=(const Iterator & other) noexcept;
					sample & operator *() noexcept;

				private:
					mutable sample * m_ptr;
					mutable sample * m_next;
				};

				Iterator begin() noexcept;
				Iterator end() noexcept;

			private:
				IDirectSoundBuffer * m_sound;
			};

			Sound() noexcept;
			~Sound() noexcept;

			void remove() noexcept;
			Locked createLock(word channel, dword size) throw(ErrorCode);
			bool createMonotone(dword dwSize, float volume, float frequency) noexcept;
			dword loadFromWaveFile(pcstr16 str) noexcept;
			dword loadFromOggStream(io::VIStream<void> stream) noexcept;
			dword loadFromOggFile(pcstr16 str) noexcept;
			dword loadFromMp3Buffer(Buffer mp3file) noexcept;
			dword loadFromMp3File(pcstr16 str) noexcept;
			dword loadFromFileAuto(pcstr16 str) noexcept;

			bool play() noexcept;
			void stop() noexcept;
			bool loop() noexcept;
			bool dupPlay() noexcept;

		protected:
			void _create(const WAVEFORMATEX * format, dword size) throw(ErrorCode);
			void _create(const WAVEFORMATEX * format, dword size, dword flags) throw(ErrorCode);
			dword _loadOgg(int res, OggVorbis_File * vf) throw(ErrorCode, InvalidSourceException);
		};

		class PrimarySound :public Sound
		{
		public:
			void create() throw(ErrorCode);
		};

		class SoundStreamer : public Sound
		{
		public:
			SoundStreamer() noexcept;
			void create(word channel) noexcept;
			Locked lock();
			Locked lock(dword size);
			Locked lockMoreThan(dword size);

		private:
			dword m_secByte;
			dword m_lastWrite;
		};

		class SoundCapture :public Keep<IDirectSoundCaptureBuffer>
		{
		public:
			class Locked
			{
			public:
				Locked() noexcept;
				Locked(Locked && other) noexcept;
				Locked(IDirectSoundCaptureBuffer * sound, byte * buffer1, dword size1, byte * buffer2, dword size2) noexcept;
				~Locked() noexcept;
				void unlock() noexcept;
				View<sample> array1() noexcept;
				View<sample> array2() noexcept;

				operator bool() noexcept;
				bool operator !() noexcept;
				Locked & operator =(Locked && other) noexcept;

				byte * buffer1;
				dword size1;
				byte * buffer2;
				dword size2;

			private:
				IDirectSoundCaptureBuffer * m_sound;
			};

			using Keep::Keep;
			using Keep::operator =;

			SoundCapture() noexcept;
			~SoundCapture() noexcept;
			void create(word channel) noexcept;
			void remove() noexcept;
			dword skip(dword size) noexcept;
			Locked lock();
			Locked lockMoreThan(dword size);

		private:
			dword m_secByte;
			dword m_lastRead;
		};
	}
}
