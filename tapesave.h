#pragma once
#include<fstream>

class TapeSave {
	public:
		TapeSave();
		TapeSave(char* pBuffer, int pBufferByteSize, int pBufferCount, int pMaxBufferSize, int pBufferSize, bool pHasRead, int pDiscReadCount, int pDiscWriteCount, std::streampos pPosition);

		void set(char* pBuffer, int pBufferByteSize, int pBufferCount, int pMaxBufferSize, int pBufferSize, bool pHasRead, int pDiscReadCount, int pDiscWriteCount, std::streampos pPosition);

		char* cBuffer;
		int cBufferCount;
		int cMaxBufferSize;
		int cBufferSize;
		bool hasRead;
		int cDiscReadCount;
		int cDiscWriteCount;
		std::streampos savedPosition;

		~TapeSave();
};