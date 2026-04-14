#include "Chassis_Control.h"
#include "imu_task.h"
#include "LQR_VMC.h"
#include "pid.h"
#include "uart_bsp.h"
#include "bsp_can.h"
#include "bsp_dwt.h"
#include "arm_math.h"
#include "bsp_delay.h"
#include "math.h"

extern float        gyro[ 3 ];
extern float 				imuAngle[ 3 ];
extern float        MotionAccel_b[3]; // 机体坐标加速度[ 3 ];
extern float		 		YawTotalAngle;
uint32_t Task_DWT_Count = 0;

KalmanFilter_t vaEstimateKF;	   // 卡尔曼滤波器结构体

float vaEstimateKF_F[4] = {1.0f, 0.002f, 
                           0.0f, 1.0f};	   // 状态转移矩阵，控制周期为0.001s

float vaEstimateKF_P[4] = {1.0f, 0.0f,
                           0.0f, 1.0f};    // 后验估计协方差初始值

float vaEstimateKF_Q[4] = {1.0f, 0.0f, 
                           0.0f, 1.0f};    // Q矩阵初始值

float vaEstimateKF_R[4] = {200.0f, 0.0f, 
                            0.0f,  200.0f}; 	
														
float vaEstimateKF_K[4];
													 
const float vaEstimateKF_H[4] = {1.0f, 0.0f,
                                 0.0f, 1.0f};	// 设置矩阵H为常量

float vel_acc[2]; 


int16_t    send_left  = 0;
int16_t    send_right = 0;
remoter_t* remoter;
bool start_finish =  true;
uint16_t start_flag=0;
int Resetcnt, StopFlag, start_f=0;
float vlb,vrb;
												
float dt = 0.002f;
								

												
//斜坡函数
float Ramp_Function(float ref)
{
	static float error = 0;
	static float out = 0;

	error = ref - out;
	
	if(ref == 0)
	{
		if(fabs(error)<0.0001f)
		{
			out = 0;
		}
		else
		{
			out += error*0.05f;
		}
	}
	else 
	{
		out += error*0.05f;
	}
	
	return out;
}


// 限制最小值
int16_t Limit_Min(int16_t num,int min)
{
	if(num<-min || num>min)
	{
		return num;
	}
	else
	{
		return 0;
	}
}
																					

void Chassis_Control( void const* argument ) 
{
	osDelay(2000);
	Chassis_Init();
	xvEstimateKF_Init(&vaEstimateKF);
	
	for ( ;; ) 
	{
		Start_DM_Motor();
		Chassis_State_Update();
		Chassis_State_Control();
		
		
		Chassis_Switch();
			


		osDelay( 1 );
   }
}




void Chassis_Switch(void)
{
	if(chassis_mode==chassis_weak)
	{
		Body.abnormal_count = 0;
		Body.IF_Abnormal = false;
		Body.IF_Offground = 0;
		
		Chassis_Weak();
	}
	else if(chassis_mode == chassis_down)
	{
		Chassis_Down();
		Body.IF_Offground = 0;
	}
	else if(chassis_mode == chassis_normal)
	{
//		AbnormalDetect(&Body);
		Chassis_Normol();
		//离地检测
//		FN_Cale(&Leg[0]);
//		FN_Cale(&Leg[1]);
		
//		if((Leg[0].off) == 1 && (Leg[1].off==1)) Body.IF_Offground = 1;
//		else  Body.IF_Offground = 0;
		
	}
	

	
}


float t1,t2;
void Start_DM_Motor(void)
{

	if(remoter->key.d == SWD_UP)
	{
		start_finish = true;
		t1 = HAL_GetTick();
		Resetcnt = 1;
		start_f = 0;
		
		LEG_Disenable_Motor(&hfdcan1,MOTOR_LEG_LEFT_ID1);
		LEG_Disenable_Motor(&hfdcan1,MOTOR_LEG_LEFT_ID2);
		LEG_Disenable_Motor(&hfdcan2,MOTOR_LEG_RIGHT_ID1);
		LEG_Disenable_Motor(&hfdcan2,MOTOR_LEG_RIGHT_ID2);
//		osDelay(1);
	}
	else if((remoter->key.d == SWD_DOWN) && !start_f)
	{
		t2 = HAL_GetTick();
		if((t2 - t1)>2000) 
		{
			start_f = 1;
			Resetcnt = 0;
		}
	}
	else if(((remoter->key.d == SWD_DOWN) && !Resetcnt) )
	{		
		LEG_Enable_Motor(&hfdcan1,MOTOR_LEG_LEFT_ID1);
		LEG_Enable_Motor(&hfdcan1,MOTOR_LEG_LEFT_ID2);
		LEG_Enable_Motor(&hfdcan2,MOTOR_LEG_RIGHT_ID1);
		LEG_Enable_Motor(&hfdcan2,MOTOR_LEG_RIGHT_ID2);
		
		osDelay(1);
		
		if(remoter->key.a == SWA_UP) ;
		else if(remoter->key.a == SWA_DOWN)
		{

			while(start_finish)
			{
				start_flag++;
				if(start_flag > 500) start_finish = false;
				
				LEG_Motor_move_control(&hfdcan1,MOTOR_LEG_LEFT_ID1,1,0,0,0,0);
				LEG_Motor_move_control(&hfdcan1,MOTOR_LEG_LEFT_ID2,-1,0,0,0,0);
				LEG_Motor_move_control(&hfdcan2,MOTOR_LEG_RIGHT_ID1,-1,0,0,0,0);
				LEG_Motor_move_control(&hfdcan2,MOTOR_LEG_RIGHT_ID2,1,0,0,0,0);
				
				osDelay(1);
			}
			
			
			LEG_Set_ZeroPos(&hfdcan1,MOTOR_LEG_LEFT_ID1);
			LEG_Set_ZeroPos(&hfdcan1,MOTOR_LEG_LEFT_ID2);
			LEG_Set_ZeroPos(&hfdcan2,MOTOR_LEG_RIGHT_ID1);
			LEG_Set_ZeroPos(&hfdcan2,MOTOR_LEG_RIGHT_ID2);
			
			osDelay(1);
		
			LEG_Motor_move_control(&hfdcan1,MOTOR_LEG_LEFT_ID1,0,0,0,0,0);
			LEG_Motor_move_control(&hfdcan1,MOTOR_LEG_LEFT_ID2,0,0,0,0,0);
			LEG_Motor_move_control(&hfdcan2,MOTOR_LEG_RIGHT_ID1,0,0,0,0,0);
			LEG_Motor_move_control(&hfdcan2,MOTOR_LEG_RIGHT_ID2,0,0,0,0,0);
			
		}


		Resetcnt=1;
	}

}


void Chassis_State_Update(void)
{
	dt = DWT_GetDeltaT(&Task_DWT_Count);
	
	Leg[0].phi1 =  leg_left1.jointoffset + leg_left1.Pos;
	Leg[0].phi4 = 	leg_left2.jointoffset + leg_left2.Pos;
	Leg[1].phi1 =  leg_right1.jointoffset - leg_right1.Pos;
	Leg[1].phi4 = 	leg_right2.jointoffset - leg_right2.Pos;

	
	Leg[0].dphi1 = leg_left1.Speed;
	Leg[0].dphi4 = leg_left2.Speed;
	Leg[1].dphi1 = -leg_right1.Speed;
	Leg[1].dphi4 = -leg_right2.Speed;
	
	get_legparam(Leg[0].phi1,Leg[0].phi4,&Leg[0].L0, &Leg[0].phi0);
	get_legparam(Leg[1].phi1,Leg[1].phi4,&Leg[1].L0, &Leg[1].phi0);
	
	Leg[0].dphi0 = get_dphi0(Leg[0].phi1,Leg[0].phi4,Leg[0].dphi1,Leg[0].dphi4);
	Leg[1].dphi0 = get_dphi0(Leg[1].phi1,Leg[1].phi4,Leg[1].dphi1,Leg[1].dphi4);

	
	/*左腿机体角度与速度*/
	Leg[1].phi = Leg[0].phi = Body.INS_angle[2];
	Leg[1].dphi = Leg[1].dphi = Body.INS_gyro[0];
	
	
	/*左腿摆杆角度*/
	Leg[0].theata = PI_2 - Leg[0].phi0 - Leg[0].phi;
	Leg[1].theata = PI_2 - Leg[1].phi0 - Leg[1].phi;
	

	/*左腿摆杆速度*/
	Leg[0].dtheata = -Leg[0].dphi0  - Leg[0].dphi;
	Leg[1].dtheata = -Leg[1].dphi0  - Leg[1].dphi;
	


	// 观测任务  左轮速度反    右路速度正
	Leg[0].Wheel_speed =  -motor_left.filter_rate * 2.0f * PI  / 60.0f / (268.0f / 17.0f) ;
	Leg[1].Wheel_speed =  motor_right.filter_rate * 2.0f * PI  / 60.0f / (268.0f / 17.0f) ;
	
	
	//测量机体速度
	vlb =  Leg[0].Wheel_speed * wheelRadius + Leg[0].L0 * Leg[0].dtheata * arm_cos_f32(Leg[0].theata) + Leg[0].dL0 * arm_cos_f32(Leg[0].theata);
	vrb =  Leg[1].Wheel_speed * wheelRadius + Leg[1].L0 * Leg[1].dtheata * arm_cos_f32(Leg[1].theata) + Leg[1].dL0 * arm_cos_f32(Leg[1].theata);

	Body.meaasure_speed = (vlb + vrb) / 2.0f;
	
 
	xvEstimateKF_Update(&vaEstimateKF,MotionAccel_b[1],Body.meaasure_speed);//- Body.INS_gyro[2] * Body.INS_gyro[2] * 0.14
	
	Body.dx = vel_acc[0];
	
	STOP_X();
	

}




void Chassis_Init(void)
{
	remoter = Get_remoter_Sbus();
	WS2812_Ctrl( 1, 50, 1 );
	
	Body.INS_gyro = gyro;
	Body.INS_angle = imuAngle;
	Body.torque_const_3508 = 0.2462823725981621f;
	Body.mw = 0.499f;
	chassis_mode = chassis_weak;
	
	PID_struct_init(&Leg[0].legLength_PID,PID_POSITION,HalfCircle,100,10,300.0f,0,2500.0f);
	PID_struct_init(&Leg[1].legLength_PID,PID_POSITION,HalfCircle,100,10,300.0f,0,2500.0f);

	PID_struct_init(&Body.Roll_feed,PID_POSITION,HalfCircle,0.20,0.20,0.4,0,0);
	
	leg_left1.jointoffset = Init_offset + 3.1415926f;
	leg_left2.jointoffset = -Init_offset;
	leg_right1.jointoffset = Init_offset +3.1415926f;
	leg_right2.jointoffset = -Init_offset;
	
	target_speed.targetlengh = 0.18f;
	
	
}


// 无力模式
void Chassis_Weak(void)				
{
	Body.x = 0;
	
	motor_left.Motorout = 0;
	motor_right.Motorout = 0;
	
	Set_ChassisMotor_Current(&hfdcan3,motor_left.Motorout,motor_right.Motorout,0,0);
	
	LEG_Motor_move_control(&hfdcan1,MOTOR_LEG_LEFT_ID1,0,0,0,0,0); delay_us(300);
	LEG_Motor_move_control(&hfdcan1,MOTOR_LEG_LEFT_ID2,0,0,0,0,0); delay_us(300);
	LEG_Motor_move_control(&hfdcan2,MOTOR_LEG_RIGHT_ID1,0,0,0,0,0); delay_us(300);
	LEG_Motor_move_control(&hfdcan2,MOTOR_LEG_RIGHT_ID2,0,0,0,0,0); delay_us(300);
	target_speed.target_yaw = YawTotalAngle;

	
}	

// 板凳模式
void Chassis_Down(void)
{
	//Body.x = 0;
	Leg[0].Setpoint_L0 = Leg[0].L0;
	Leg[1].Setpoint_L0 = Leg[1].L0;
	
	Fitting_K_Calc(Fitting_K,P,Leg[0].L0,Leg[1].L0);
	LQR_Calc(&Body,Leg);

	

	
	motor_left.Motorout = (int16_t)(Leg[0].T / Body.torque_const_3508 * (16384.0f / 20.0f) / (268.0f / 17.0f));	
	LIMIT_MIN_MAX(motor_left.Motorout,-16384,16384);
	
	motor_right.Motorout = (int16_t)(Leg[1].T / Body.torque_const_3508 * (16384.0f / 20.0f) / (268.0f / 17.0f));	
	LIMIT_MIN_MAX(motor_right.Motorout,-16384,16384);
	
	

	Set_ChassisMotor_Current(&hfdcan3,-motor_right.Motorout,motor_left.Motorout,0,0);
	
	

	LEG_Motor_move_control(&hfdcan1,MOTOR_LEG_LEFT_ID1,0,-Init_offset,0,10,0.1);  delay_us(300);
	LEG_Motor_move_control(&hfdcan1,MOTOR_LEG_LEFT_ID2,0,Init_offset,0,10,0.1); 	delay_us(300);
	LEG_Motor_move_control(&hfdcan2,MOTOR_LEG_RIGHT_ID1,0,Init_offset,0,10,0.1); 	delay_us(300);
	LEG_Motor_move_control(&hfdcan2,MOTOR_LEG_RIGHT_ID2,0,-Init_offset,0,10,0.1); delay_us(300);

	
	
}

// 腿部正常运动模式
void Chassis_Normol(void)
{
	//重力补偿
	static float gravity_comp = 5;
	
	//左右腿目标长度
	static float L0_L, L0_R;
	
	
	//拟合对应腿长所需的K增益
	Fitting_K_Calc(Fitting_K,P,Leg[0].L0,Leg[1].L0);
	LQR_Calc(&Body,Leg);

	
	//腿长计算
	L0_L = target_speed.targetlengh ;
	L0_R = target_speed.targetlengh ;

	
	//腿长限制
	LIMIT_MIN_MAX(L0_L,0.12,0.32f);
	LIMIT_MIN_MAX(L0_R,0.12,0.32f);
	
	
	// 一阶低通滤波器alpha参数
	static float alpha = 0.02f;
	


	if(Leg[0].Jump_Flag == 1||Leg[0].Jump_Flag == 2||Leg[0].Jump_Flag == 3 || Leg[0].Jump_Flag==4)
	{
		if(Leg[0].Jump_Flag==4)
		{
			//跳跃控制
			Leg[0].F0 = 	pid_calc(&Leg[0].legLength_PID,Leg[0].L0,0.18f);
			Leg[1].F0 = 	pid_calc(&Leg[1].legLength_PID,Leg[1].L0,0.18f);
			
		}
		else JUMP_Function();
			
	}
	else
	{
		// 正常直立行走模式
		Leg[0].Setpoint_L0 = (1.0f - alpha) * Leg[0].Setpoint_L0 + alpha * L0_L;
		Leg[1].Setpoint_L0 = (1.0f - alpha) * Leg[1].Setpoint_L0 + alpha * L0_R;
		
		
		if(Body.IF_Offground == 0)
		{
			Leg[0].F0 = 	pid_calc(&Leg[0].legLength_PID,Leg[0].L0,Leg[0].Setpoint_L0 + pid_calc(&Body.Roll_feed,Body.INS_angle[1],0.0f))+ gravity_comp;
			Leg[1].F0 = 	pid_calc(&Leg[1].legLength_PID,Leg[1].L0,Leg[1].Setpoint_L0 - pid_calc(&Body.Roll_feed,Body.INS_angle[1],0.0f))+ gravity_comp;
		}
		else
		{
			Leg[0].F0 = 	pid_calc(&Leg[0].legLength_PID,Leg[0].L0,Leg[0].Setpoint_L0 )+ gravity_comp;
			Leg[1].F0 = 	pid_calc(&Leg[1].legLength_PID,Leg[1].L0,Leg[1].Setpoint_L0 )+ gravity_comp;
		}
	}
	
	

	
	
	if(Body.IF_Offground)
	{
		// 离地
		Leg[0].T =Leg[1].T = 0;
	}
	else if(Body.recover)
	{
		// 异常状态紧急停车
		Leg[0].Tp = Leg[1].Tp = 0;
		Leg[0].T =Leg[1].T = 0;
	}

		
	// VMC控制映射
	VM_cal(Leg[0].F0, Leg[0].Tp *0.5, Leg[0].phi1, Leg[0].phi4, &Leg[0].T1,&Leg[0].T2);
	VM_cal(Leg[1].F0, Leg[1].Tp *0.5, Leg[1].phi1, Leg[1].phi4, &Leg[1].T1,&Leg[1].T2);
	
	
	// 驱动轮力矩 -> 电流
	motor_left.Motorout = (int16_t)(Leg[0].T / Body.torque_const_3508 * (16384.0f / 20.0f) / (268.0f / 17.0f));	
	LIMIT_MIN_MAX(motor_left.Motorout,-16384,16384);
	motor_right.Motorout = (int16_t)(Leg[1].T / Body.torque_const_3508 * (16384.0f / 20.0f) / (268.0f / 17.0f));	
	LIMIT_MIN_MAX(motor_right.Motorout,-16384,16384);
	
	Set_ChassisMotor_Current(&hfdcan3,-motor_right.Motorout,motor_left.Motorout,0,0);
	
	Send_Hip_Motor();
}	


void Chassis_State_Control(void)
{

	if  		(remoter->key.b == SWC_UP) 		{ target_speed.targetlengh  = 0.15;  Leg[0].Jump_Flag =  Leg[1].Jump_Flag = 0;}
	else if	(remoter->key.b == SWC_DOWN)  { target_speed.targetlengh  = 0.20;  Leg[0].Jump_Flag =  Leg[1].Jump_Flag = 0;} //{	if((Leg_L.Jump_Flag ==0) && (Leg_R.Jump_Flag ==0)) Leg_L.Jump_Flag =  Leg_R.Jump_Flag = 1; }
	
	
	Body.target_v = Ramp_Function(Limit_Min(remoter->joy.right_vert,8))/400.0f;
	
	if(abs(remoter->joy.right_hori) < 100) target_speed.target_yaw = target_speed.target_yaw;
	else
	{
//		target_speed.target_yaw -= abs(remoter->joy.right_hori)/(remoter->joy.right_hori) * 0.000001f;
		target_speed.target_yaw -= remoter->joy.right_hori * 0.000025f;
	}
  

	
	
	
}


// 关节力矩返送函数
void Send_Hip_Motor(void)
{
	LEG_Motor_move_control(&hfdcan1,MOTOR_LEG_LEFT_ID1,Leg[0].T1,0,0,0,0); delay_us(300);
	LEG_Motor_move_control(&hfdcan1,MOTOR_LEG_LEFT_ID2,Leg[0].T2,0,0,0,0); delay_us(300);
	LEG_Motor_move_control(&hfdcan2,MOTOR_LEG_RIGHT_ID1,-Leg[1].T1,0,0,0,0); delay_us(300);
	LEG_Motor_move_control(&hfdcan2,MOTOR_LEG_RIGHT_ID2,-Leg[1].T2,0,0,0,0); delay_us(300);
}




//停车定位控制
void STOP_X(void)  
{
	if(Body.target_v !=0 || abs(remoter->joy.right_hori) > 100) 
	{
		StopFlag = 1;
		Body.x = 0;
	}
	else
	{
		if(StopFlag && fabs(Body.dx)<0.2f)
		{
			StopFlag=0; 
		}
		if(!StopFlag)
		{
			Body.x += Body.dx * dt;
		}
	}
}

//跳跃控制函数（控制腿部长度变化）
void JUMP_Function(void)
{

}

void xvEstimateKF_Init(KalmanFilter_t *EstimateKF)
{
    Kalman_Filter_Init(EstimateKF, 2, 0, 2);	// 状态向量2维 没有控制量 测量向量2维
	
		memcpy(EstimateKF->F_data, vaEstimateKF_F, sizeof(vaEstimateKF_F));
    memcpy(EstimateKF->P_data, vaEstimateKF_P, sizeof(vaEstimateKF_P));
    memcpy(EstimateKF->Q_data, vaEstimateKF_Q, sizeof(vaEstimateKF_Q));
    memcpy(EstimateKF->R_data, vaEstimateKF_R, sizeof(vaEstimateKF_R));
    memcpy(EstimateKF->H_data, vaEstimateKF_H, sizeof(vaEstimateKF_H));

}

void xvEstimateKF_Update(KalmanFilter_t *EstimateKF ,float acc,float vel)
{   	
	 memcpy(EstimateKF->Q_data, vaEstimateKF_Q, sizeof(vaEstimateKF_Q));
   memcpy(EstimateKF->R_data, vaEstimateKF_R, sizeof(vaEstimateKF_R));
	
    //卡尔曼滤波器测量值更新
    EstimateKF->MeasuredVector[0] =	vel;//测量速度
    EstimateKF->MeasuredVector[1] = acc;//测量加速度
    		
    //卡尔曼滤波器更新函数
    Kalman_Filter_Update(EstimateKF);

    // 提取估计值
    for (uint8_t i = 0; i < 2; i++)
    {
      vel_acc[i] = EstimateKF->FilteredValue[i];
    }
}


//void AbnormalDetect(Body_Posture *balance_infantry)  //异常状态切换
//{
//	if ((fabsf(balance_infantry->INS_angle[2]) > 0.20f) ||
//			 (fabs(Leg_L.theata) > 0.5f) ||
//			 (fabs(Leg_R.theata) > 0.5f))
//	{
//			balance_infantry->abnormal_count++;
//			if(balance_infantry->abnormal_count > 250)
//			{
//				balance_infantry->abnormal_count = 250;
//			}			

//	}
////	else
////	{
////		balance_infantry->abnormal_count = balance_infantry->abnormal_count - 5;
////		if(balance_infantry->abnormal_count < 0) balance_infantry->abnormal_count = 0;
////	}

//	if (balance_infantry->abnormal_count >= 250)
//	{
//		balance_infantry->IF_Abnormal = TRUE;
//	}
////	else if(balance_infantry->abnormal_count <10)
////	{
////		balance_infantry->IF_Abnormal = FALSE;
////	}
//}





Chassis_Mode chassis_mode;

