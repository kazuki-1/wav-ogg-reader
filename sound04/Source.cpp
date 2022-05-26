//M_PI‚ðŽg—p‰Â”\‚É
#define _USE_MATH_DEFINES
#include <memory>
#include <al.h>
#include <alc.h>
#include <cmath>
#include <DirectXMath.h>
#include <iostream>
#include "FileAnalyzer.h"
#define PI DirectX::XM_PI
#include <vector>
#pragma comment(lib, "OpenAL32.lib")



int wrap(int tar, int min, int max)
{
	return tar > max ? min : tar < min ? max : tar;

}
int main(void) 
{
	// OpenAL initialization
	ALCdevice* OALDevice = alcOpenDevice(0);
	ALCcontext* OALContext = alcCreateContext(OALDevice, 0);
	alcMakeContextCurrent(OALContext);
	


	// openAL parameters
	ALuint buffer;
	ALuint source;


	// Buffer Generation
	alGenBuffers(1, &buffer);



	// SourceVoice generation
	alGenSources(1, &source);


	std::shared_ptr<AudioFile>audio = FileAnalyzer::Instance()->Analyze("sound.ogg");

	// Set the format based on the audio file properties
	ALenum format{};
	switch (audio->GetChannels())
	{
	case 1:
	{
		switch (audio->GetBitRate())
		{
		case 8:
			format = AL_FORMAT_MONO8;
			break;
		case 16:
			format = AL_FORMAT_MONO16;
			break;
		}
		break;
	}
	case 2:
	{
		switch (audio->GetBitRate())
		{
		case 8:
			format = AL_FORMAT_STEREO8;
			break;
		case 16:
			format = AL_FORMAT_STEREO16;
			break;
		}
		break;
	}

	}


	// Maps the audioData to the buffer
	alBufferData(buffer, format, audio->GetData(), audio->GetFileSize(), audio->GetSamplingRate());

	// Maps the buffer to the sourceVoice
	alSourcei(source, AL_BUFFER, buffer);

	// Loops the sourceVoice
	alSourcei(source, AL_LOOPING, AL_TRUE);



	alSourcePlay(source);



	std::cout << "Press left to lower pitch \n Press Right to raise pitch\n\n";
	while (1)
	{

		audio->pitch = min(max(audio->pitch, 0.0f), 2.0f);
		if (GetAsyncKeyState(VK_LEFT) & 1)
			audio->pitch -= 0.01f;
		else if (GetAsyncKeyState(VK_RIGHT) & 1)
			audio->pitch += 0.01f;
		alSourcef(source, AL_PITCH, audio->pitch);

		if (GetAsyncKeyState(VK_SPACE))
			break;
	}




	// Stops the sourceVoice
	alSourceStop(source);

	// Deinitialize the parameters
	alDeleteBuffers(1, &buffer);
	alDeleteSources(1, &source);

	// OpenAL deinitialization
	alcDestroyContext(OALContext);
	alcCloseDevice(OALDevice);




	return 0;
}