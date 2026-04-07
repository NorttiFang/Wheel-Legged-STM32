#include "imu_task.h"
#include "MahonyAHRS.h"
#include "bmi088driver.h"
#include "bsp_can.h"
#include <math.h>

#define DES_TEMP 40.0f
#define KP 100.f
#define KI 50.f
#define KD 10.f
#define MAX_OUT 500

float        gyro[ 3 ]          = { 0.0f };
float        gyro_raw[ 3 ]      = { 0.0f };
float        acc[ 3 ]           = { 0.0f };
float        gyro_zero_out[ 3 ] = { 0.0f };
static float temp               = 0.0f;

float imuQuat[ 4 ]  = { 0.0f };
float imuAngle[ 3 ] = { 0.0f };
int   time_imu      = 0;
float YawTotalAngle,YawAngleLast=0.0f;
int YawRoundCount;


const float xb[3] = {1, 0, 0};
const float yb[3] = {0, 1, 0};
const float zb[3] = {0, 0, 1};

// 加速度在绝对系的向量表示
float xn[3];
float yn[3];
float zn[3];

float MotionAccel_b[3]; // 机体坐标加速度
float MotionAccel_n[3]; // 绝对系加速度


float* Get_Gyro( void ) {
    return gyro;
}

void AHRS_init( float quat[ 4 ] ) {
    quat[ 0 ] = 1.0f;
    quat[ 1 ] = 0.0f;
    quat[ 2 ] = 0.0f;
    quat[ 3 ] = 0.0f;
}

void AHRS_update( float quat[ 4 ], float gyro[ 3 ], float accel[ 3 ] ) {
    MahonyAHRSupdateIMU( quat, gyro[ 0 ], gyro[ 1 ], gyro[ 2 ], accel[ 0 ], accel[ 1 ], accel[ 2 ] );
}

void GetAngle( float q[ 4 ], float* yaw, float* pitch, float* roll ) {
    *yaw   = atan2f( 2.0f * ( q[ 0 ] * q[ 3 ] + q[ 1 ] * q[ 2 ] ), 2.0f * ( q[ 0 ] * q[ 0 ] + q[ 1 ] * q[ 1 ] ) - 1.0f );
    *pitch = asinf( -2.0f * ( q[ 1 ] * q[ 3 ] - q[ 0 ] * q[ 2 ] ) );
    *roll  = atan2f( 2.0f * ( q[ 0 ] * q[ 1 ] + q[ 2 ] * q[ 3 ] ), 2.0f * ( q[ 0 ] * q[ 0 ] + q[ 3 ] * q[ 3 ] ) - 1.0f );
}

/* USER CODE BEGIN Header_ImuTask_Entry */
/**
 * @brief Function implementing the ImuTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_ImuTask_Entry */

void ins_task_fun( void const* argument ) {
    /* USER CODE BEGIN ImuTask_Entry */
    const float  gravity[ 3 ] = { 0, 0, 9.81f };
    const double AccelLPF     = 0.0085;
    osDelay( 10 );
    while ( BMI088_init() ) {
        osDelay( 100 );
    }
    AHRS_init( imuQuat );
    /* Infinite loop */
    for ( ;; ) {
        BMI088_read( gyro_raw, acc, &temp );
        if ( time_imu >= 200 ) {
            for ( int i = 0; i < 3; i++ )
                gyro[ i ] = gyro_raw[ i ] - gyro_zero_out[ i ];
            AHRS_update( imuQuat, gyro, acc );
            GetAngle( imuQuat, imuAngle + INS_YAW_ADDRESS_OFFSET, imuAngle + INS_PITCH_ADDRESS_OFFSET, imuAngle + INS_ROLL_ADDRESS_OFFSET );
					
						if (imuAngle[0] - YawAngleLast > 3.1415926f)
						{
							YawRoundCount--;
						}
						else if (imuAngle[0] - YawAngleLast < -3.1415926f)
						{			
							YawRoundCount++;
						}
						YawTotalAngle = 6.283f* YawRoundCount + imuAngle[0];
						YawAngleLast = imuAngle[0];
					
						
						// 机体系基向量转换到导航坐标系，本例选取惯性系为导航系
						BodyFrameToEarthFrame(xb, xn, imuQuat);
						BodyFrameToEarthFrame(yb, yn, imuQuat);
						BodyFrameToEarthFrame(zb, zn, imuQuat);

						// 将重力从导航坐标系n转换到机体系b,随后根据加速度计数据计算运动加速度
						float gravity_b[3];
            EarthFrameToBodyFrame( gravity, gravity_b, imuQuat );
            for ( uint8_t i = 0; i < 3; i++ )  // 同样过一个低通滤波
                MotionAccel_b[ i ] = ( acc[ i ] - gravity_b[ i ] ) * 0.001f / ( AccelLPF + 0.001f ) + MotionAccel_b[ i ] * AccelLPF / ( AccelLPF + 0.001f );
					
						BodyFrameToEarthFrame(MotionAccel_b, MotionAccel_n, imuQuat); // 转换回导航系n
        }
        else if ( time_imu == 199 ) {
            for ( int i = 0; i < 3; i++ )
                gyro_zero_out[ i ] = gyro_zero_out[ i ] / 200.0f;
            time_imu++;
        }
        else {
            for ( int i = 0; i < 3; i++ )
                gyro_zero_out[ i ] += gyro_raw[ i ];
            time_imu++;
        }
        osDelay( 1 );
    }
    /* USER CODE END ImuTask_Entry */
}


/**
 * @brief          Transform 3dvector from BodyFrame to EarthFrame
 * @param[1]       vector in BodyFrame
 * @param[2]       vector in EarthFrame
 * @param[3]       quaternion
 */
void BodyFrameToEarthFrame(const float *vecBF, float *vecEF, float *q)
{
    vecEF[0] = 2.0f * ((0.5f - q[2] * q[2] - q[3] * q[3]) * vecBF[0] +
                       (q[1] * q[2] - q[0] * q[3]) * vecBF[1] +
                       (q[1] * q[3] + q[0] * q[2]) * vecBF[2]);

    vecEF[1] = 2.0f * ((q[1] * q[2] + q[0] * q[3]) * vecBF[0] +
                       (0.5f - q[1] * q[1] - q[3] * q[3]) * vecBF[1] +
                       (q[2] * q[3] - q[0] * q[1]) * vecBF[2]);

    vecEF[2] = 2.0f * ((q[1] * q[3] - q[0] * q[2]) * vecBF[0] +
                       (q[2] * q[3] + q[0] * q[1]) * vecBF[1] +
                       (0.5f - q[1] * q[1] - q[2] * q[2]) * vecBF[2]);
}



/**
 * @brief          Transform 3dvector from EarthFrame to BodyFrame
 * @param[1]       vector in EarthFrame
 * @param[2]       vector in BodyFrame
 * @param[3]       quaternion
 */
void EarthFrameToBodyFrame( const float* vecEF, float* vecBF, float* q ) {
    vecBF[ 0 ] = 2.0f * ( ( 0.5f - q[ 2 ] * q[ 2 ] - q[ 3 ] * q[ 3 ] ) * vecEF[ 0 ] + ( q[ 1 ] * q[ 2 ] + q[ 0 ] * q[ 3 ] ) * vecEF[ 1 ] + ( q[ 1 ] * q[ 3 ] - q[ 0 ] * q[ 2 ] ) * vecEF[ 2 ] );
    vecBF[ 1 ] = 2.0f * ( ( q[ 1 ] * q[ 2 ] - q[ 0 ] * q[ 3 ] ) * vecEF[ 0 ] + ( 0.5f - q[ 1 ] * q[ 1 ] - q[ 3 ] * q[ 3 ] ) * vecEF[ 1 ] + ( q[ 2 ] * q[ 3 ] + q[ 0 ] * q[ 1 ] ) * vecEF[ 2 ] );
    vecBF[ 2 ] = 2.0f * ( ( q[ 1 ] * q[ 3 ] + q[ 0 ] * q[ 2 ] ) * vecEF[ 0 ] + ( q[ 2 ] * q[ 3 ] - q[ 0 ] * q[ 1 ] ) * vecEF[ 1 ] + ( 0.5f - q[ 1 ] * q[ 1 ] - q[ 2 ] * q[ 2 ] ) * vecEF[ 2 ] );
}
