#pragma once

#include <iostream>

enum ErrorType
{
	NO_ERROR,
	IO_ERROR,
	SHADER_ERROR,
};

class Debug
{
public:

	static void fatalError(char* msg)
	{
		std::cout << "The program has encountered a fatal error and must close, message:" << std::endl;
		std::cout << msg << std::endl;
		std::cout << "Press any key to exit" << std::endl;
		std::cin.get();
		exit(1);
	}

	static void error(char* msg)
	{
		std::cout << "The program has encountered a non-fatal error and may continue, message:" << std::endl;
		std::cout << msg << std::endl;
	}

};
