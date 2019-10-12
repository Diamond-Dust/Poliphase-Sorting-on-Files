#pragma once
#include<string>
#include<sstream>
#include<limits>

static class Record
{
	public:
		/* Transform the record string to its sortable value */
		static double getValue(std::string pRecord);

	private:
		/* Task 46. Given current and resistance, sort by power */
		static double computePower(double& pI, double& pR);
		static double computePower(std::pair<double, double> pIR);
		/* Separate record string into computable values */
		static std::pair<double, double> separateValues(std::string pRecord);
		
};