#include "pch.h"
#include "File.h"

File::File(std::string path, bool read_only) : File(path, ".ssp", read_only)
{
}

File::File(std::string path, std::string ext, bool read_only)
{
	this->path = path;
	this->ext = ext;
}

bool File::read()
{
	inFile.open(path + ext);
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

	return true;
}

bool File::write()
{
	outFile.open(path + ext);
	if (!outFile) {
		//Debug out?
		return false;
	}

	for (int y = 0; y < contents.size(); y++)
	{
		outFile << "{\n";
		for (int x = 0; x < contents.at(y).size(); x++)
		{
			outFile << contents.at(y).at(x) + "\n";
		}
		outFile << "}\n";
	}

	outFile.close();

	return true;
}

std::vector<std::string> File::getBlock(int index) const
{
	return contents.at(index);
}

void File::addBlock(std::vector<std::string>& block)
{
	contents.push_back(block);
}
