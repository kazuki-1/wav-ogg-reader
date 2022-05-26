#pragma once
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <assert.h>
#include <vector>
#include <iostream>
#include <ogg/ogg.h>
#include <ogg/os_types.h>
#include <vorbis/vorbisfile.h>	

#define FOUR_BYTE sizeof(byte) * 4
#define TWO_BYTE sizeof(byte) * 2
typedef unsigned int uint32;
const UINT RIFF_TAG{ 'FFIR' }, FMT_TAG{ ' tmf' }, DATA_TAG{ 'atad' };

class AudioFile;
class FileAnalyzer
{
	FileAnalyzer() {};
	struct Datasize
	{
		UINT tag{}, size;
	};
	struct WaveChunk {
		UINT tag;
		UINT name;
		UINT size;
	} ;
	struct RIFFHeader
	{
		UINT tag{}, size, wave_header;
	};
	struct FMTHeader
	{
		UINT tag{}, byte_size{};
		WAVEFORMATEX wfx{};
	};
	int file_size{};
	char* file_data;
	std::vector<WaveChunk>chunks;
	bool SearchTag(UINT tag, char* data)
	{
		void* d = (void*)data;
		UINT* t = (UINT*)d;
		if (*t == tag)
			return true;
		return false;
	}
	template <class T>
	T* LoadChunk(UINT tag)
	{
		while (file_data != nullptr)
		{
			char* data_tag = (char*)file_data;
			if (SearchTag(tag, data_tag))
			{
				T* output = (T*)file_data;
				file_data += sizeof(T);
				return output;

			}
			++file_data;
		}
		assert("Chunk not found");
	}

	std::shared_ptr<AudioFile>AnalyzeOGG(std::string file_path);
	std::shared_ptr<AudioFile>AnalyzeWAV(std::string file_path);

	friend class AudioFile;

public:
	static FileAnalyzer* Instance()
	{
		static FileAnalyzer f;
		return &f;
	}
	std::shared_ptr<AudioFile> Analyze(std::string file_path);


};

class AudioFile
{
	friend class FileAnalyzer;
	

protected:
	void* data;
	unsigned int file_size;

public:
	float pitch{ 1.0f };


	AudioFile() {};
	int GetFileSize() { return file_size; }
	virtual int GetChannels() { return {}; }
	virtual int GetSamplingRate() { return {}; }
	virtual int GetBitRate() { return {}; }
	void* GetData() { return data; }

};


class WAV_Audio : public AudioFile
{
	WAVEFORMATEX wfx;
	friend class FileAnalyzer;
	
public:
	
	WAV_Audio() {};
	WAVEFORMATEX* GetFormat() { return &wfx; }
	unsigned int GetFileSize() { return file_size; }
	int GetChannels() override { return wfx.nChannels; }
	int GetSamplingRate() override { return wfx.nSamplesPerSec; }
	int GetBitRate() override { return wfx.wBitsPerSample; }

};


class OGG_Audio : public AudioFile
{
	friend class FileAnalyzer;
	OggVorbis_File ogg;
	vorbis_info* info{};
	int bitRate{ 16};
public:
	OGG_Audio() {};
	~OGG_Audio() { ov_clear(&ogg); }
	int GetChannels() override { return info->channels; }
	int GetSamplingRate() override { return info->rate; }
	int GetBitRate() override { return bitRate; }
};