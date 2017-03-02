#include "FileReading.h"
#include <fstream>
#include <iostream>

FileReading *FileReading::file = NULL;
//FileReading is using Singleton pattern, so only one instance 
//of FileReading is created throughout the whole game. 

FileReading *FileReading::getInstance()
{
	if (file == NULL)
		file = new FileReading();
	return file;
}

FileReading::~FileReading()
{
	delete file;
}

std::string FileReading::getWords(int ID)
{
	return words[ID];
}
void FileReading::readFiles()
{
	for (int i = 0; i < totalFiles; i++)
	{
		int ID = 0;
		std::string text;

		if (i == 0)
		{
			std::ifstream file("Text File//Shooting1.txt");
			while (std::getline(file, text))
			{
				words[i] += text;
				words[i].push_back('\n');
			}
		}
		else if (i == 1)
		{
			std::ifstream file("Text File//Shooting2.txt");
			while (std::getline(file, text))
			{
				words[i] += text;
				words[i].push_back('\n');
			}
		}
		else if (i == 2)
		{
			std::ifstream file("Text File//Shooting3.txt");
			while (std::getline(file, text))
			{
				words[i] += text;
				words[i].push_back('\n');
			}
		}

		else if (i == 3)
		{
			std::ifstream file("Text File//Shooting4.txt");
			while (std::getline(file, text))
			{
				words[i] += text;
				words[i].push_back('\n');
			}
		}
		else if (i == 4)
		{
			std::ifstream file("Text File//Abilities.txt");
			while (std::getline(file, text))
			{
				words[i] += text;
				words[i].push_back('\n');
			}
		}

	}

}