#pragma once
#include<iomanip>
#include<string>
#include<sstream>
#include<fstream>
#include<iostream>

/*
	Tape is a text file comprised of many records.
	Every record is exactly 80 ASCII chars long + a single newline character '\r\n' to act as delimiter.
	Record can be empty or have two numbers, Current [I] and Resistance [R], represented as double strings, in that order.
*/

//Ma byc buforowanie o stalym, duzym? buforze. Wczytujemy/zapisujemy kilka rekordow naraz, potem dopiero wyjmujemy pojedyncze rekordy.

//Nie uzywamy rozkladu Fibonacciego. Zamiast tego:
//Zaczyna my z X pustymi, jedna z nich zostanie ciagle pusta
//Do pierwszej zapisujemy 1 rekord
// Nie ruszamy pierwszej tasmy, do reszty dodajemy tyle rekordow, ile jest na pierwszej tasmie
// Nie ruszamy drugiej tasmy,  do reszty dodajemy tyle rekordow, ile jest na drugiej tasmie
//Powtorz az nie dojdziesz do ostatniej tasmy

//Akcja zapisu lub odczytu ma automatycznie przejsc dalej

class Tape
{
	public:
		Tape(std::string pFilePath, int tapeSize);
		/* Try to roll tape forward to the next record. If the tape ends, loop around. If the tape is empty, return false. */
		bool nextRecord();
		/* Clear current record. Leaves an empty line in its place, to simulate an empty space on the tape. If the tape does not exist, return false. */
		bool clearRecord();
		/* Insert a record as the current line. Overwrites any data there. If the tape does not exist, return false. */
		bool writeRecord(double pI, double pR);
		/* Read a record on the current line. If the tape does not exist, return false. */
		bool readRecord(std::string& pOutput);
		~Tape();

	private:
		std::fstream cFile;
		static const std::string EMPTY_RECORD;
		bool writeRecord(std::string pOutput);
};