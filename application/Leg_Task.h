#ifndef __LEG_H
#define __LEG_H

#include "main.h"
#include <stdbool.h>
#include <math.h>
#include "pid.h"


#define wheelRadius 0.077f  //轮子
#define PI       3.14159265358979323846f   // pi
#define PI_2     1.57079632679489661923f   // pi/2
#define Init_offset 0.2268927f //0.52361163

#define LIMIT_MIN_MAX(x,min,max) (x) = (((x)<=(min))?(min):(((x)>=(max))?(max):(x)))

typedef struct
{	
	double x,dx;             									//驱动轮位移和速度		x+=dx;   (leftWheel.speed+rightWheel.speed)/2*wheelRadius;
	double theata,dtheata,ddtheata;   			 	//摆杆与竖直方向夹角  phi0-PI_2-Pitch; dphi0-Pitch_speed;
	double phi,dphi;         								 	//机体与水平夹角      Pitch Pitch_speed
		
	double last_dtheata,last_L0,last_dL0;    
	
	double phi0,dphi0,L0,Setpoint_L0,dL0,ddL0;
	double phi1,phi4,dphi1,dphi4;
	double z_dot;														
	
	
	double F0, T, Tp;														//F0	腿部电机推力	T	驱动轮电机输出扭矩   Tp	髋关节电机输出扭矩
	
	double T1,T2;															//对应电机的输出扭矩		VMC映射
	
	double Fn;   															//支持力		机体竖直加速度Zam-9.8
	double Wheel_speed;												//轮子速度
	float aver[4];
	
	pid_t legLength_PID;
	uint8_t Jump_Flag;
	uint16_t Jump_Time;
	
	bool off;
	
}Leg_Posture;

typedef struct
{
	float * INS_quat;  				//四元数指针
	float * INS_accel; 				//加速度指针
	float * INS_gyro; 				 	//角速度指针
	float * INS_angle; 				//欧拉角指针
	
	double mw;                  			//机体质量
	double meaasure_speed;
	int abnormal_count;
	bool IF_Abnormal,IF_Offground,recover;  //特殊状态检测   IF_Abnormal——不正常    IF_Offground——离地
    
	double target_v;										//目标速度
	pid_t Feedback_phi0, Yaw_feed, Roll_feed,Yaw_Pos;   //PID补偿系统
	double torque_const_3508;											//3508扭矩系数
	double turn_T;
}Body_Posture;

typedef struct
{
	double vx;
	double vy;
	double vr;
	double target_yaw;
	double targetlengh;
}Target_Speed;


void FN_Cale(Leg_Posture * Leg);
void Chassis_cal(Leg_Posture * Leg, double * K_Ratio);

extern Leg_Posture Leg_L,Leg_R;	
extern Body_Posture  Body;
extern Target_Speed target_speed;

#endif
