//M_PIを使用可能に
#define _USE_MATH_DEFINES
#include <memory>
#include <al.h>
#include <alc.h>
#include <cmath>
#include <DirectXMath.h>
#include <iostream>
#include "../Sound_03.h"
#define PI DirectX::XM_PI
#include <vector>
#pragma comment(lib, "OpenAL32.lib")


AudioFile FileAnalyzer::Analyze(std::string file_path)
{
	if (file_data)
		delete[] file_data;
	FILE* file{};
	fopen_s(&file, file_path.c_str(), "r");
	fseek(file, 0, SEEK_END);
	long size = ftell(file);

	file_data = new char[size];
	fseek(file, 0, SEEK_SET);
	fread(file_data, 1, size, file);

	RIFFHeader* header = LoadChunk<RIFFHeader>(RIFF_TAG);

	FMTHeader* output = LoadChunk<FMTHeader>(FMT_TAG);
	Datasize* ds{};
	ds = LoadChunk<Datasize>(DATA_TAG);


	AudioFile out;
	out.wfx = output->wfx;
	out.frequency = (void*)file_data;
	out.file_size = ds->size;
	return out;
}





int wrap(int tar, int min, int max)
{
	return tar > max ? min : tar < min ? max : tar;

}



int main(void) {
	//OpenALの初期化
	ALCdevice* OALDevice = alcOpenDevice(0);
	ALCcontext* OALContext = alcCreateContext(OALDevice, 0);
	
	alcMakeContextCurrent(OALContext);



	//バッファ(保存領域)とソース(音源)を宣言
	ALuint buffer;
	ALuint source;



	//バッファを生成
	alGenBuffers(1, &buffer);



	//ソースを生成
	alGenSources(1, &source);


	//WAVEFORMATEX wfx{};
	AudioFile file = AudioFile("mario.wav");


	ALenum format{};
	switch (file.GetFormat()->nChannels)
	{
	case 1:
	{
		switch (file.GetFormat()->wBitsPerSample)
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
		switch (file.GetFormat()->wBitsPerSample)
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

	//バッファに音源データを入れる

	alBufferData(buffer, format, file.GetData(), file.GetFileSize(), 44100);
	//ソースにバッファを設定
	alSourcei(source, AL_BUFFER, buffer);

	//ループ再生をON
	alSourcei(source, AL_LOOPING, AL_TRUE);



	//ソースを再生！
	alSourcePlay(source);



	//ここで一時停止（画面遷移が停止で、音は鳴り続ける）
	system("PAUSE");

	//再生を停止
	alSourceStop(source);

	//動的確保したメモリの破棄


	// バッファの破棄
	alDeleteBuffers(1, &buffer);


	// ソースの破棄
	alDeleteSources(1, &source);


	//OpenALの破棄
	alcDestroyContext(OALContext);
	alcCloseDevice(OALDevice);




	return 0;
}