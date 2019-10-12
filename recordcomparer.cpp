#include "recordcomparer.h"

double Record::getValue(std::string pRecord)
{
	return computePower(
		separateValues(pRecord)
	);
}

double Record::computePower(double& pI, double& pR)
{
	return pI * pI * pR;
}

double Record::computePower(std::pair<double, double> pIR)
{
	/* Incomplete records are sent to the end. */
	if (pIR.first == -1 && pIR.second == -1)
		return INFINITY;
	return computePower(pIR.first, pIR.second);
}

std::pair<double, double> Record::separateValues(std::string pRecord)
{
	std::stringstream stream(pRecord); 
	int foundRecordCount = 0;
	double recordValues[2];
	while (stream >> recordValues[foundRecordCount])
	{
		/* If there are more values in the record, take only first two. */
		foundRecordCount++;
		if (foundRecordCount >= 2)
			break;
	}
	return (foundRecordCount < 2) ? std::pair<double, double>(-1, -1) : std::pair<double, double>(recordValues[0], recordValues[1]);
}
