#pragma once
#include<iostream>
#include<vector>
#include<string>

class CSVReader
{
    std::string fileName;
    std::string delimeter;
    
public:
    CSVReader(std::string filename, std::string delm = ",");
    std::vector<std::string> getData();
	void getData(std::vector<double>& p, std::vector<double>& r, std::string& pro_name, double& AP, double& Binder, double& Alu);
};