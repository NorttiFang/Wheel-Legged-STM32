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
extern float        MotionAccel_b[3]; // ����������ٶ�[ 3 ];
extern float		 		YawTotalAngle;
uint32_t Task_DWT_Count = 0;

double K_Ratio_L[12] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,    // ����ķ�������K
												1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

double K_Ratio_R[12] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,    // ����ķ�������K
												1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

KalmanFilter_t vaEstimateKF;	   // �������˲����ṹ��

float vaEstimateKF_F[4] = {1.0f, 0.002f, 
                           0.0f, 1.0f};	   // ״̬ת�ƾ��󣬿�������Ϊ0.001s

float vaEstimateKF_P[4] = {1.0f, 0.0f,
                           0.0f, 1.0f};    // �������Э�����ʼֵ

float vaEstimateKF_Q[4] = {1.0f, 0.0f, 
                           0.0f, 1.0f};    // Q�����ʼֵ

float vaEstimateKF_R[4] = {200.0f, 0.0f, 
                            0.0f,  200.0f}; 	
														
float vaEstimateKF_K[4];
													 
const float vaEstimateKF_H[4] = {1.0f, 0.0f,
                                 0.0f, 1.0f};	// ���þ���HΪ����

float vel_acc[2]; 


int16_t    send_left  = 0;
int16_t    send_right = 0;
remoter_t* remoter;
bool start_finish =  true;
uint16_t start_flag=0;
int Resetcnt, StopFlag, start_f=0;
float vlb,vrb;
												
float dt = 0.002f;
								

												
//б�º���
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


// ������Сֵ
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
		
		lqr_k(Leg_L.L0,K_Ratio_L);  //����K����
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
		//��ؼ��
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

	
	/*���Ȼ���Ƕ����ٶ�*/
	Leg_R.phi = Leg_L.phi = Body.INS_angle[2];
	Leg_R.dphi = Leg_L.dphi = Body.INS_gyro[0];
	
	
	/*���Ȱڸ˽Ƕ�*/
	Leg_L.theata = PI_2 - Leg_L.phi0 - Leg_L.phi;
	Leg_R.theata = PI_2 - Leg_R.phi0 - Leg_R.phi;
	

	/*���Ȱڸ��ٶ�*/
	Leg_L.dtheata = -Leg_L.dphi0  - Leg_L.dphi;
	Leg_R.dtheata = -Leg_R.dphi0  - Leg_R.dphi;
	

	// �۲�����  �����ٶȷ�    ��·�ٶ���
	Leg_L.Wheel_speed =  -motor_left.filter_rate * 2.0f * PI  / 60.0f / (268.0f / 17.0f) ;
	Leg_R.Wheel_speed =  motor_right.filter_rate * 2.0f * PI  / 60.0f / (268.0f / 17.0f) ;
	
	
	//���������ٶ�
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


// ����ģʽ
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

// ���ģʽ
void Chassis_Down(void)
{
	Leg_R.x = Leg_L.x = 0;
	
	Chassis_cal(&Leg_L,K_Ratio_L);
	Chassis_cal(&Leg_R,K_Ratio_R);
	
	//����ת�򻷵Ŀ���
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

// �Ȳ������˶�ģʽ
void Chassis_Normol(void)
{
	//��������
	static float gravity_comp = 5;
	
	//������Ŀ�곤��
	static float L0_L, L0_R;
	
	
	//��϶�Ӧ�ȳ������K����
	Chassis_cal(&Leg_L,K_Ratio_L);
	Chassis_cal(&Leg_R,K_Ratio_R);
	
	//����ת�򻷵Ŀ���
	Body.turn_T = Body.Yaw_feed.p * (target_speed.target_yaw - YawTotalAngle) - Body.Yaw_feed.d * Body.INS_gyro[2];
	LIMIT_MIN_MAX(Body.turn_T,-20.0,20.0);
	
	//˫��Э������
	pid_calc(&Body.Feedback_phi0,Leg_R.phi0-Leg_L.phi0,0);

	
	//�ȳ�����
	L0_L = target_speed.targetlengh ;
	L0_R = target_speed.targetlengh ;

	
	//�ȳ�����
	LIMIT_MIN_MAX(L0_L,0.12,0.32f);
	LIMIT_MIN_MAX(L0_R,0.12,0.32f);
	
	
	// һ�׵�ͨ�˲���alpha����
	static float alpha = 0.02f;
	


	if(Leg_L.Jump_Flag == 1||Leg_L.Jump_Flag == 2||Leg_L.Jump_Flag == 3 || Leg_L.Jump_Flag==4)
	{
		if(Leg_L.Jump_Flag==4)
		{
			//��Ծ����
			Leg_L.F0 = 	pid_calc(&Leg_L.legLength_PID,Leg_L.L0,0.18f);
			Leg_R.F0 = 	pid_calc(&Leg_R.legLength_PID,Leg_R.L0,0.18f);
			
		}
		else JUMP_Function();
			
	}
	else
	{
		// ����ֱ������ģʽ
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
	

	// �Ȳ�ʵ�����ؼ���
	Leg_L.Tp =  Leg_L.Tp - Body.Feedback_phi0.pos_out;
	Leg_R.Tp =  Leg_R.Tp + Body.Feedback_phi0.pos_out;
	

	
	
	if(Body.IF_Offground)
	{
		// ���
		Leg_L.T =Leg_R.T = 0;
	}
	else if(Body.recover)
	{
		// �쳣״̬����ͣ��
		Leg_L.Tp = Leg_R.Tp = 0;
		Leg_L.T =Leg_R.T = 0;
	}
	else
	{
		Leg_L.T = Leg_L.T + Body.turn_T;
		Leg_R.T = Leg_R.T - Body.turn_T;
	}

	// 做一个MPC约束 T和Tp 是否要具体修饰代价值考虑
		
	// VMC����ӳ��
	VM_cal(Leg_L.F0, Leg_L.Tp, Leg_L.phi1, Leg_L.phi4, &Leg_L.T1,&Leg_L.T2);
	VM_cal(Leg_R.F0, Leg_R.Tp, Leg_R.phi1, Leg_R.phi4, &Leg_R.T1,&Leg_R.T2);
	
	
	// ���������� -> ����
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


// �ؽ����ط��ͺ���
void Send_Hip_Motor(void)
{
	LEG_Motor_move_control(&hfdcan1,MOTOR_LEG_LEFT_ID1,Leg_L.T1,0,0,0,0); delay_us(300);
	LEG_Motor_move_control(&hfdcan1,MOTOR_LEG_LEFT_ID2,Leg_L.T2,0,0,0,0); delay_us(300);
	LEG_Motor_move_control(&hfdcan2,MOTOR_LEG_RIGHT_ID1,-Leg_R.T1,0,0,0,0); delay_us(300);
	LEG_Motor_move_control(&hfdcan2,MOTOR_LEG_RIGHT_ID2,-Leg_R.T2,0,0,0,0); delay_us(300);
}




//ͣ����λ����
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

//��Ծ���ƺ����������Ȳ����ȱ仯��
void JUMP_Function(void)
{
	if( Leg_L.Jump_Flag ==1 && Leg_R.Jump_Flag ==1)
	{//ѹ���׶�
		Leg_L.F0=pid_calc(&Leg_L.legLength_PID,Leg_L.L0,0.15f);//ǰ��+pd
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
			Leg_R.Jump_Flag=2;//ѹ����Ͻ����������ٽ׶�
		}			 
	}
	else if(Leg_L.Jump_Flag==2)
	{//�������ٽ׶�
		Leg_L.F0=pid_calc(&Leg_L.legLength_PID,Leg_L.L0,0.27f) ;//ǰ��+pd
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
	{//���Ƚ׶�
		Leg_L.F0=pid_calc(&Leg_L.legLength_PID,Leg_L.L0,0.18f);//ǰ��+pd
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
    Kalman_Filter_Init(EstimateKF, 2, 0, 2);	// ״̬����2ά û�п����� ��������2ά
	
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
	
    //�������˲�������ֵ����
    EstimateKF->MeasuredVector[0] =	vel;//�����ٶ�
    EstimateKF->MeasuredVector[1] = acc;//�������ٶ�
    		
    //�������˲������º���
    Kalman_Filter_Update(EstimateKF);

    // ��ȡ����ֵ
    for (uint8_t i = 0; i < 2; i++)
    {
      vel_acc[i] = EstimateKF->FilteredValue[i];
    }
}


void AbnormalDetect(Body_Posture *balance_infantry)  //�쳣״̬�л�
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

