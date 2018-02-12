#pragma once
#include <fstream>
#include <string>

class File
{
public:
	File(std::string path, bool read_only);
	~File();

	
	bool read(); /* Will return false if file read error */

	bool write();

	std::string getLine(int index) const;
private:
	std::ifstream inFile;

	std::string path;
	std::vector<std::string> contents;
};

