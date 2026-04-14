#include "Leg_Task.h"
#include "Chassis_Control.h"

extern double MotionAccel_b[ 3 ];


void	FN_Cale( Leg_Posture* Leg ) {
    double P1, P2;
    P1 = Leg->F0 * cos( Leg->theata ) + ( Leg->Tp * sin( Leg->theata ) ) / Leg->L0;

    Leg->z_dot = MotionAccel_b[ 2 ] - Leg->ddL0 * cos( Leg->theata ) + 2 * Leg->dL0 * Leg->dtheata * sin( Leg->theata ) + Leg->L0 * Leg->ddtheata * sin( Leg->theata )
                 + Leg->L0 * Leg->dtheata * Leg->dtheata * cos( Leg->theata );

    Leg->aver[ 0 ] = Leg->aver[ 1 ];
    Leg->aver[ 1 ] = Leg->aver[ 2 ];
    Leg->aver[ 2 ] = Leg->aver[ 3 ];
    Leg->aver[ 3 ] = P1 + Body.mw * 9.8f + Body.mw * Leg->z_dot;

    Leg->Fn = 0.25f * Leg->aver[ 0 ] + 0.25f * Leg->aver[ 1 ] + 0.25f * Leg->aver[ 2 ] + 0.25f * Leg->aver[ 3 ];  // 对支持力进行均值滤波

    if ( Leg->Fn < 5 )
        Leg->off = 1;
    else
        Leg->off = 0;
}


void Chassis_cal(Leg_Posture * Leg, double * K_Ratio)
{
	if(Body.IF_Offground)
	{
		Leg->T = 0;
		Leg->Tp = K_Ratio[1] * Leg->theata + K_Ratio[3] * Leg->dtheata;
	}
	else
	{
//		if(chassis_mode == chassis_down)
//		{
//			Leg->T = K_Ratio[0] * Leg->theata + K_Ratio[2] * Leg->dtheata + K_Ratio[8] * Leg->phi + K_Ratio[10] * Leg->dphi; // + K_Ratio[4] * Leg->x + K_Ratio[6] * (Leg->dx + Body.target_v) 
//			Leg->Tp = K_Ratio[1] * Leg->theata + K_Ratio[3] * Leg->dtheata + K_Ratio[9] * Leg->phi + K_Ratio[11] * Leg->dphi; //  + K_Ratio[5] * Leg->x + K_Ratio[7] * (Leg->dx + Body.target_v)
//		}
//		else
//		{
			Leg->T = K_Ratio[0] * Leg->theata + K_Ratio[2] * Leg->dtheata + K_Ratio[4] * Leg->x + K_Ratio[6] * (Leg->dx + Body.target_v) + K_Ratio[8] * Leg->phi + K_Ratio[10] * Leg->dphi;
			Leg->Tp = K_Ratio[1] * Leg->theata + K_Ratio[3] * Leg->dtheata + K_Ratio[5] * Leg->x + K_Ratio[7] * (Leg->dx + Body.target_v) + K_Ratio[9] * Leg->phi + K_Ratio[11] * Leg->dphi;
//		}

		
	}

}

Leg_Posture Leg_L,Leg_R;	
Body_Posture  Body;
Target_Speed target_speed;
