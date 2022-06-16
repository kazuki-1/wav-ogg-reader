#pragma once
#include <al.h>
#include <alc.h>
#include <iostream>
#define SAMPLE_RATE 44100

class SoundCapturer
{
	ALCdevice* micDevice;
	ALshort* buffer;
	ALshort* output;
	float record_time{ 5.0f };		// In seconds
	bool capturing{};
	int size{};
public:
	SoundCapturer() {};
	~SoundCapturer() { if (buffer) delete[] buffer; }
	static SoundCapturer* Instance()
	{
		static SoundCapturer f;
		return &f;
	}
	void Initialize()
	{
		micDevice = alcCaptureOpenDevice(0, SAMPLE_RATE, AL_FORMAT_MONO16, SAMPLE_RATE * record_time);
	}
	void Finalize()
	{
		alcCloseDevice(micDevice);
	}
	bool BeginCapture(float time = 5.0f)
	{
		record_time = time;
		size = SAMPLE_RATE * record_time;
		if (!capturing)
		{
			if (buffer)
				delete[] buffer;
			buffer = new ALshort[size];
			alcCaptureStart(micDevice);
			capturing = true;
			std::cout << "Begin Recording...\n";
		}
		{
			ALCint sample{};
			while (sample < size)
			{
				alcGetIntegerv(micDevice, ALC_CAPTURE_SAMPLES, sizeof(ALCint), &sample);
			}
			alcCaptureStop(micDevice);
			std::cout << "Recording ended\n";
		}
		capturing = false;
		return true;

	}
	ALshort* GetBuffer()
	{
		alcCaptureSamples(micDevice, buffer, size);
		return buffer;
	}
	ALshort* GetReversedBuffer()
	{
		if (output)
			delete[] output;
		output = new ALshort[size];
		int ind{  };
		while ( ind < size )
		{
			output[ind] = buffer[size - ind];
			++ind;
		}
		return output;


	}
	int GetSize()
	{
		return SAMPLE_RATE * record_time * sizeof(ALshort);
	}

};