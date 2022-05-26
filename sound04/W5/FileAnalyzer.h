#pragma once
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <assert.h>
#include <vector>
#include <iostream>
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

	friend class AudioFile;

public:
	static FileAnalyzer* Instance()
	{
		static FileAnalyzer f;
		return &f;
	}
	AudioFile Analyze(std::string file_path);


};

class AudioFile
{
	friend class FileAnalyzer;
	unsigned int file_size;
	WAVEFORMATEX wfx;
	void* frequency;
public:
	AudioFile() {};
	AudioFile(std::string file_path) { *this = FileAnalyzer::Instance()->Analyze(file_path); }
	WAVEFORMATEX* GetFormat() { return &wfx; }
	unsigned int GetFileSize() { return file_size; }
	void* GetData() { return frequency; }

};