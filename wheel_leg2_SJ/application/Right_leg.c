//#include "Right_Leg.h"

//Leg_Posture         Leg_R;
//static Encoder_Leg *leg_right1, *leg_right2, *leg_left1, *leg_left2;
//static Encoder *    motor_right, *motor_left;
//float               R_dt            = 0.003f;
//double              K_Ratio_R[ 12 ] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

//void Leg_Task_R( void const* argument ) {
//    osDelay( 2000 );
//    leg_right1  = Get_Leg_Motor_right1();
//    leg_right2  = Get_Leg_Motor_right2();
//    leg_left1   = Get_Leg_Motor_left1();
//    leg_left2   = Get_Leg_Motor_left2();
//    motor_right = Get_Motor_right();
//    motor_left  = Get_Motor_left();
//    while ( 1 ) {
//        Chassis_Update_R();
//        FN_Cale( &Leg_R );
//        //
//        //		if(standstate == StandupState_Prepare)
//        //		{
//        //			chassis_mode = chassis_weak;
//        //		}
//        //		else
//        //		{
//        //			if((Leg_L.off==1) && (Leg_R.off ==1)) {B.IF_Offground = TRUE;}
//        //			else {B.IF_Offground = FALSE;}
//        //
//        //
//        //			if(chassis_mode != chassis_weak && chassis_mode != chassis_save)
//        //			{
//        //				if(!B.IF_Offground)
//        //				{
//        //					if(B.IF_Abnormal) chassis_mode = chassis_dowm;
//        //				}
//        //			}
//        //		}

//        //
//        //
//        Chassis_Normol_R();

//        		
//        
//        		osDelay(CHASSL_TIME_R); 
//    }
//}

//extern float imuAngle[ 3 ];
//extern float gyro[ 3 ];
//extern Body_Posture  Body;
//extern Target_Speed target_speed;

//void Chassis_Update_R( void ) {
//    Leg_R.phi1 = leg_right1->Pos + leg_right1->Offset;
//    Leg_R.phi4 = leg_right2->Pos + leg_right2->Offset;

//    Leg_R.dphi1 = leg_right1->Speed;
//    Leg_R.dphi4 = leg_right2->Speed;

//    get_legparam( Leg_R.phi1, Leg_R.phi4, &Leg_R.L0, &Leg_R.phi0 );

//    Leg_R.dphi0 = get_dphi0( Leg_R.phi1, Leg_R.phi4, Leg_R.dphi1, Leg_R.dphi4 );

//    Leg_R.dx = motor_right->filter_rate;

//    //    if ( chassis_mode == chassis_weak || B.IF_Offground ) {
//    //        Leg_R.x = 0;
//    //    }
//    //    else {
//    //        STOP_X( &Leg_R );
//    //    }

//    /*右腿机体角度与速度*/
//    Leg_R.phi  = -imuAngle[ 1 ];
//    Leg_R.dphi = -gyro[ 1 ];

//    /*右腿摆杆角度*/
//    Leg_R.theata = PI2 - Leg_R.phi0 - Leg_R.phi;

//    /*右腿摆杆速度*/
//    Leg_R.dtheata = -Leg_R.dphi0 - Leg_R.dphi;

//    /*右腿摆杆加速度*/
//    Leg_R.ddtheata     = ( Leg_R.dtheata - Leg_R.last_dtheata ) / R_dt;
//    Leg_R.last_dtheata = Leg_R.dtheata;

//    /*右腿伸展速度*/
//    Leg_R.dL0     = ( Leg_R.L0 - Leg_R.last_L0 ) / R_dt;
//    Leg_R.last_L0 = Leg_R.L0;

//    /*右腿伸展加速度*/
//    Leg_R.ddL0     = ( Leg_R.dL0 - Leg_R.last_dL0 ) / R_dt;
//    Leg_R.last_dL0 = Leg_R.dL0;

//    lqr_k( Leg_R.L0, K_Ratio_R );  // 计算拟合LQR_L的K增益
//}

//int16_t err_Angle_R;
//double  Right_1_8009_Motor_OUT = 0.0f;
//double  Right_4_8009_Motor_OUT = 0.0f;	
//void Chassis_Normol_R(void)
//{
//	static float gravity_comp = 100.0f;
//	static float L0 ;

//	Chassis_cal(&Leg_R,K_Ratio_R);
//	
//	
//	pid_calc(&Body.Feedback_phi0,Leg_R.phi0-Leg_L.phi0,0);

////		err_Angle_R = angle_z_min_err_get(FollowAngle);
////		target_speed.vr = err_Angle_R * 0.003f;
////		LIMIT_MIN_MAX(target_speed.vr,-5.0f,5.0f);
//////		turn = err_Angle_R * 0.00076f * 10.0f - 0.3f * B.INS_gyro[2];
////	
////	pid_calc(&Body.Yaw_feed,Body.INS_gyro[2],-target_speed.vr);
//	
//	
////	if(Body.IF_Offground)
////	{
////		Leg_R.F0 = 	pid_calc(&Leg_R.legLength_PID,Leg_R.L0,target_speed.targetlengh) + gravity_comp;
////	}
////	else
////	{
//		L0 = target_speed.targetlengh - pid_calc(&Body.Roll_feed,Body.INS_angle[1],0.0f);
//		LIMIT_MIN_MAX(L0,0.08f,0.36f);
//		Leg_R.Tp =  Leg_R.Tp + Body.Feedback_phi0.pos_out;
//		
////		if(Leg_R.Jump_Flag == 1||Leg_R.Jump_Flag == 2||Leg_R.Jump_Flag == 3 || Leg_R.Jump_Flag ==4)
////		{
////			if(Leg_R.Jump_Flag ==4) 
////			{
////				Leg_R.F0 = 	pid_calc(&Leg_R.legLength_PID,Leg_R.L0,0.15f);
////				Leg_R.T = Leg_R.T - B.Yaw_feed.pos_out;
////			}
////			else JUMP_Function_R();
////		}
////		else 
////		{
//			Leg_R.F0 = 	pid_calc(&Leg_R.legLength_PID,Leg_R.L0,L0)+ gravity_comp;
//			Leg_R.T = Leg_R.T - Body.Yaw_feed.pos_out;
////		}
//	
//	

//	
//	VM_cal(Leg_R.F0, Leg_R.Tp, Leg_R.phi1, Leg_R.phi4, &Leg_R.T1,&Leg_R.T2);
//	
//	Wheel_R.Motorout = -Leg_R.T / Body.torque_const_3508 * (16384.0f / 20.0f) / (268.0f / 17.0f);	
//	LIMIT_MIN_MAX(Wheel_R.Motorout,-16384.0f,16384.0f);
//	
//	Right_1_8009_Motor_OUT = Leg_R.T1;
//	Right_4_8009_Motor_OUT = Leg_R.T2;
//	
//	mit_ctrl(&hcan1,&DM_R_Phi1,0,0,0,0,Right_1_8009_Motor_OUT);osDelay(CHASSL_TIME_R);
//	mit_ctrl(&hcan1,&DM_R_Phi4,0,0,0,0,Right_4_8009_Motor_OUT);osDelay(CHASSL_TIME_R);
//	
//	
//	Chassis_3508Motorout_R();
//}	
