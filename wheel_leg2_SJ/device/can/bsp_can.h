#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__

#include "fdcan.h"

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#define Master_CAN_ID 0x1F

#define RATE_BUF_SIZE 6
#define MOTOR_CHASSIS_RIGHT_ID 0X201
#define MOTOR_CHASSIS_LEFT_ID 0X202
#define MOTOR_LEG_LEFT_ID1 0x001
#define MOTOR_LEG_LEFT_ID2 0x002
//#define MOTOR_LEG_RIGHT_ID1 0x004
//#define MOTOR_LEG_RIGHT_ID2 0x003
#define MOTOR_LEG_RIGHT_ID1 0x003
#define MOTOR_LEG_RIGHT_ID2 0x004
#define MOTOR_LEG_RIGHT_test 0x005

#define Communication_Type_MotionControl 0x01           // 运控模式
#define Communication_Type_MotorEnable 0x03             // 电机使能
#define Communication_Type_MotorStop 0x04               // 电机失能
#define Communication_Type_SetPosZero 0x06              // 电机设置0点
#define Communication_Type_Can_ID 0x07                  // 电机设置ID
#define Communication_Type_Control_Mode 0x12            // 设置电机模式
#define Communication_Type_ProactiveEscalationSet 0x18  // 设置电机主动上报

#define P_MIN -12.5f
#define P_MAX 12.5f
#define V_MIN -44.0f
#define V_MAX 44.0f
#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 5.0f
#define T_MIN -17.0f
#define T_MAX 17.0f

#define U16TO2PI 0.0000958738F
#define SPEED_MAX 40.0F
#define U16TOSPEED SPEED_MAX / 65536.0F

typedef struct {
    int32_t raw_value;                  // 编码器不经处理的原始值
    int32_t last_raw_value;             // 上一次的编码器原始值
    int32_t ecd_value;                  // 经过处理后连续的编码器值
    int32_t diff;                       // 两次编码器之间的差值
    int32_t temp_count;                 // 计数用
    uint8_t buf_count;                  // 滤波更新buf用
    int32_t ecd_bias;                   // 初始编码器值
    int32_t ecd_raw_rate;               // 通过编码器计算得到的速度原始值
    int32_t rate_buf[ RATE_BUF_SIZE ];  // buf，for filter
    int32_t round_cnt;                  // 圈数
    int16_t filter_rate;                // 速度
    float   ecd_angle;                  // 角度
		int16_t Motorout;
} Encoder;

typedef struct {
    float Speed;
    float Pos;
    float Offset;
    float Torque;
    float Temp;
    float error_code;
    float pattern;
		float jointoffset;
} Encoder_Leg;

void         getEncoderData( volatile Encoder* v, uint8_t* msg );
void         GetLegEncoderData( volatile Encoder_Leg* encoder_leg, uint8_t* msg );
void         can_filter_init( void );
uint8_t      Set_ChassisMotor_Current( FDCAN_HandleTypeDef* hfdcan, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq );
void         bsp_can_init( void );
Encoder*     Get_Motor_right( void );
Encoder*     Get_Motor_left( void );
Encoder_Leg* Get_Leg_Motor_right1( void );
Encoder_Leg* Get_Leg_Motor_right2( void );
Encoder_Leg* Get_Leg_Motor_left1( void );
Encoder_Leg* Get_Leg_Motor_left2( void );
void LEG_Motor_Analysis( uint32_t* rec_id, uint8_t* buf, volatile Encoder_Leg* encoder );


void LEG_Disenable_Motor( FDCAN_HandleTypeDef* hfdcan, uint8_t CAN_ID );
void LEG_Enable_Motor( FDCAN_HandleTypeDef* hfdcan, uint8_t CAN_ID ) ;
void LEG_Motor_move_control( FDCAN_HandleTypeDef* hfdcan, uint8_t CAN_ID, float Torque, float Angle, float Speed, float Kp, float Kd );
void LEG_Set_ZeroPos( FDCAN_HandleTypeDef* hfdcan, uint8_t CAN_ID );


extern Encoder_Leg leg_right1, leg_right2, leg_left1, leg_left2;
extern Encoder     motor_right, motor_left;
#endif
