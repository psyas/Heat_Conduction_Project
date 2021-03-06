// One_dim_heat_conduction_product.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include"FileConverter.h"
#include<boost/filesystem.hpp>
#include<boost/range/iterator_range.hpp>

using namespace std;

class Propellant
{
private:
	std::string name;
	double AP;
	double binder;
	double aluminium;
	std::vector<double> p;
	std::vector<double> r;
	double A_opt;
	double B_opt;
	double T_s;
	double T_f;
	std::vector<double> a;
	std::vector<vector<double>> b;
	std::vector<double> b_mean;
	std::vector<double> devi;
	std::vector<double> Tn;
	

public:
	/*default Constructor*/
	Propellant() {
		this->name = "default object";
		this->AP = 0.0;
		this->binder = 0.0;
		this->aluminium = 0.0;
		this->A_opt = 0.0;
		this->B_opt = 0.0;
		this->T_s = 0.0;
		this->T_f = 0.0;
	}
	Propellant(string& name, double AP, double binder, double aluminium, std::vector<double> p, std::vector<double> r)
	{
		/*member variable initialization*/
		this->name = name;
		this->AP = AP;
		this->binder = binder;
		this->aluminium = aluminium;
		std::copy(p.begin(), p.end(), this->p.begin());
		std::copy(r.begin(), r.end(), this->r.begin());

		/*initialize target variables*/
		this->A_opt = 0.0;
		this->B_opt = 0.0;
		this->T_s = 0.0;
		this->T_f = 0.0;

		/*calculate A, B, T_s, T_f*/
		this->calAB();
		this->calTsTf(this->T_s, this->T_f);
	}
	Propellant(std::string fileName)
	{
		CSVReader file(fileName);
		file.getData(this->p, this->r, this->name, this->AP, this->binder, this->aluminium);
		//추진제 이름이 제대로 출력되지 않는 경우 csv파일의 포맷을 확인할 것 (utf-8 x)
		//this->print_status();
		//Fill up the a,b,b_mean,devi
		this->calAB();
		//Fill up the Tn
		this->cal_Tn();

		//find optimal value of A and B
		int min_idx = this->findOptidx();
		this->A_opt = this->a[min_idx];
		this->B_opt = this->b_mean[min_idx];

		this->print_status();
	}

	//return the index of minimum standared deviation
	int findOptidx()
	{
		int idx = 0;
		double min = 999999;
		for (int i = 0; i < this->devi.size(); i++) {
			if (this->devi[i] < min) {
				min = devi[i];
				idx = i;
			}
		}
		return idx;
	}

	//calculate Tn value and fill up the Tn vector
	void cal_Tn()
	{
		for (int i = 0; i < this->a.size(); i++)
		{
			double T = 0.0;
			T = a[i];//A -> T fomula
			this->Tn.push_back(T);
		}
	}

	//print elements of 1-D vector
	void print_vec(std::vector<double> vec)
	{
		vector<double>::iterator it = vec.begin();
		for (; it != vec.end(); ++it) {
			cout << *it << endl;
		}
	}

	//Print status of propellant object
	void print_status()
	{
		cout << "------------------------------" << endl;
		cout << "Status of propellant : " << this->name << endl;
		cout << "Name : " << this->name << endl;
		cout << "AP : " << this->AP << endl;
		cout << "Binder : " << this->binder << endl;
		cout << "Aluminium : " << this->aluminium << endl;
		cout << "optimal A : " << this->A_opt << endl;
		cout << "optimal B : " << this->B_opt << endl;
		cout << "------------------------------" << endl;
	}

	//특정 A값에 해당하는 B값들의 배열을 받아서 표준편차를 반환해주는 함수
	double cal_Standard_devi(vector<double> b)
	{
		double sum = 0.0, mean, standardDeviation = 0.0;
		vector<double>::iterator it = b.begin();
		
		for (; it != b.end(); ++it)
		{
			sum += *it;
		}
		mean = sum / b.size();
		it = b.begin();
		for (; it != b.end(); ++it)
		{
			standardDeviation += pow(*it - mean, 2);
		}

		this->b_mean.push_back(mean);
		return standardDeviation;
	}

	//A를 받아서 p와 r값에 각각 대응하는 B를 계산한다. 그 후 B값들의 표준편차를 devi 배열에 넣는다.
	void calB(double A)
	{
		//p, r배열의 원소를 하나씩 참조하면서 B를 계산, B1 ~ Bn의 표준편차를 구함
		//B 배열을 유지, 표준편차를 반환
		vector<double> temp_b;
		vector<double>::iterator p_it = this->p.begin();
		vector<double>::iterator r_it = this->r.begin();

		double B = 0.0;

		/*Fill the temp B array for certain A value*/
		for (; p_it != p.end(); ++p_it) {
			//how to calculate B from A?
			B = A + *p_it + *r_it;
			temp_b.push_back(B);
			++r_it;
		}

		this->b.push_back(temp_b);
		this->devi.push_back(this->cal_Standard_devi(temp_b));
	}

	/*calculate optimal A and B*/
	void calAB()
	{
		/*Basic property*/
		double G = 1.987;                            // universial gas constant[cal / molK]
		double Cs = 0.33;                            // specific heat of surface[cal / gK]
		double Cg = 0.33;                            // specific heat of gas phase[cal / gK]
		double ap = 0.002;                           // thermal diffusivity[cm ^ 2 / s]
		double Eg = 20 * 10 ^ 3;                     // activation energy of gas phase[cal / mol]
		double Es = 16 * 10 ^ 3;                     // activation energy of surface[cal / mol]
		double Qf = 544.7;                           // gas phase O / F heat release per unit mass[cal / g]
		double Qs = -225 * this->binder + 250 * this->AP - 68 * this->aluminium;  // surface heat release[cal / g]
		double kp = 6.59*(10 ^ -4);                  // thermal conductivity of solid propellant[cal / cmsK]
		double kg = 1.0*(10 ^ -4);                   // thermal conductivity of gas phase[cal / cmsK]

		double Tso = 1000;                           // surface reference temperature[K]
		double Tfo = 2250;                           // flame reference temperature[K](adiabatic flame temp.)
		double Ta = 300;                             // temperature at ambient[K]
		//variable
		double p = 10 ^ 3;                           // pressure[psi]
		double po = 10 ^ 3;                          // reference pressure[psi]
		//variable
		double r = .62;                              // olid propellant burning rate[cm / s]
		double ro = .837;                            // reference burning rate[cm / s]

		/*Target Variables*/
		double A = 0.0;                            // constant in chemical reaction term
		double B = 0.0;                             // constant in diffusion term

		/*non dimensionalization*/
		double P = p / po;                           // non dimensinal pressure
		double H = Qs / (Cs*(Tso - Ta));             // nondimensional surface heat release
		double Q = Qf / (Cg*(Tso - Ta));             // nondimensional flame heat release
		double R = r / ro;                           // nondimensional burning rate               **inaccurate**
													 /*output 1*/
		double Ts = (Es*Tso) / (Tso*G*log(R) + Es);  // actual surface temperature
													 /*output 2*/
		double Tf = Tfo - (50 / 68)*(po - p);        // actual flame temperature
		double Tsred = Ts / Tso;                     // reduced surface temperature
		double Tfred = Tf / Tfo;                     // reduced flame temperature

		for (double a = 0.0; a < 1; a += 0.001) {
			this->a.push_back(a);
			this->calB(a);
		}
	}

	/*calculate optimal T_s, T_f*/
	void calTsTf(double& T_s, double& T_f)
	{
		T_s = 0.1;
		T_f = 0.1;
	}

	/*finally write the information of Propellants to csv files*/
	std::vector<std::vector<std::string>> writeData(std::string filename)
	{
		std::ofstream myfile;
		std::vector<std::vector<std::string>> result;
		myfile.open(filename);
		myfile << "Propellant name\n";
		myfile << "psig,r,AP,Binder,Aluminium,,x,Tn,,Ts,Tf,,A,Bm,σ,,A,B,\n";
		myfile << this->p[0] << "," << this->r[0] << "," << this->AP << "," << this->binder << "," << this->aluminium << "," << "," << this->a[0] << "," << this->Tn[0] << "," << "," << this->T_s << "," << this->T_f << "," << "," << this->a[0] << "," << this->b_mean[0] <<","<<this->devi[0]<<","<<","<<this->A_opt<<","<<this->B_opt<<",\n";
		for (int i = 1; i < this->p.size(); i++) {
			myfile << this->p[i] << "," << this->r[i] << ",,,,," << this->a[i] << "," << this->Tn[i] << ",,,,," << this->a[i] << "," << this->b_mean[i] << "," << this->devi[i] << ",\n";
		}
		for (int i = this->p.size(); i < a.size(); i++) {
			myfile << ",,,,,," << this->a[i] << "," << this->Tn[i] << ",,,,," << this->a[i] << "," << this->b_mean[i] << "," << this->devi[i] << ",\n";
		}
		return result;
	}
};

using namespace boost::filesystem;

int main(int argc, char** argv)
{
	int file_count = 0;
	int result_file_count = 0;
	vector<std::string> file_name;
	/*array of propellant objects*/
	vector<Propellant> propellants;


	std::string input_dir(argv[1]);
	//std::string input_dir = "C:\\Users\\PSY\\source\\repos\\Heat_Project\\input_files";
	boost::filesystem::path input_path(input_dir);

	/*get filenames at certain directory and fill up the propellant vector*/
	if (is_directory(input_path)) {
		std::cout << input_path << " is a directory!"<<endl;
		
		directory_iterator path_it(input_path);
		directory_iterator end_it;
		
		
		while (path_it != end_it) {
			Propellant* temp = new Propellant(path_it->path().string().c_str());
			file_name.push_back(path_it->path().string().c_str());
			propellants.push_back(*temp);
			++path_it;
		}
	}
	//write the information of propellant to csv file
	std::string output_dir(argv[2]);
	//std::string output_dir = "C:\\Users\\PSY\\source\\repos\\Heat_Project\\output_files\\";
	for (int i = 0; i < propellants.size(); i++) {
		std::string file_name = output_dir+"\\"+ "output" + std::to_string(i + 1) + ".csv";
		propellants[i].writeData(file_name);
	}
	
    return 0;
}


