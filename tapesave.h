#pragma once
#include<fstream>

class TapeSave {
	public:
		TapeSave();
		TapeSave(char* pBuffer, int pBufferByteSize, int pBufferCount, int pMaxBufferSize, int pBufferSize, bool pHasRead, std::streampos pPosition);

		void set(char* pBuffer, int pBufferByteSize, int pBufferCount, int pMaxBufferSize, int pBufferSize, bool pHasRead, std::streampos pPosition);

		char* cBuffer;
		int cBufferCount;
		int cMaxBufferSize;
		int cBufferSize;
		bool hasRead;
		std::streampos savedPosition;

		~TapeSave();
};