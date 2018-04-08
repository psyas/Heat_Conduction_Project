#pragma once
#ifndef GLOBAL_VARIABLES
#define GLOBAL_VARIABLES

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


#endif