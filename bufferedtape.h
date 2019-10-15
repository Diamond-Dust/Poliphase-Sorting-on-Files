#pragma once
#include "tape.h"

class BufferedTape {
	public:
	BufferedTape();
	BufferedTape(int pTapeSize, int pBufferSize);

	/* Clear current record. Leaves an empty line in its place, to simulate an empty space on the tape. If the tape does not exist, return false. */
	bool clearRecord();
	/* Insert a record as the current line. Overwrites any data there. If the tape does not exist, return false. Go to the next line. */
	bool writeRecord(double pI, double pR);
	/* Read a record on the current line. If the tape does not exist, return false. Go to the next line.*/
	bool readRecord(std::string& pOutput);
	/* Flushes the filestream. Can cause errors in writing further into the file. */
	void flush();

	~BufferedTape();
	private:
		char* cBuffer;
		int cBufferCount;
		int cMaxBufferSize;
		int cBufferSize;
		bool hasRead;
		std::fstream cFile;
		char cFileName[L_tmpnam];
		static const int RECORD_LENGTH;
		static const std::string EMPTY_RECORD;
		bool writeRecord(std::string pOutput);
};