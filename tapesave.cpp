#include "tapesave.h"

TapeSave::TapeSave() 
{
	cBuffer = (char*)malloc(sizeof(char) * 1);
}

TapeSave::TapeSave(char* pBuffer, int pBufferByteSize, int pBufferCount, int pMaxBufferSize,
	int pBufferSize, bool pHasRead, int pDiscReadCount, int pDiscWriteCount, std::streampos pPosition) : cBufferCount(pBufferCount),
	cMaxBufferSize(pMaxBufferSize),  cBufferSize(pBufferSize), hasRead(pHasRead), cDiscReadCount(pDiscReadCount), cDiscWriteCount(pDiscWriteCount), savedPosition(pPosition)
{
	cBuffer = (char*)malloc(sizeof(char) * pBufferByteSize + 1);
	strncpy_s(cBuffer, sizeof(char) * pBufferByteSize + 1, pBuffer, sizeof(char) * pBufferByteSize);
}

void TapeSave::set(char* pBuffer, int pBufferByteSize, int pBufferCount, int pMaxBufferSize, 
	int pBufferSize, bool pHasRead, int pDiscReadCount, int pDiscWriteCount, std::streampos pPosition)
{
	free(cBuffer);
	cBuffer = (char*)malloc(sizeof(char) * pBufferByteSize + 1);
	strncpy_s(cBuffer, sizeof(char) * pBufferByteSize + 1, pBuffer, sizeof(char) * pBufferByteSize);
	cBufferCount = pBufferCount;
	cMaxBufferSize = pMaxBufferSize;
	cBufferSize = pBufferSize;
	hasRead = pHasRead;
	cDiscReadCount = pDiscReadCount;
	cDiscWriteCount = pDiscWriteCount;
	savedPosition = pPosition;
}

TapeSave::~TapeSave()
{
	free(cBuffer);
}
