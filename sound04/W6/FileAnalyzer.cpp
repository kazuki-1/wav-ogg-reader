#include "FileAnalyzer.h"
#include <filesystem>
/*----------------------------------------------------------FileAnalyzer------------------------------------------------------*/
/*----------------------------------------------------------FileAnalyzer Analyze()--------------------------------------------*/

std::shared_ptr<AudioFile> FileAnalyzer::Analyze(std::string file_path)
{
	std::filesystem::path path(file_path);
	std::string format = path.extension().string();

	if (format == ".wav")
		return AnalyzeWAV(file_path);
	else if (format == ".ogg")
		return AnalyzeOGG(file_path);

}

/*----------------------------------------------------------FileAnalyzer AnalyzeOGG()--------------------------------------------*/

std::shared_ptr<AudioFile> FileAnalyzer::AnalyzeOGG(std::string file_path)
{
	std::shared_ptr<OGG_Audio>output = std::make_shared<OGG_Audio>();


	if (ov_fopen(file_path.c_str(), &output->ogg) < 0)
		assert("Failed to open OBB file");

	output->info = ov_info(&output->ogg, -1);
	output->file_size = ov_pcm_total(&output->ogg, -1);
	
	int bitstream{ -1 };
	long cur_byte{};
	char* obb_data = new char[output->file_size];
	while (cur_byte < output->file_size)
		cur_byte += ov_read(&output->ogg, obb_data + cur_byte, output->file_size - cur_byte, 0, 2, 1, &bitstream);

	


	output->data = (void*)obb_data;
	
	return output;
}

/*----------------------------------------------------------FileAnalyzer AnalyzeWAV()--------------------------------------------*/

std::shared_ptr<AudioFile> FileAnalyzer::AnalyzeWAV(std::string file_path)
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
	FMTHeader* fmt = LoadChunk<FMTHeader>(FMT_TAG);
	Datasize* ds = LoadChunk<Datasize>(DATA_TAG);

	std::shared_ptr<WAV_Audio>output = std::make_shared<WAV_Audio>();
	output->wfx = output->wfx;
	output->data = (void*)file_data;
	output->file_size = ds->size;
	return output;

}