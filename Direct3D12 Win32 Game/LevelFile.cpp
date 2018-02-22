#include "pch.h"
#include "LevelFile.h"

void LevelFile::read(std::string file, std::string ext)
{
	level_file = std::make_unique<File>(file, ext, true);

	level_file->read();

}
