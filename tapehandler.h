#pragma once
#include<string>

class TapeHandler
{
	public:
	TapeHandler();
	TapeHandler(std::string pBasePath);
	TapeHandler(int pTapeCount);
	TapeHandler(std::string pBasePath, int pTapeCount);
	TapeHandler(std::string pBasePath, std::string pOriginalTapeName, int pTapeCount);

	void setBasePath(std::string pBasePath);
	void setOriginalTapeName(std::string pOriginalTapeName);
	void setTapeCount(int pTapeCount);

	std::string getBasePath();
	std::string getOriginalTapeName();
	int			getTapeCount();

	private:
		~TapeHandler();
};
