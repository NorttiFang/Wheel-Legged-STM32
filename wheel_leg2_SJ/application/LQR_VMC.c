#include "LQR_VMC.h"
#include "arm_math.h"
#include <math.h>
#include "Chassis_Control.h"

extern float		 		YawTotalAngle;

//变腿长下多项式拟合的全部系数
float P[40][6] = 
{ 
		-3.7666,  -13.231,  10.968,  18.777,  -11.667,  -3.0836,
     -45.747,  -49.469,  164.01,  171.44,  -245.86,  -83.924,
     -72.24,  148.92,  -39.408,  -88.758,  75.044,  -3.164,
     -15.282,  9.0254,  -1.0513,  0.27009,  6.5035,  -2.632,
     -93.546,  -396.98,  14.166,  501.42,  15.745,  10.395,
     -5.4675,  -53.723,  0.19177,  18.412,  3.4427,  1.6259,
     -11.741,  44.377,  -138.52,  -25.865,  38.659,  216.91,
     -0.58622,  1.1373,  7.7379,  2.3072,  -16.612,  5.3176,
     -140.55,  524.49,  155.2,  -584.7,  -261.09,  -132.11,
     -12.273,  37.381,  14.627,  -37.996,  -23.081,  -11.12,
     -3.7666,  10.968,  -13.231,  -3.0836,  -11.667,  18.777,
     -45.747,  164.01,  -49.469,  -83.924,  -245.86,  171.44,
     72.24,  39.408,  -148.92,  3.164,  -75.044,  88.758,
     15.282,  1.0513,  -9.0254,  2.632,  -6.5035,  -0.27009,
     -11.741,  -138.52,  44.377,  216.91,  38.659,  -25.865,
     -0.58622,  7.7379,  1.1373,  5.3176,  -16.612,  2.3072,
     -93.546,  14.166,  -396.98,  10.395,  15.745,  501.42,
     -5.4675,  0.19177,  -53.723,  1.6259,  3.4427,  18.412,
     -140.55,  155.2,  524.49,  -132.11,  -261.09,  -584.7,
     -12.273,  14.627,  37.381,  -11.12,  -23.081,  -37.996,
     3.2071,  3.8179,  -21.119,  -23.454,  22.591,  24.832,
     31.228,  -1.2718,  -193.31,  -154.37,  244.06,  227.75,
     -71.241,  -164.02,  -82.544,  343.38,  -151.89,  62.338,
     -15.387,  -9.4507,  -4.8919,  21.595,  -8.9988,  3.9307,
     263.34,  -816.17,  -20.997,  767.35,  147.63,  89.051,
     16.342,  -70.775,  -4.3292,  61.896,  0.48065,  10.626,
     13.277,  -93.887,  -423.55,  187.18,  -93.173,  838.04,
     0.80973,  -3.5194,  -5.5376,  5.3141,  3.1371,  33.801,
     -155.54,  -847.6,  217.53,  1209.4,  26.228,  -103.35,
     -11.942,  -60.776,  12.51,  82.886,  6.1705,  -3.0121,
     3.2071,  -21.119,  3.8179,  24.832,  22.591,  -23.454,
     31.228,  -193.31,  -1.2718,  227.75,  244.06,  -154.37,
     71.241,  82.544,  164.02,  -62.338,  151.89,  -343.38,
     15.387,  4.8919,  9.4507,  -3.9307,  8.9988,  -21.595,
     13.277,  -423.55,  -93.887,  838.04,  -93.173,  187.18,
     0.80973,  -5.5376,  -3.5194,  33.801,  3.1371,  5.3141,
     263.34,  -20.997,  -816.17,  89.051,  147.63,  767.35,
     16.342,  -4.3292,  -70.775,  10.626,  0.48065,  61.896,
     -155.54,  217.53,  -847.6,  -103.35,  26.228,  1209.4,
     -11.942,  12.51,  -60.776,  -3.0121,  6.1705,  82.886

	};

//float P[40][6] = 
//{
//     -4.0073,  -17.057,   16.601,   28.775,  -20.644,  -7.5757,
//     -41.159,   -68.84,   181.15,   215.45,  -269.08,  -112.54,
//     -51.712,   194.66,  -6.7122,  -189.53,   52.149,   7.6001,
//       -11.4,   15.654,   1.0221,  -10.579,   6.7328,  -1.5643,
//      -119.2,   -193.1,   7.4444,   238.53,   27.074,  -6.7125,
//     -8.3057,  -34.994,  0.67133,   4.1288,   8.3417, -0.62979,
//     -4.5442,  -9.6606,  -9.0416,   6.3667,   -36.82,   44.474,
//    -0.18223,  -3.1756,   10.614,   7.5531,  -29.017,  0.30633,
//     -91.901,   416.49,   40.684,  -520.39,  -45.459,  -81.201,
//     -8.8379,   29.586,   7.3572,  -32.157,  -9.0736,   -8.772,
//     -4.0073,   16.601,  -17.057,  -7.5757,  -20.644,   28.775,
//     -41.159,   181.15,   -68.84,  -112.54,  -269.08,   215.45,
//      51.712,   6.7122,  -194.66,  -7.6001,  -52.149,   189.53,
//        11.4,  -1.0221,  -15.654,   1.5643,  -6.7328,   10.579,
//     -4.5442,  -9.0416,  -9.6606,   44.474,   -36.82,   6.3667,
//    -0.18223,   10.614,  -3.1756,  0.30633,  -29.017,   7.5531,
//      -119.2,   7.4444,   -193.1,  -6.7125,   27.074,   238.53,
//     -8.3057,  0.67133,  -34.994, -0.62979,   8.3417,   4.1288,
//     -91.901,   40.684,   416.49,  -81.201,  -45.459,  -520.39,
//     -8.8379,   7.3572,   29.586,   -8.772,  -9.0736,  -32.157,
//     0.41076,   0.8419,  -3.4213,  -7.6029,   5.0344,   5.1295,
//      3.3004,  0.11014,  -23.629,  -39.835,   39.467,   34.463,
//     -51.313,  -42.806,    -31.5,    102.5,  -1.6561,   17.367,
//     -11.559,  -2.4957,  -1.8874,   7.9751,   1.4463,   1.0672,
//      69.364,  -267.27,  -4.1807,   307.95,   11.713,   15.112,
//      5.0117,  -36.272,  -1.4017,   34.556,  -2.4641,   2.6605,
//    -0.56391,   10.352,  -87.882,   11.586,  -16.597,   161.99,
//    -0.13634,   2.1042,   9.0623,  -2.1123,   1.7812,  0.21494,
//     -95.995,  -165.14,   55.305,   238.88,  -25.948,  -14.984,
//     -8.1701,   -12.32,   3.9106,     16.3,   -1.028, -0.35333,
//     0.41076,  -3.4213,   0.8419,   5.1295,   5.0344,  -7.6029,
//      3.3004,  -23.629,  0.11014,   34.463,   39.467,  -39.835,
//      51.313,     31.5,   42.806,  -17.367,   1.6561,   -102.5,
//      11.559,   1.8874,   2.4957,  -1.0672,  -1.4463,  -7.9751,
//    -0.56391,  -87.882,   10.352,   161.99,  -16.597,   11.586,
//    -0.13634,   9.0623,   2.1042,  0.21494,   1.7812,  -2.1123,
//      69.364,  -4.1807,  -267.27,   15.112,   11.713,   307.95,
//      5.0117,  -1.4017,  -36.272,   2.6605,  -2.4641,   34.556,
//     -95.995,   55.305,  -165.14,  -14.984,  -25.948,   238.88,
//     -8.1701,   3.9106,   -12.32, -0.35333,   -1.028,     16.3,
//};
//定腿长系数
float K[4][10] = 
{
		 -3.9975,  -29.97,  -66.518,  -15.04,  -128.37,  -13.444,  -33.65,  0.1267,  -60.298,  -5.9936,
     -3.9975,  -29.97,  66.518,  15.04,  -33.65,  0.1267,  -128.37,  -13.444,  -60.298,  -5.9936,
     2.8713,  21.398,  -164.02,  -21.212,  352.54,  18.097,  -119.38,  -3.3365,  -448.82,  -32.918,
     2.8713,  21.398,  164.02,  21.212,  -119.38,  -3.3365,  352.54,  18.097,  -448.82,  -32.918
};

//多项式拟合的控制律K矩阵
float Fitting_K[4][10];

void Fitting_K_Calc(float (*fitting_k)[10],float (*p)[6],float L_l,float L_r)
{
	static unsigned short int i = 0;
	static unsigned short int j = 0;
	
	for(i=0;i<=3;i++)
	{
		for(j=0;j<=9;j++)
		{
			fitting_k[i][j] = p[i*10+j][0] + p[i*10+j][1]*L_l + p[i*10+j][2]*L_r + p[i*10+j][3]*(L_l*L_l) + p[i*10+j][4]*L_l*L_r + p[i*10+j][5]*(L_r*L_r);
		}
	}
}

void LQR_Calc(Body_Posture *body,
							Leg_Posture *leg
						 )
{
	//bz:|0,T_wl|1,T_wr|2,T_bl|3,T_br|
	static float T[4];
	static unsigned short int i = 0;
	
	if(chassis_mode == chassis_down)
	{
//		for(i=0;i<=3;i++)
//		{
//			T[i] =     (comp->Gravity_Comp_Theta    -leg[0]->theta)*K[i][4] + (0              -leg[0]->d_theta)*K[i][5] 
//								+(comp->Gravity_Comp_Theta    -leg[1]->theta)*K[i][6] + (0              -leg[1]->d_theta)*K[i][7] 
//								+(0                           -body->theta  )*K[i][8] + (0              -body->d_theta  )*K[i][9]
//			
//								+(comp->Gravity_Comp_X        -body->x      )*K[i][0] + (goal->d_x_t    -body->d_x      )*K[i][1]
//								+(Find_Min_RADIAN(body->yaw,goal->yaw_t)    )*K[i][2] + (goal->d_yaw_t  -body->d_yaw    )*K[i][3];
//		}
		for(i=0;i<=3;i++)
		{
			T[i] = body->x * Fitting_K[i][0] + body->dx * Fitting_K[i][1] +
						 0 * Fitting_K[i][2] + body->INS_gyro[2] * Fitting_K[i][3] +
						 leg[0].theata * Fitting_K[i][4] + leg[0].dtheata * Fitting_K[i][5] +
						 leg[1].theata * Fitting_K[i][6] + leg[1].dtheata * Fitting_K[i][7] +
						 -body->INS_angle[2] * Fitting_K[i][8]  -body->INS_gyro[0] * Fitting_K[i][9];
		}
	}
	else if(chassis_mode == chassis_normal)
	{

//			for(i=0;i<=3;i++)
//			{
//				T[i] =     (0 - leg[0]->theta)*Fitting_K[i][4]*0.5f + (0 - leg[0]->d_theta)*Fitting_K[i][5]*0.5f 
//									+(0 - leg[1]->theta)*Fitting_K[i][6]*0.5f + (0 - leg[1]->d_theta)*Fitting_K[i][7]*0.5f;
//			}
		//for(i=0;i<=3;i++)
		//{
			T[0] = body->x * Fitting_K[0][0] + (body->dx + Body.target_v) * Fitting_K[0][1] 
						 -(target_speed.target_yaw - YawTotalAngle) * Fitting_K[0][2] + body->INS_gyro[2] * Fitting_K[0][3] +
						 leg[0].theata * Fitting_K[0][4] + leg[0].dtheata * Fitting_K[0][5] +
						 leg[1].theata * Fitting_K[0][6] + leg[1].dtheata * Fitting_K[0][7] +
						 -body->INS_angle[2] * Fitting_K[0][8]  -body->INS_gyro[0] * Fitting_K[0][9];
		
			T[1] = 	body->x * Fitting_K[1][0] + (body->dx + Body.target_v) * Fitting_K[1][1] 
							-(target_speed.target_yaw - YawTotalAngle) * Fitting_K[1][2] + body->INS_gyro[2] * Fitting_K[1][3] +
							leg[0].theata * Fitting_K[1][4] + leg[0].dtheata * Fitting_K[1][5] +
							leg[1].theata * Fitting_K[1][6] + leg[1].dtheata * Fitting_K[1][7] +
							-body->INS_angle[2] * Fitting_K[1][8]  -body->INS_gyro[0] * Fitting_K[1][9];
		
			T[2] = 	body->x * Fitting_K[2][0] + (body->dx + Body.target_v) * Fitting_K[2][1] 
							-(target_speed.target_yaw - YawTotalAngle) * Fitting_K[2][2] + body->INS_gyro[2] * Fitting_K[2][3] +
							leg[0].theata * Fitting_K[2][4] + leg[0].dtheata * Fitting_K[2][5] +
							leg[1].theata * Fitting_K[2][6] + leg[1].dtheata * Fitting_K[2][7] +
							-body->INS_angle[2] * Fitting_K[2][8]  -body->INS_gyro[0] * Fitting_K[2][9];
							
			T[3] = 	body->x * Fitting_K[3][0] + (body->dx + Body.target_v) * Fitting_K[3][1] 
							-(target_speed.target_yaw - YawTotalAngle) * Fitting_K[3][2] + body->INS_gyro[2] * Fitting_K[3][3] +
							leg[0].theata * Fitting_K[3][4] + leg[0].dtheata * Fitting_K[3][5] +
							leg[1].theata * Fitting_K[3][6] + leg[1].dtheata * Fitting_K[3][7] +
							-body->INS_angle[2] * Fitting_K[3][8]  -body->INS_gyro[0] * Fitting_K[3][9];
		//}

	}
	
	leg[0].T = T[0]; leg[1].T = T[1];
	leg[0].Tp = T[2]; leg[1].Tp = T[3];
}

void VM_cal(double F0, double Tp, double phi1, double phi4, double *Tp1,
            double *Tp2)
{
  double A0;
  double B0;
  double C0;
  double XB_tmp;
  double XD;
  double YB_tmp;
  double YD;
  double lBD;
  double phi2;
  double phi3;
  YD = 0.15 * sin(phi4);
  YB_tmp = 0.15 * sin(phi1);
  XD = 0.15 * cos(phi4) + 0.15;
  XB_tmp = 0.15 * cos(phi1);
  C0 = XD - XB_tmp;
  B0 = YD - YB_tmp;
  lBD = sqrt(C0 * C0 + B0 * B0);
  A0 = 0.54 * C0;
  B0 *= 0.54;
  C0 = (lBD * lBD + 0.0729) - 0.0729;
  phi2 = 2.0 * atan2(B0 + sqrt((A0 * A0 + B0 * B0) - C0 * C0), A0 + C0);
  C0 = 0.27 * cos(phi2);
  lBD = 0.27 * sin(phi2);
  phi3 = atan2((YB_tmp - YD) + lBD, (XB_tmp - XD) + C0);
  B0 = XB_tmp + C0;
  C0 = YB_tmp + lBD;
  XD = sqrt((B0 - 0.075) * (B0 - 0.075) + C0 * C0);
  C0 = atan2(C0, B0 - 0.075);
  lBD = sin(phi1 - phi2);
  A0 = sin(phi3 - phi2);
  YD = sin(phi3 - phi4);
  B0 = C0 - phi3;
  C0 -= phi2;
  *Tp1 = 0.15 * sin(B0) * lBD / A0 * F0 + 0.15 * cos(B0) * lBD / XD * A0 * Tp;
  *Tp2 = 0.15 * sin(C0) * YD / A0 * F0 + 0.15 * cos(C0) * YD / XD * A0 * Tp;
}

void get_legparam(double phi1, double phi4, double *L0, double *phi0)
{
  double A0;
  double B0;
  double C0;
  double XB_tmp;
  double YB_tmp;
  double lBD;
  YB_tmp = 0.15 * sin(phi1);
  XB_tmp = 0.15 * cos(phi1);
  C0 = (0.15 * cos(phi4) + 0.15) - XB_tmp;
  B0 = 0.15 * sin(phi4) - YB_tmp;
  lBD = sqrt(C0 * C0 + B0 * B0);
  A0 = 0.54 * C0;
  B0 *= 0.54;
  C0 = (lBD * lBD + 0.0729) - 0.0729;
  C0 = 2.0 * atan2(B0 + sqrt((A0 * A0 + B0 * B0) - C0 * C0), A0 + C0);
  B0 = XB_tmp + 0.27 * cos(C0);
  C0 = YB_tmp + 0.27 * sin(C0);
  *L0 = sqrt((B0 - 0.075) * (B0 - 0.075) + C0 * C0);
  *phi0 = atan2(C0, B0 - 0.075);
}

double get_dphi0(double phi_1, double phi_4, double d_phi1, double d_phi4)
{
  double a;
  double a_tmp;
  double a_tmp_tmp;
  double b_a;
  double b_a_tmp;
  double b_a_tmp_tmp;
  double b_d_phi0_tmp;
  double b_d_phi0_tmp_tmp;
  double c_a;
  double c_a_tmp;
  double c_a_tmp_tmp;
  double c_d_phi0_tmp;
  double c_d_phi0_tmp_tmp;
  double d_a;
  double d_a_tmp;
  double d_a_tmp_tmp;
  double d_d_phi0_tmp;
  double d_d_phi0_tmp_tmp;
  double d_phi0_tmp;
  double d_phi0_tmp_tmp;
  double e_a;
  double e_a_tmp;
  double f_a;
  double f_a_tmp;
  double g_a;
  double h_a;
  double i_a;
  d_phi0_tmp_tmp = cos(phi_1);
  b_d_phi0_tmp_tmp = sin(phi_1);
  c_d_phi0_tmp_tmp = cos(phi_4);
  d_d_phi0_tmp_tmp = sin(phi_4);
  a_tmp_tmp = 0.15 * b_d_phi0_tmp_tmp;
  a_tmp = a_tmp_tmp - 0.15 * d_d_phi0_tmp_tmp;
  b_a_tmp_tmp = 0.15 * d_phi0_tmp_tmp;
  b_a_tmp = (0.15 - b_a_tmp_tmp) + 0.15 * c_d_phi0_tmp_tmp;
  c_a_tmp_tmp = a_tmp * a_tmp;
  d_a_tmp_tmp = b_a_tmp * b_a_tmp;
  c_a_tmp = ((c_a_tmp_tmp + d_a_tmp_tmp) + 0.0729) - 0.0729;
  d_a_tmp = c_a_tmp + 0.54 * b_a_tmp;
  e_a_tmp = 0.54 * a_tmp;
  f_a_tmp = 0.2916 * c_a_tmp_tmp;
  c_a_tmp_tmp = 0.2916 * d_a_tmp_tmp;
  d_a_tmp_tmp = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) - e_a_tmp;
  a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) - e_a_tmp;
  b_a = (0.27 *
             cos(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                             e_a_tmp) /
                            d_a_tmp)) -
         0.075) +
        b_a_tmp_tmp;
  c_a =
      0.27 * sin(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                             e_a_tmp) /
                            d_a_tmp)) +
      a_tmp_tmp;
  d_a = (0.27 *
             cos(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                             e_a_tmp) /
                            d_a_tmp)) -
         0.075) +
        b_a_tmp_tmp;
  e_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) - e_a_tmp;
  f_a = sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) - e_a_tmp;
  g_a = (0.27 *
             cos(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                             e_a_tmp) /
                            d_a_tmp)) -
         0.075) +
        b_a_tmp_tmp;
  h_a =
      0.27 * sin(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                             e_a_tmp) /
                            d_a_tmp)) +
      a_tmp_tmp;
  i_a = (0.27 *
             cos(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                             e_a_tmp) /
                            d_a_tmp)) -
         0.075) +
        b_a_tmp_tmp;
  d_phi0_tmp = 0.08748 * d_phi0_tmp_tmp * a_tmp;
  b_d_phi0_tmp = 0.08748 * b_d_phi0_tmp_tmp * b_a_tmp;
  c_d_phi0_tmp = 0.08748 * c_d_phi0_tmp_tmp * a_tmp;
  d_d_phi0_tmp = 0.08748 * d_d_phi0_tmp_tmp * b_a_tmp;
  return d_phi1 *
             ((0.15 * d_phi0_tmp_tmp +
               0.54 *
                   cos(2.0 *
                       atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                             e_a_tmp) /
                            d_a_tmp)) *
                   ((((d_phi0_tmp - 2.0 *
                                        (0.3 * d_phi0_tmp_tmp * a_tmp +
                                         0.3 * b_d_phi0_tmp_tmp * b_a_tmp) *
                                        c_a_tmp) +
                      b_d_phi0_tmp) /
                         (2.0 *
                          sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp)) -
                     0.081 * d_phi0_tmp_tmp) /
                        d_a_tmp -
                    (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                     e_a_tmp) *
                        ((0.081 * b_d_phi0_tmp_tmp +
                          0.3 * cos(phi_1) *
                              (0.15 * sin(phi_1) - 0.15 * sin(phi_4))) +
                         0.3 * sin(phi_1) *
                             ((0.15 - 0.15 * cos(phi_1)) + 0.15 * cos(phi_4))) /
                        (d_a_tmp * d_a_tmp)) /
                   (d_a_tmp_tmp * d_a_tmp_tmp / (d_a_tmp * d_a_tmp) + 1.0)) /
                  ((0.27 * cos(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                c_a_tmp_tmp) -
                                           e_a_tmp) /
                                          d_a_tmp)) -
                    0.075) +
                   b_a_tmp_tmp) +
              (0.27 * sin(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                           c_a_tmp_tmp) -
                                      e_a_tmp) /
                                     d_a_tmp)) +
               a_tmp_tmp) *
                  (a_tmp_tmp +
                   0.54 *
                       sin(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                            c_a_tmp_tmp) -
                                       e_a_tmp) /
                                      d_a_tmp)) *
                       ((((d_phi0_tmp -
                           2.0 *
                               (0.3 * cos(phi_1) *
                                    (0.15 * sin(phi_1) - 0.15 * sin(phi_4)) +
                                0.3 * sin(phi_1) *
                                    ((0.15 - 0.15 * cos(phi_1)) +
                                     0.15 * cos(phi_4))) *
                               (((a_tmp * a_tmp + b_a_tmp * b_a_tmp) + 0.0729) -
                                0.0729)) +
                          b_d_phi0_tmp) /
                             (2.0 * sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                         c_a_tmp_tmp)) -
                         0.081 * cos(phi_1)) /
                            d_a_tmp -
                        (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                         e_a_tmp) *
                            ((0.081 * sin(phi_1) +
                              0.3 * cos(phi_1) *
                                  (0.15 * sin(phi_1) - 0.15 * sin(phi_4))) +
                             0.3 * sin(phi_1) *
                                 ((0.15 - 0.15 * cos(phi_1)) +
                                  0.15 * cos(phi_4))) /
                            (d_a_tmp * d_a_tmp)) /
                       (a * a / (d_a_tmp * d_a_tmp) + 1.0)) /
                  (b_a * b_a)) /
             (c_a * c_a / (d_a * d_a) + 1.0) -
         d_phi4 *
             (0.54 *
                  cos(2.0 *
                      atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                            e_a_tmp) /
                           d_a_tmp)) *
                  ((((c_d_phi0_tmp - 2.0 *
                                         (0.3 * c_d_phi0_tmp_tmp * a_tmp +
                                          0.3 * d_d_phi0_tmp_tmp * b_a_tmp) *
                                         c_a_tmp) +
                     d_d_phi0_tmp) /
                        (2.0 *
                         sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp)) -
                    0.081 * c_d_phi0_tmp_tmp) /
                       d_a_tmp -
                   (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                    e_a_tmp) *
                       ((0.081 * d_d_phi0_tmp_tmp +
                         0.3 * cos(phi_4) *
                             (0.15 * sin(phi_1) - 0.15 * sin(phi_4))) +
                        0.3 * sin(phi_4) *
                            ((0.15 - 0.15 * cos(phi_1)) + 0.15 * cos(phi_4))) /
                       (d_a_tmp * d_a_tmp)) /
                  ((e_a * e_a / (d_a_tmp * d_a_tmp) + 1.0) *
                   ((0.27 * cos(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                                 c_a_tmp_tmp) -
                                            e_a_tmp) /
                                           d_a_tmp)) -
                     0.075) +
                    b_a_tmp_tmp)) +
              0.54 *
                  sin(2.0 *
                      atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                            e_a_tmp) /
                           d_a_tmp)) *
                  (0.27 * sin(2.0 * atan((sqrt((f_a_tmp - c_a_tmp * c_a_tmp) +
                                               c_a_tmp_tmp) -
                                          e_a_tmp) /
                                         d_a_tmp)) +
                   a_tmp_tmp) *
                  ((((c_d_phi0_tmp -
                      2.0 *
                          (0.3 * cos(phi_4) *
                               (0.15 * sin(phi_1) - 0.15 * sin(phi_4)) +
                           0.3 * sin(phi_4) *
                               ((0.15 - 0.15 * cos(phi_1)) +
                                0.15 * cos(phi_4))) *
                          (((a_tmp * a_tmp + b_a_tmp * b_a_tmp) + 0.0729) -
                           0.0729)) +
                     d_d_phi0_tmp) /
                        (2.0 *
                         sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp)) -
                    0.081 * cos(phi_4)) /
                       d_a_tmp -
                   (sqrt((f_a_tmp - c_a_tmp * c_a_tmp) + c_a_tmp_tmp) -
                    e_a_tmp) *
                       ((0.081 * sin(phi_4) +
                         0.3 * cos(phi_4) *
                             (0.15 * sin(phi_1) - 0.15 * sin(phi_4))) +
                        0.3 * sin(phi_4) *
                            ((0.15 - 0.15 * cos(phi_1)) + 0.15 * cos(phi_4))) /
                       (d_a_tmp * d_a_tmp)) /
                  ((f_a * f_a / (d_a_tmp * d_a_tmp) + 1.0) * (g_a * g_a))) /
             (h_a * h_a / (i_a * i_a) + 1.0);
}

