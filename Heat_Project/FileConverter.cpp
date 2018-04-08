#include<iostream>
#include<string>
#include<fstream>
#include<iterator>
#include<algorithm>
#include<vector>
//#include<boost/algorithm/string.hpp>
#include"FileConverter.h"
using namespace std;

CSVReader::CSVReader(std::string filename, std::string delimeter)
{
	this->fileName = filename;
	this->delimeter = delimeter;
}

void CSVReader::getData(std::vector<double>& p, std::vector<double>& r, std::string& pro_name, double& AP, double& Binder, double& Alu)
{
	std::ifstream file(CSVReader::fileName);
	std::vector<std::vector<std::string>> dataList;
	/*checking file open*/
	if (file.is_open()) {
		cout << this->fileName<<" is opened!" << endl;
	}
	else
	{
		cout << "File open failed!" << endl;
		return;
	}



	std::string line = "";
	getline(file, line);
	pro_name = line.substr(0, line.find(this->delimeter));

	/*one line skip*/
	getline(file, line);
	size_t pos = 0;
	getline(file, line);

	pos = line.find(this->delimeter);
	p.push_back(std::stod(line.substr(0, pos)));
	line.erase(0, pos + this->delimeter.length());
	
	pos = line.find(this->delimeter);
	r.push_back(std::stod(line.substr(0, pos)));
	line.erase(0, pos + this->delimeter.length());
	
	pos = line.find(this->delimeter);
	AP = std::stod(line.substr(0, pos));
	line.erase(0, pos + this->delimeter.length());

	pos = line.find(this->delimeter);
	Binder = std::stod(line.substr(0, pos));
	line.erase(0, pos + this->delimeter.length());

	Alu = std::stod(line);
	
	while (getline(file, line))
	{
		pos = line.find(this->delimeter);
		p.push_back(std::stod(line.substr(0, pos)));
		line.erase(0, pos + this->delimeter.length());
		r.push_back(std::stod(line));
	}

}

std::vector<std::string> CSVReader::getData()
{
	std::vector<std::string> dataList;
	std::ifstream file(this->fileName);

	std::string line = "";

	while (getline(file, line))
	{
		dataList.push_back(line);
	}
	return dataList;
}