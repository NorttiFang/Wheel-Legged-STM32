#ifndef __LQR_H
#define __LQR_H

#include "main.h"
#include "Leg_Task.h"
#include "stdbool.h"


void Fitting_K_Calc(float (*fitting_k)[10],float (*p)[6],float L_l,float L_r);
void LQR_Calc(Body_Posture *body,
							Leg_Posture *leg
						 );

//变腿长下多项式拟合的全部系数
extern float P[40][6];
extern float K[4][10];
extern float Fitting_K[4][10];


void VM_cal(double F0, double Tp, double phi1, double phi4, double *Tp1,
            double *Tp2);
void get_legparam(double phi1, double phi4, double *L0, double *phi0);
double get_dphi0(double phi_1, double phi_4, double d_phi1, double d_phi4);



#endif
