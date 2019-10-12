#include "tape.h"

Tape::Tape(std::string pFilePath, int tapeSize)
{
	cFile.open(pFilePath, std::fstream::in | std::fstream::out | std::fstream::trunc);
	std::locale myLocale("");
	cFile.imbue(myLocale);
	if (!cFile.is_open())
	{
		std::cerr << "Tape could not be opened." << std::endl;
		return;
	}
	while (tapeSize--)
	{
		std::cout << " tape1 " << cFile.tellg() << std::endl;
		cFile << EMPTY_RECORD;
	}
	cFile.clear();
	cFile.seekg(0, std::ios::beg);

	std::cout << " tape2 " << cFile.tellg() << std::endl;
}

bool Tape::nextRecord()
{
	if (!cFile.is_open())
	{
		std::cerr << "Record could not be read. Tape could not be opened." << std::endl;
		return false;
	}

	if (cFile.tellg() == 0 && cFile.peek() == std::ifstream::traits_type::eof())
	{
		return false;
	}

	std::string throwaway;
	std::cout << " next1 " << cFile.tellg() << std::endl;
	std::getline(cFile, throwaway); //The getline function moves the pointer by itself
	std::cout << " next2 " << cFile.tellg() << std::endl;
	if (cFile.peek() == std::ifstream::traits_type::eof())
	{
		cFile.clear();
		//cFile.peek(); This here does NOT break the first write
		cFile.seekg(0, std::ios::beg);
		std::cout << "  next00 " << cFile.good() << std::endl;
		//cFile.peek(); //This here breaks even the first write, clear does not help
		std::cout << "  next01 " << cFile.good() << std::endl;
	}
	/*else
	{
		cFile.peek();  This here does NOT break the first write
	}*/
	std::cout << " next3 " << cFile.tellg() << std::endl;

	return true;
}

bool Tape::clearRecord()
{
	return writeRecord(EMPTY_RECORD);
}

bool Tape::writeRecord(double pI, double pR)
{
	std::stringstream stream;
	stream << std::scientific << pI;
	int spaceCountNeeded = 40 - stream.str().length();
	while (spaceCountNeeded--)
	{
		stream << " ";
	}
	stream << std::scientific << pR;
	spaceCountNeeded = 80 - stream.str().length();
	while (spaceCountNeeded--)
	{
		stream << " ";
	}
	stream << "\n";

	std::string record = stream.str();
	return writeRecord(record);
}

bool Tape::writeRecord(std::string pOutput)
{
	if (!cFile.is_open())
	{
		std::cerr << "Record could not be read. Tape could not be opened." << std::endl;
		return false;
	}

	std::cout << " write0 " << cFile.good() << std::endl;

	std::cout << " write1 " << cFile.tellg() << std::endl;


	cFile << pOutput;

	std::cout << " write2 " << cFile.tellg() << std::endl;

	/*
		<< moves the stream pointer to the next line.
		That is undesired and thus needs to be reverted.
		Negative value for seekg moves the cursor back to the start of the line.
	*/
	cFile.seekg(-(pOutput.length() + 1), std::ios::cur);
	cFile << std::flush;

	std::cout << " write3 " << cFile.tellg() << std::endl;

	return true;
}

bool Tape::readRecord(std::string& pOutput)
{
	if (!cFile.is_open())
	{
		std::cerr << "Record could not be read. Tape could not be opened." << std::endl;
		return false;
	}

	std::cout << " read1 " << cFile.tellg() << std::endl;

	std::getline(cFile, pOutput);
	/* 
		std::getline moves the stream pointer to the next line. 
		That is undesired and thus needs to be reverted. 
		Negative value for seekg moves the cursor back to the start of the line.
	*/
	cFile.seekg(-((int)pOutput.length() + 2), std::ios::cur);

	std::cout << " read2 " << cFile.tellg() << std::endl;

	return true;
}

const std::string Tape::EMPTY_RECORD = "                                                                                \n";

Tape::~Tape()
{
	cFile.close();
}
