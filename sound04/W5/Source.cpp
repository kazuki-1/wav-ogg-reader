//M_PI���g�p�\��
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
	//OpenAL�̏�����
	ALCdevice* OALDevice = alcOpenDevice(0);
	ALCcontext* OALContext = alcCreateContext(OALDevice, 0);
	
	alcMakeContextCurrent(OALContext);



	//�o�b�t�@(�ۑ��̈�)�ƃ\�[�X(����)��錾
	ALuint buffer;
	ALuint source;



	//�o�b�t�@�𐶐�
	alGenBuffers(1, &buffer);



	//�\�[�X�𐶐�
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

	//�o�b�t�@�ɉ����f�[�^������

	alBufferData(buffer, format, file.GetData(), file.GetFileSize(), 44100);
	//�\�[�X�Ƀo�b�t�@��ݒ�
	alSourcei(source, AL_BUFFER, buffer);

	//���[�v�Đ���ON
	alSourcei(source, AL_LOOPING, AL_TRUE);



	//�\�[�X���Đ��I
	alSourcePlay(source);



	//�����ňꎞ��~�i��ʑJ�ڂ���~�ŁA���͖葱����j
	system("PAUSE");

	//�Đ����~
	alSourceStop(source);

	//���I�m�ۂ����������̔j��


	// �o�b�t�@�̔j��
	alDeleteBuffers(1, &buffer);


	// �\�[�X�̔j��
	alDeleteSources(1, &source);


	//OpenAL�̔j��
	alcDestroyContext(OALContext);
	alcCloseDevice(OALDevice);




	return 0;
}