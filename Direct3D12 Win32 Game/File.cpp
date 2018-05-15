#pragma once
#include "pch.h"
#include "File.h"
#include <sstream>

File::File(std::string path, bool read_only) : File(path, ".ssp", read_only)
{
}

File::File(std::string path, std::string ext, bool read_only)
{
	this->path = path;
	this->ext = ext;
	this->read_only = read_only;
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
			int index = line.find(':');
			string new_line = line.substr(index + 1, line.size());
			block.push_back(new_line);
		}
	}

	inFile.close();

	return true;
}

bool File::write()
{
	if (read_only)
	{
		//Debug out READ ONLY FILE
		return false;
	}
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

const std::vector<std::string>& File::getBlock(int index) const
{
	return contents.at(index);
}

int File::getNumberOfBlocks() const
{
	return contents.size();
}

void File::addBlock(std::vector<std::string>& block)
{
	contents.push_back(block);
}

Vector2 File::parseVector2(std::string s)
{
	int index = s.find(',');
	float a = std::stof(s.substr(0, index));
	float b = std::stof(s.substr(index + 1, s.size()));
	return Vector2(a, b);
}

bool File::parseBoolean(std::string s)
{
	if (s == "true")
	{
		return true;
	}
	else if (s == "false")
	{
		return false;
	}
}
