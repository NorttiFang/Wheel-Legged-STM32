#ifndef __Right_LEG_H
#define __Right_LEG_H
//#include "pid.h"
//#include "Leg_Task.h"
#include "main.h"
#include "cmsis_os.h"
#include "Leg_Task.h"
#include "bsp_can.h"
#include "LQR_VMC.h"
#include "imu_task.h"
#include <math.h>

void Chassis_Update_R(void);
void Chassis_Normol_R(void);

#endif
