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





Leg_Posture Leg[2];	
Body_Posture  Body;
Target_Speed target_speed;
