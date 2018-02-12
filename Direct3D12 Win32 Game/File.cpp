#include "pch.h"
#include "File.h"

File::File(std::string path, bool read_only)
{
	this->path = path;
}


File::~File()
{
}

bool File::read()
{
	
	inFile.open(path);
	if (!inFile) {
		//Debug out?
		return false;
	}

	std::string line;

	while (std::getline(inFile, line)) {
		contents.push_back(line);
	}

	inFile.close();
}

std::string File::getLine(int index) const
{
	return contents.at(index);
}
