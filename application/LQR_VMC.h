#ifndef __LQR_H
#define __LQR_H

void lqr_k(double L0, double K[12]);
void VM_cal(double F0, double Tp, double phi1, double phi4, double *Tp1,
            double *Tp2);
void VM_cal1(double F0, double Tp, double phi1, double phi4, double *Tp1,
            double *Tp2);
void get_legparam(double phi1, double phi4, double *L0, double *phi0);
double get_dphi0(double phi_1, double phi_4, double d_phi1, double d_phi4);






#endif
