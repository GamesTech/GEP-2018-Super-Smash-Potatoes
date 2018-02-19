#pragma once
#include "File.h"

class Debug
{
public:
	
	static void init();
	static void output(std::string error, std::string system);

private:
	static std::unique_ptr<File> file;
};