#ifndef __CHASSIS_CONTROL_H
#define __CHASSIS_CONTROL_H
#include "main.h"
#include "cmsis_os.h"
#include "bsp_can.h"
#include "uart_bsp.h"
#include "ws2812.h"
#include "wheel_ins.h"
#include "Leg_Task.h"


typedef enum
{
	chassis_weak = 0,
	chassis_normal,
	chassis_gyro,
	chassis_down,
}Chassis_Mode;

void Chassis_State_Update(void);
void Chassis_Switch(void);
void Chassis_Init(void);
void Start_DM_Motor(void);
void Chassis_Weak(void);
void Chassis_Down(void);
void Send_Hip_Motor(void);
void Chassis_Normol(void);
void Chassis_State_Control(void);
void STOP_X(void);
void JUMP_Function(void);
void AbnormalDetect(Body_Posture *balance_infantry) ;

void xvEstimateKF_Update(KalmanFilter_t *EstimateKF ,float acc,float vel);
void xvEstimateKF_Init(KalmanFilter_t *EstimateKF);

extern Chassis_Mode chassis_mode;

#endif
