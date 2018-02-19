#include "pch.h"
#include "Debug.h"

std::unique_ptr<File> Debug::file;

void Debug::init()
{
	file = std::make_unique<File>("debug", ".log", false);
}

void Debug::output(std::string error, std::string system)
{
	file->read();
	//todo time stamp
	std::vector<std::string> block;
	block.push_back("Error: " + error);
	block.push_back("System: " + system);
	file->addBlock(block);
	file->write();
}
