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

double K_Ratio_L[12] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,    // 定义的反馈增益K
												1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

double K_Ratio_R[12] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,    // 定义的反馈增益K
												1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

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
		
		lqr_k(Leg_L.L0,K_Ratio_L);  //计算K矩阵
		lqr_k(Leg_R.L0,K_Ratio_R); 
		
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
		FN_Cale(&Leg_L);
		FN_Cale(&Leg_R);
		
		if((Leg_L.off) == 1 && (Leg_R.off==1)) Body.IF_Offground = 1;
		else  Body.IF_Offground = 0;
		
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
	
	Leg_L.phi1 =  leg_left1.jointoffset + leg_left1.Pos;
	Leg_L.phi4 = 	leg_left2.jointoffset + leg_left2.Pos;
	Leg_R.phi1 =  leg_right1.jointoffset - leg_right1.Pos;
	Leg_R.phi4 = 	leg_right2.jointoffset - leg_right2.Pos;

	
	Leg_L.dphi1 = leg_left1.Speed;
	Leg_L.dphi4 = leg_left2.Speed;
	Leg_R.dphi1 = -leg_right1.Speed;
	Leg_R.dphi4 = -leg_right2.Speed;
	
	get_legparam(Leg_L.phi1,Leg_L.phi4,&Leg_L.L0, &Leg_L.phi0);
	get_legparam(Leg_R.phi1,Leg_R.phi4,&Leg_R.L0, &Leg_R.phi0);
	
	Leg_L.dphi0 = get_dphi0(Leg_L.phi1,Leg_L.phi4,Leg_L.dphi1,Leg_L.dphi4);
	Leg_R.dphi0 = get_dphi0(Leg_R.phi1,Leg_R.phi4,Leg_R.dphi1,Leg_R.dphi4);

	
	/*左腿机体角度与速度*/
	Leg_R.phi = Leg_L.phi = Body.INS_angle[2];
	Leg_R.dphi = Leg_L.dphi = Body.INS_gyro[0];
	
	
	/*左腿摆杆角度*/
	Leg_L.theata = PI_2 - Leg_L.phi0 - Leg_L.phi;
	Leg_R.theata = PI_2 - Leg_R.phi0 - Leg_R.phi;
	

	/*左腿摆杆速度*/
	Leg_L.dtheata = -Leg_L.dphi0  - Leg_L.dphi;
	Leg_R.dtheata = -Leg_R.dphi0  - Leg_R.dphi;
	
	/*左腿摆杆加速度*/
//	Leg_L.ddtheata = (Leg_L.dtheata - Leg_L.last_dtheata)/dt;
//	Leg_L.last_dtheata = Leg_L.dtheata;
//	Leg_R.ddtheata = (Leg_R.dtheata - Leg_R.last_dtheata)/dt;
//	Leg_R.last_dtheata = Leg_R.dtheata;
//	
//	
//		/*左腿伸展速度*/
//	Leg_L.dL0 = (Leg_L.L0 - Leg_L.last_L0)/dt;
//	Leg_L.last_L0 = Leg_L.L0;
//	Leg_R.dL0 = (Leg_R.L0 - Leg_R.last_L0)/dt;
//	Leg_R.last_L0 = Leg_R.L0;

//	
//	/*左腿伸展加速度*/
//	Leg_L.ddL0 = (Leg_L.dL0 - Leg_L.last_dL0)/dt;
//	Leg_L.last_dL0 = Leg_L.dL0;
//	Leg_R.ddL0 = (Leg_R.dL0 - Leg_R.last_dL0)/dt;
//	Leg_R.last_dL0 = Leg_R.dL0;


	// 观测任务  左轮速度反    右路速度正
	Leg_L.Wheel_speed =  -motor_left.filter_rate * 2.0f * PI  / 60.0f / (268.0f / 17.0f) ;
	Leg_R.Wheel_speed =  motor_right.filter_rate * 2.0f * PI  / 60.0f / (268.0f / 17.0f) ;
	
	
	//测量机体速度
	vlb =  Leg_L.Wheel_speed * wheelRadius + Leg_L.L0 * Leg_L.dtheata * arm_cos_f32(Leg_L.theata) + Leg_L.dL0 * arm_cos_f32(Leg_L.theata);
	vrb =  Leg_R.Wheel_speed * wheelRadius + Leg_R.L0 * Leg_R.dtheata * arm_cos_f32(Leg_R.theata) + Leg_R.dL0 * arm_cos_f32(Leg_R.theata);

	Body.meaasure_speed = (vlb + vrb) / 2.0f;
	
	
	//KF_Wheel_Accel_Update(Body.meaasure_speed,MotionAccel_b[1] ,dt); //- Body.INS_gyro[2] * Body.INS_gyro[2] * 0.14
	xvEstimateKF_Update(&vaEstimateKF,MotionAccel_b[1],Body.meaasure_speed);
	
	Leg_R.dx = Leg_L.dx = vel_acc[0];

//	if(Body.IF_Offground) Leg_L.x = Leg_R.x = 0;
//	else STOP_X();
	

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
	
	PID_struct_init(&Leg_L.legLength_PID,PID_POSITION,HalfCircle,100,10,300.0f,0,2500.0f);
	PID_struct_init(&Leg_R.legLength_PID,PID_POSITION,HalfCircle,100,10,300.0f,0,2500.0f);

	PID_struct_init(&Body.Yaw_feed,PID_POSITION,HalfCircle,50,50,20,0,10);
	PID_struct_init(&Body.Feedback_phi0,PID_POSITION,HalfCircle,5,5,15,0,8);
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
	Leg_R.x = Leg_L.x = 0;
	
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
	Leg_R.x = Leg_L.x = 0;
	
	Chassis_cal(&Leg_L,K_Ratio_L);
	Chassis_cal(&Leg_R,K_Ratio_R);
	
	//机体转向环的控制
	Body.turn_T = Body.Yaw_feed.p * (target_speed.target_yaw - YawTotalAngle) - Body.Yaw_feed.d * Body.INS_gyro[2];
	LIMIT_MIN_MAX(Body.turn_T,-20.0,20.0);
	
	Leg_L.Setpoint_L0 = Leg_L.L0;
	Leg_R.Setpoint_L0 = Leg_R.L0;
	

	Leg_L.T = Leg_L.T + Body.turn_T;
	Leg_R.T = Leg_R.T - Body.turn_T;

	
	motor_left.Motorout = (int16_t)(Leg_L.T / Body.torque_const_3508 * (16384.0f / 20.0f) / (268.0f / 17.0f));	
	LIMIT_MIN_MAX(motor_left.Motorout,-16384,16384);
	
	motor_right.Motorout = (int16_t)(Leg_R.T / Body.torque_const_3508 * (16384.0f / 20.0f) / (268.0f / 17.0f));	
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
	Chassis_cal(&Leg_L,K_Ratio_L);
	Chassis_cal(&Leg_R,K_Ratio_R);
	
	//机体转向环的控制
	Body.turn_T = Body.Yaw_feed.p * (target_speed.target_yaw - YawTotalAngle) - Body.Yaw_feed.d * Body.INS_gyro[2];
	LIMIT_MIN_MAX(Body.turn_T,-20.0,20.0);
	
	//双腿协调控制
	pid_calc(&Body.Feedback_phi0,Leg_R.phi0-Leg_L.phi0,0);

	
	//腿长计算
	L0_L = target_speed.targetlengh ;
	L0_R = target_speed.targetlengh ;

	
	//腿长限制
	LIMIT_MIN_MAX(L0_L,0.12,0.32f);
	LIMIT_MIN_MAX(L0_R,0.12,0.32f);
	
	
	// 一阶低通滤波器alpha参数
	static float alpha = 0.02f;
	


	if(Leg_L.Jump_Flag == 1||Leg_L.Jump_Flag == 2||Leg_L.Jump_Flag == 3 || Leg_L.Jump_Flag==4)
	{
		if(Leg_L.Jump_Flag==4)
		{
			//跳跃控制
			Leg_L.F0 = 	pid_calc(&Leg_L.legLength_PID,Leg_L.L0,0.18f);
			Leg_R.F0 = 	pid_calc(&Leg_R.legLength_PID,Leg_R.L0,0.18f);
			
		}
		else JUMP_Function();
			
	}
	else
	{
		// 正常直立行走模式
		Leg_L.Setpoint_L0 = (1.0f - alpha) * Leg_L.Setpoint_L0 + alpha * L0_L;
		Leg_R.Setpoint_L0 = (1.0f - alpha) * Leg_R.Setpoint_L0 + alpha * L0_R;
		
		
		if(Body.IF_Offground == 0)
		{
			Leg_L.F0 = 	pid_calc(&Leg_L.legLength_PID,Leg_L.L0,Leg_L.Setpoint_L0 + pid_calc(&Body.Roll_feed,Body.INS_angle[1],0.0f))+ gravity_comp;
			Leg_R.F0 = 	pid_calc(&Leg_R.legLength_PID,Leg_R.L0,Leg_R.Setpoint_L0 - pid_calc(&Body.Roll_feed,Body.INS_angle[1],0.0f))+ gravity_comp;
		}
		else
		{
			Leg_L.F0 = 	pid_calc(&Leg_L.legLength_PID,Leg_L.L0,Leg_L.Setpoint_L0 )+ gravity_comp;
			Leg_R.F0 = 	pid_calc(&Leg_R.legLength_PID,Leg_R.L0,Leg_R.Setpoint_L0 )+ gravity_comp;
		}
	}
	

	// 腿部实际力矩计算
	Leg_L.Tp =  Leg_L.Tp - Body.Feedback_phi0.pos_out;
	Leg_R.Tp =  Leg_R.Tp + Body.Feedback_phi0.pos_out;
	

	
	
	if(Body.IF_Offground)
	{
		// 离地
		Leg_L.T =Leg_R.T = 0;
	}
	else if(Body.recover)
	{
		// 异常状态紧急停车
		Leg_L.Tp = Leg_R.Tp = 0;
		Leg_L.T =Leg_R.T = 0;
	}
	else
	{
		Leg_L.T = Leg_L.T + Body.turn_T;
		Leg_R.T = Leg_R.T - Body.turn_T;
	}

		
	// VMC控制映射
	VM_cal(Leg_L.F0, Leg_L.Tp, Leg_L.phi1, Leg_L.phi4, &Leg_L.T1,&Leg_L.T2);
	VM_cal(Leg_R.F0, Leg_R.Tp, Leg_R.phi1, Leg_R.phi4, &Leg_R.T1,&Leg_R.T2);
	
	
	// 驱动轮力矩 -> 电流
	motor_left.Motorout = (int16_t)(Leg_L.T / Body.torque_const_3508 * (16384.0f / 20.0f) / (268.0f / 17.0f));	
	LIMIT_MIN_MAX(motor_left.Motorout,-16384,16384);
	motor_right.Motorout = (int16_t)(Leg_R.T / Body.torque_const_3508 * (16384.0f / 20.0f) / (268.0f / 17.0f));	
	LIMIT_MIN_MAX(motor_right.Motorout,-16384,16384);
	
	Set_ChassisMotor_Current(&hfdcan3,-motor_right.Motorout,motor_left.Motorout,0,0);
	
	Send_Hip_Motor();
}	


void Chassis_State_Control(void)
{

	if  		(remoter->key.b == SWC_UP) 		{ target_speed.targetlengh  = 0.15;  Leg_L.Jump_Flag =  Leg_R.Jump_Flag = 0;}
	else if	(remoter->key.b == SWC_DOWN)  { target_speed.targetlengh  = 0.20;  Leg_L.Jump_Flag =  Leg_R.Jump_Flag = 0;} //{	if((Leg_L.Jump_Flag ==0) && (Leg_R.Jump_Flag ==0)) Leg_L.Jump_Flag =  Leg_R.Jump_Flag = 1; }
	
	
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
	LEG_Motor_move_control(&hfdcan1,MOTOR_LEG_LEFT_ID1,Leg_L.T1,0,0,0,0); delay_us(300);
	LEG_Motor_move_control(&hfdcan1,MOTOR_LEG_LEFT_ID2,Leg_L.T2,0,0,0,0); delay_us(300);
	LEG_Motor_move_control(&hfdcan2,MOTOR_LEG_RIGHT_ID1,-Leg_R.T1,0,0,0,0); delay_us(300);
	LEG_Motor_move_control(&hfdcan2,MOTOR_LEG_RIGHT_ID2,-Leg_R.T2,0,0,0,0); delay_us(300);
}




//停车定位控制
void STOP_X(void)  
{
	if(Body.target_v !=0 || abs(remoter->joy.right_hori) > 100) 
	{
		StopFlag = 1;
		Leg_R.x = Leg_L.x = 0;
	}
	else
	{
		if(StopFlag && fabs(Leg_L.x)<0.2f)
		{
			StopFlag=0; 
		}
		if(!StopFlag)
		{
			Leg_L.x += Leg_L.dx * dt;
			Leg_R.x = Leg_L.x;
		}
	}
}

//跳跃控制函数（控制腿部长度变化）
void JUMP_Function(void)
{
	if( Leg_L.Jump_Flag ==1 && Leg_R.Jump_Flag ==1)
	{//压缩阶段
		Leg_L.F0=pid_calc(&Leg_L.legLength_PID,Leg_L.L0,0.15f);//前馈+pd
		Leg_R.F0=pid_calc(&Leg_R.legLength_PID,Leg_R.L0,0.15f);

		if(Leg_L.L0<0.20f || Leg_R.L0<0.20f)
		{
			Leg_L.Jump_Time++;
			Leg_R.Jump_Time++;
		}
		if(Leg_L.Jump_Time>=4&&Leg_R.Jump_Time>=4)
		{  
			Leg_L.Jump_Time=0;
			Leg_R.Jump_Time=0;
			Leg_L.Jump_Flag=2;
			Leg_R.Jump_Flag=2;//压缩完毕进入上升加速阶段
		}			 
	}
	else if(Leg_L.Jump_Flag==2)
	{//上升加速阶段
		Leg_L.F0=pid_calc(&Leg_L.legLength_PID,Leg_L.L0,0.27f) ;//前馈+pd
		Leg_R.F0=pid_calc(&Leg_R.legLength_PID,Leg_R.L0,0.27f) ;
		
		if(Leg_L.L0>0.25f || Leg_R.L0>0.25f )
		{
			Leg_L.Jump_Time++;
			Leg_R.Jump_Time++;
		}
		if(Leg_L.Jump_Time>=12&&Leg_R.Jump_Time>=12)
		{  
			Leg_L.Jump_Time=0;
			Leg_R.Jump_Time=0;
			Leg_L.Jump_Flag=3;
			Leg_R.Jump_Flag=3;
		}
	}
	else if(Leg_L.Jump_Flag==3)
	{//缩腿阶段
		Leg_L.F0=pid_calc(&Leg_L.legLength_PID,Leg_L.L0,0.18f);//前馈+pd
		Leg_R.F0=pid_calc(&Leg_R.legLength_PID,Leg_R.L0,0.18f);
		if(Leg_L.L0<0.20f || Leg_R.L0<0.20f)
		{
			Leg_L.Jump_Time++;
			Leg_R.Jump_Time++;
		}
		if(Leg_L.Jump_Time>=7&&Leg_L.Jump_Time>=7)
		{ 
			Leg_L.Jump_Time=0;
			Leg_R.Jump_Time=0;
			Leg_L.Jump_Flag=4;
			Leg_R.Jump_Flag=4;
			target_speed.targetlengh = 0.20f;
		}
	}
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


void AbnormalDetect(Body_Posture *balance_infantry)  //异常状态切换
{
	if ((fabsf(balance_infantry->INS_angle[2]) > 0.20f) ||
			 (fabs(Leg_L.theata) > 0.5f) ||
			 (fabs(Leg_R.theata) > 0.5f))
	{
			balance_infantry->abnormal_count++;
			if(balance_infantry->abnormal_count > 250)
			{
				balance_infantry->abnormal_count = 250;
			}			

	}
//	else
//	{
//		balance_infantry->abnormal_count = balance_infantry->abnormal_count - 5;
//		if(balance_infantry->abnormal_count < 0) balance_infantry->abnormal_count = 0;
//	}

	if (balance_infantry->abnormal_count >= 250)
	{
		balance_infantry->IF_Abnormal = TRUE;
	}
//	else if(balance_infantry->abnormal_count <10)
//	{
//		balance_infantry->IF_Abnormal = FALSE;
//	}
}





Chassis_Mode chassis_mode;

