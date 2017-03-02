#ifndef FILEREADING_H
#define FILEREADING_H
#include <string>

class FileReading
{
public:
	~FileReading();
	static FileReading *getInstance();

	std::string getWords(int);
	void readFiles();

private:
	FileReading() {}
	std::string words[24];
	int totalFiles = 4; //Change this value if you add in more files
	static FileReading *file;

};



#endif
