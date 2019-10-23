#pragma once
#include "tape.h"
#include "tapesave.h"

class BufferedTape {
	public:
	BufferedTape();
	BufferedTape(int pTapeSize, int pBufferSize);

	/* Clear current record. Leaves an empty line in its place, to simulate an empty space on the tape. If the tape does not exist, return false. */
	bool clearRecord();
	/* Insert a record as the current line. Overwrites any data there. If the tape does not exist, return false. Go to the next line. */
	bool writeRecord(double pI, double pR);
	bool writeRecord(std::string pOutput);
	/* Read a record on the current line. If the tape does not exist, return false. Go to the next line.*/
	bool readRecord(std::string& pOutput);
	/* Flushes the filestream. Can cause errors in writing further into the file. */
	void flush();
	/* Clears the entire file. */
	void clear();
	/* Rewind the tape back to the start. */
	void rewind();
	/* Remember where you are, so that printout can happen */
	void printStart();
	/* Restore the state from before the printout */
	void printEnd();

	~BufferedTape();
	private:
		char* cBuffer;
		int cBufferCount;
		int cMaxBufferSize;
		int cBufferSize;
		bool hasRead;
		std::fstream cFile;
		char cFileName[L_tmpnam];

		TapeSave cSave{};

		static const int RECORD_LENGTH;
		static const std::string EMPTY_RECORD;
};