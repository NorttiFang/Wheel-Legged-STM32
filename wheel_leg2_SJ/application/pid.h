#ifndef __pid_H
#define __pid_H
/* Includes ------------------------------------------------------------------*/
#include "main.h"
enum{
    LLAST	= 0,
    LAST 	= 1,
    NOW 	= 2,
    
    WholeCircle,
    HalfCircle,
		
		PID_DELTA,
		PID_POSITION,
	
};
typedef struct __pid_t
{
    float p;
    float i;
    float d;
    
    float set[3];				//目标值,包含NOW， LAST， LLAST上上次
    float get[3];				//测量值
    float err[3];				//误差
	
    
    float pout;							//p输出
    float iout;							//i输出
    float dout;							//d输出
    
    float pos_out;						//本次位置式输出
    float last_pos_out;				//上次输出
    float delta_u;						//本次增量值
    float delta_out;					//本次增量式输出 = last_delta_out + delta_u
    float last_delta_out;
    
	  float max_err;
	  float deadband;				//err < deadband return
		uint32_t pid_formula;
    uint32_t pid_mode;
    float MaxOutput;				//输出限幅
    float IntegralLimit;		//积分限幅
    
    void (*f_param_init)(struct __pid_t *pid,  //PID参数初始化
										uint32_t pid_formula,
                    uint32_t pid_mode,
                    float maxOutput,
                    float integralLimit,
                    float p,
                    float i,
                    float d);
    void (*f_pid_reset)(struct __pid_t *pid, float p, float i, float d);		//pid三个参数修改

}pid_t;

typedef struct{
		float K1;			
	  float K2;
	  float Last_DeltIn;
	  float Now_DeltIn;
	  float Out;
	  float OutMax;
}FeedForward_Typedef;


void PID_struct_init(
    pid_t* pid,
    uint32_t formula,
    uint32_t mode,
    float maxout,
    float intergral_limit,
    
    float 	kp, 
    float 	ki, 
    float 	kd);
		
void FEeedForward_Init(FeedForward_Typedef *FF,
											 float k1,
											 float k2,
											 float OutMax);
  
float pid_calc(pid_t* pid, float fdb, float ref);
float auto_aim_pid_calc(pid_t* pid, float error);
float pid_calc_Limt(pid_t* pid, float fdb, float ref,float maxout);
float FeedForward_Calc(FeedForward_Typedef *FF);
void PID_cLear(pid_t* pid);											 
#endif

