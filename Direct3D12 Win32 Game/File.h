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

	std::vector<std::string> getBlock(int index) const;
	void addBlock(std::vector<std::string> & block);
private:
	std::ifstream inFile;

	std::string path;
	std::vector<std::vector<std::string>> contents;
};

