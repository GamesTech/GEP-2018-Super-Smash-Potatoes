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
	std::vector<std::string> block;

	while (std::getline(inFile, line)) {
		if (line == "{")
		{
			continue;
		}
		else if (line == "}")
		{
			contents.push_back(block);
			block.clear();
		}
		else
		{
			block.push_back(line);
		}
	}

	inFile.close();
}

std::vector<std::string> File::getBlock(int index) const
{
	return contents.at(index);
}

void File::addBlock(std::vector<std::string>& block)
{
	contents.push_back(block);
}
