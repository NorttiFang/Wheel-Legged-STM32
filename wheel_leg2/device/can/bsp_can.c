#include "bsp_can.h"

Encoder     motor_right, motor_left;
Encoder_Leg leg_right1, leg_right2, leg_left1, leg_left2;

Encoder* Get_Motor_right( void ) {
    return &motor_right;
}

Encoder* Get_Motor_left( void ) {
    return &motor_left;
}

/*float转int型*/
int float_to_uint( float x, float x_min, float x_max, int bits ) {
    float span   = x_max - x_min;
    float offset = x_min;
    if ( x > x_max )
        x = x_max;
    else if ( x < x_min )
        x = x_min;
    return ( int )( ( x - offset ) * ( ( float )( ( 1 << bits ) - 1 ) ) / span );
}

float uint16_to_float( uint16_t x, float x_min, float x_max, int bits ) {
    uint32_t span = ( 1 << bits ) - 1;
    x &= span;
    float offset = x_max - x_min;
    return offset * x / span + x_min;
}

uint8_t mapFaults( uint16_t fault16 ) {
    uint8_t fault8 = 0;

    if ( fault16 & ( 1 << 14 ) )
        fault8 |= ( 1 << 4 );  // 过载
    if ( fault16 & ( 1 << 7 ) )
        fault8 |= ( 1 << 5 );  // 未标定
    if ( fault16 & ( 1 << 3 ) )
        fault8 |= ( 1 << 3 );  // 磁编故障
    if ( fault16 & ( 1 << 2 ) )
        fault8 |= ( 1 << 0 );  // 欠压
    if ( fault16 & ( 1 << 1 ) )
        fault8 |= ( 1 << 1 );  // 驱动故障
    if ( fault16 & ( 1 << 0 ) )
        fault8 |= ( 1 << 2 );  // 过温

    return fault8;
}

Encoder_Leg* Get_Leg_Motor_right1( void ) {
    return &leg_right1;
}
Encoder_Leg* Get_Leg_Motor_right2( void ) {
    return &leg_right2;
}
Encoder_Leg* Get_Leg_Motor_left1( void ) {
    return &leg_left1;
}
Encoder_Leg* Get_Leg_Motor_left2( void ) {
    return &leg_left2;
}

void bsp_can_init( void ) {
    can_filter_init();

    HAL_FDCAN_ActivateNotification( &hfdcan3, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0 );
    HAL_FDCAN_Start( &hfdcan3 );

    HAL_FDCAN_ActivateNotification( &hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0 );
    HAL_FDCAN_Start( &hfdcan1 );

    HAL_FDCAN_ActivateNotification( &hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0 );
    HAL_FDCAN_Start( &hfdcan2 );
}

/**
************************************************************************
* @brief:      	can_filter_init(void)
* @param:       void
* @retval:     	void
* @details:    	CAN滤波器初始化
************************************************************************
**/
void can_filter_init( void ) {
    FDCAN_FilterTypeDef fdcan_filter;
		FDCAN_FilterTypeDef fdcan_filter1;

    fdcan_filter.IdType       = FDCAN_EXTENDED_ID;  // 标准ID
    fdcan_filter.FilterIndex  = 0;                  // 滤波器索引
    fdcan_filter.FilterType   = FDCAN_FILTER_MASK;
    fdcan_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;  // 过滤器0关联到FIFO0
    fdcan_filter.FilterID1    = 0x00;
    fdcan_filter.FilterID2    = 0x00;
	
		fdcan_filter1.IdType       = FDCAN_STANDARD_ID;  // 标准ID
    fdcan_filter1.FilterIndex  = 0;                  // 滤波器索引
    fdcan_filter1.FilterType   = FDCAN_FILTER_MASK;
    fdcan_filter1.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;  // 过滤器0关联到FIFO0
    fdcan_filter1.FilterID1    = 0x00;
    fdcan_filter1.FilterID2    = 0x00;

    HAL_FDCAN_ConfigFilter( &hfdcan3, &fdcan_filter1 );  // 接收ID2
    // 拒绝接收匹配不成功的标准ID和扩展ID,不接受远程帧
    HAL_FDCAN_ConfigGlobalFilter( &hfdcan3, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE );
    HAL_FDCAN_ConfigFifoWatermark( &hfdcan3, FDCAN_CFG_RX_FIFO1, 1 );


    HAL_FDCAN_ConfigFilter( &hfdcan1, &fdcan_filter );  // 接收ID2
    // 拒绝接收匹配不成功的标准ID和扩展ID,不接受远程帧
    HAL_FDCAN_ConfigGlobalFilter( &hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE );
    HAL_FDCAN_ConfigFifoWatermark( &hfdcan1, FDCAN_CFG_RX_FIFO0, 1 );

    HAL_FDCAN_ConfigFilter( &hfdcan2, &fdcan_filter );  // 接收ID2
    // 拒绝接收匹配不成功的标准ID和扩展ID,不接受远程帧
    HAL_FDCAN_ConfigGlobalFilter( &hfdcan2, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE );
    HAL_FDCAN_ConfigFifoWatermark( &hfdcan2, FDCAN_CFG_RX_FIFO0, 1 );
    //	HAL_FDCAN_ConfigFifoWatermark(&hfdcan1, FDCAN_CFG_RX_FIFO1, 1);
    // HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_TX_COMPLETE, FDCAN_TX_BUFFER0);
}

uint8_t Set_ChassisMotor_Current( FDCAN_HandleTypeDef* hfdcan, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq ) {
    uint8_t Data[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    FDCAN_TxHeaderTypeDef pTxHeader;
    pTxHeader.Identifier          = 0X200;
    pTxHeader.IdType              = FDCAN_STANDARD_ID;
    pTxHeader.TxFrameType         = FDCAN_DATA_FRAME;
    pTxHeader.DataLength          = FDCAN_DLC_BYTES_8;
    pTxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    pTxHeader.BitRateSwitch       = FDCAN_BRS_OFF;
    pTxHeader.FDFormat            = FDCAN_CLASSIC_CAN;
    pTxHeader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
    pTxHeader.MessageMarker       = 0;
    Data[ 0 ]                     = ( uint8_t )( cm1_iq >> 8 );
    Data[ 1 ]                     = ( uint8_t )cm1_iq;
    Data[ 2 ]                     = ( uint8_t )( cm2_iq >> 8 );
    Data[ 3 ]                     = ( uint8_t )cm2_iq;
    Data[ 4 ]                     = ( uint8_t )( cm3_iq >> 8 );
    Data[ 5 ]                     = ( uint8_t )cm3_iq;
    Data[ 6 ]                     = ( uint8_t )( cm4_iq >> 8 );
    Data[ 7 ]                     = ( uint8_t )cm4_iq;
    if ( HAL_FDCAN_AddMessageToTxFifoQ( hfdcan, &pTxHeader, Data ) != HAL_OK )
        return 1;  // 发送
    return 0;
}

uint8_t Leg_Can_EXT_Send( FDCAN_HandleTypeDef* hfdcan, uint32_t CAN_ID, uint8_t* Data ) {
    FDCAN_TxHeaderTypeDef pTxHeader;
    pTxHeader.Identifier          = CAN_ID;
    pTxHeader.IdType              = FDCAN_EXTENDED_ID;
    pTxHeader.TxFrameType         = FDCAN_DATA_FRAME;
    pTxHeader.DataLength          = FDCAN_DLC_BYTES_8;
    pTxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    pTxHeader.BitRateSwitch       = FDCAN_BRS_OFF;
    pTxHeader.FDFormat            = FDCAN_CLASSIC_CAN;
    pTxHeader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
    pTxHeader.MessageMarker       = 0;

    if ( HAL_FDCAN_AddMessageToTxFifoQ( hfdcan, &pTxHeader, Data ) != HAL_OK )
        return 1;  // 发送
    return 0;
}

void CanReceiveMsgProcess( FDCAN_HandleTypeDef* hfdcan, uint16_t* rec_id, uint8_t* buf )  // 接受数据 将四个电机分类储存数据
{
    FDCAN_RxHeaderTypeDef pRxHeader;
    if ( HAL_FDCAN_GetRxMessage( hfdcan, FDCAN_RX_FIFO1, &pRxHeader, buf ) == HAL_OK ) {
        *rec_id = pRxHeader.Identifier;
        if ( *rec_id == MOTOR_CHASSIS_RIGHT_ID )
            getEncoderData( &motor_right, buf );
        else if ( *rec_id == MOTOR_CHASSIS_LEFT_ID )
            getEncoderData( &motor_left, buf );
    }
}



void EXT_CanReceiveMsgProcess( FDCAN_HandleTypeDef* hfdcan, uint32_t* rec_id, uint8_t* buf )  // 接受数据 将四个电机分类储存数据
{
    FDCAN_RxHeaderTypeDef pRxHeader;
    if ( HAL_FDCAN_GetRxMessage( hfdcan, FDCAN_RX_FIFO0, &pRxHeader, buf ) == HAL_OK ) {
        *rec_id = pRxHeader.Identifier;
        if ( (( ( int )( ( *rec_id & 0x3F000000 ) >> 24 ) == 2 ))&& ( ( *rec_id & 0xFF ) == Master_CAN_ID ) ) {
            switch ( ( uint8_t )( ( *rec_id & 0xFF00 ) >> 8 ) ) {
            case MOTOR_LEG_LEFT_ID1:
                /* code */
                LEG_Motor_Analysis( rec_id, buf, &leg_left1 );
                break;
            case MOTOR_LEG_LEFT_ID2:
                /* code */
                LEG_Motor_Analysis( rec_id, buf, &leg_left2 );
                break;
            case MOTOR_LEG_RIGHT_ID1:
                /* code */
                LEG_Motor_Analysis( rec_id, buf, &leg_right1 );
                break;
            case MOTOR_LEG_RIGHT_ID2:
                /* code */
                LEG_Motor_Analysis( rec_id, buf, &leg_right2 );
                break;
            default:
                break;
            }
        }
    }
}

void GetLegEncoderData( volatile Encoder_Leg* encoder_leg, uint8_t* msg ) {}

void getEncoderData( volatile Encoder* encoder, uint8_t* msg )  // 编码器解算
{
    encoder->filter_rate = (uint16_t)(( msg[ 2 ] << 8 ) | msg[ 3 ]);
}

int b;
    uint16_t rec_id3;
    uint8_t  rx_data3[ 8 ];
void fdcan3_rx_callback( void ) {
//		b++;
//    uint16_t rec_id3;
//    uint8_t  rx_data3[ 8 ];
    CanReceiveMsgProcess( &hfdcan3, &rec_id3, rx_data3 );
}


uint32_t rec_id1;
uint8_t  rx_data1[ 8 ];
    
void fdcan1_rx_callback( void ) {
//    uint32_t rec_id1;
//		uint8_t  rx_data1[ 8 ];
    EXT_CanReceiveMsgProcess( &hfdcan1, &rec_id1, rx_data1 );
}

uint32_t rec_id2;
uint8_t  rx_data2[ 8 ];
//int aaa=0;
void fdcan2_rx_callback( void ) {
//    uint32_t rec_id2;
//		uint8_t  rx_data2[ 8 ];
//	aaa++;
    EXT_CanReceiveMsgProcess( &hfdcan2, &rec_id2, rx_data2 );
}

void HAL_FDCAN_RxFifo0Callback( FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs ) {

	
    if ( hfdcan == &hfdcan1 )
		{
        fdcan1_rx_callback();
		}

    if ( hfdcan == &hfdcan2 )
        fdcan2_rx_callback();

//    if ( hfdcan == &hfdcan3 )
//        fdcan3_rx_callback();
}


void HAL_FDCAN_RxFifo1Callback( FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs ) {
	    if ( hfdcan == &hfdcan3 )
        fdcan3_rx_callback();
}

/*电机使能*/
void LEG_Enable_Motor( FDCAN_HandleTypeDef* hfdcan, uint8_t CAN_ID ) {
    uint8_t  txdata[ 8 ] = { 0 };
    uint32_t ext_id;
    ext_id = Communication_Type_MotorEnable << 24 | Master_CAN_ID << 8 | CAN_ID;
    Leg_Can_EXT_Send( hfdcan, ext_id, txdata );
}

/*电机失能*/
void LEG_Disenable_Motor( FDCAN_HandleTypeDef* hfdcan, uint8_t CAN_ID ) {

    uint8_t  txdata[ 8 ] = { 0 };
    uint32_t ext_id;
    txdata[ 0 ] = 1;
    ext_id      = Communication_Type_MotorStop << 24 | Master_CAN_ID << 8 | CAN_ID;
    Leg_Can_EXT_Send( hfdcan, ext_id, txdata );
}

/*电机设置0位*/
void LEG_Set_ZeroPos( FDCAN_HandleTypeDef* hfdcan, uint8_t CAN_ID ) {

    uint8_t  txdata[ 8 ] = { 0 };
    uint32_t ext_id;
    LEG_Disenable_Motor( hfdcan, CAN_ID );
    txdata[ 0 ] = 1;
    ext_id      = Communication_Type_SetPosZero << 24 | Master_CAN_ID << 8 | CAN_ID;
    Leg_Can_EXT_Send( hfdcan, ext_id, txdata );
    LEG_Enable_Motor( hfdcan, CAN_ID );
}

/*电机主动上报 默认10ms一次 cmd为1开启主动上报*/
void LEG_ProactiveEscalationSet( FDCAN_HandleTypeDef* hfdcan, uint8_t CAN_ID, uint8_t F_CMD ) {
    uint8_t  txdata[ 8 ] = { 0 };  //
    uint32_t ext_id;
    ext_id      = Communication_Type_ProactiveEscalationSet << 24 | Master_CAN_ID << 8 | CAN_ID;
    txdata[ 0 ] = 0x01;
    txdata[ 1 ] = 0x02;
    txdata[ 2 ] = 0x03;
    txdata[ 3 ] = 0x04;
    txdata[ 4 ] = 0x05;
    txdata[ 5 ] = 0x06;
    txdata[ 6 ] = F_CMD;  // 0关闭主动上报，1开启 10ms上报一次
    txdata[ 7 ] = 0x08;
    Leg_Can_EXT_Send( hfdcan, ext_id, txdata );
}

/*关节部分控制下发*/
void LEG_Motor_move_control( FDCAN_HandleTypeDef* hfdcan, uint8_t CAN_ID, float Torque, float Angle, float Speed, float Kp, float Kd ) {
    uint8_t  txdata[ 8 ] = { 0 };
    uint32_t ext_id;

    ext_id      = Communication_Type_MotionControl << 24 | float_to_uint( Torque, T_MIN, T_MAX, 16 ) << 8 | CAN_ID;
    txdata[ 0 ] = float_to_uint( Angle, P_MIN, P_MAX, 16 ) >> 8;
    txdata[ 1 ] = float_to_uint( Angle, P_MIN, P_MAX, 16 );
    txdata[ 2 ] = float_to_uint( Speed, V_MIN, V_MAX, 16 ) >> 8;
    txdata[ 3 ] = float_to_uint( Speed, V_MIN, V_MAX, 16 );
    txdata[ 4 ] = float_to_uint( Kp, KP_MIN, KP_MAX, 16 ) >> 8;
    txdata[ 5 ] = float_to_uint( Kp, KP_MIN, KP_MAX, 16 );
    txdata[ 6 ] = float_to_uint( Kd, KD_MIN, KD_MAX, 16 ) >> 8;
    txdata[ 7 ] = float_to_uint( Kd, KD_MIN, KD_MAX, 16 );
    Leg_Can_EXT_Send( hfdcan, ext_id, txdata );
}

/*接收角度分析*/
void LEG_Motor_Analysis( uint32_t* rec_id, uint8_t* buf, volatile Encoder_Leg* encoder ) {
    encoder->Pos        = uint16_to_float( ( buf[ 0 ] << 8 ) | ( buf[ 1 ] ), P_MIN, P_MAX, 16 );
    encoder->Speed      = uint16_to_float( buf[ 2 ] << 8 | buf[ 3 ], V_MIN, V_MAX, 16 );
    encoder->Torque     = uint16_to_float( buf[ 4 ] << 8 | buf[ 5 ], T_MIN, T_MAX, 16 );
    encoder->Temp       = ( buf[ 6 ] << 8 | buf[ 7 ] ) * 0.1;
    encoder->error_code = ( uint8_t )( ( *rec_id & 0x3F0000 ) >> 16 );
    encoder->pattern    = ( uint8_t )( ( *rec_id & 0xC00000 ) >> 22 );
}


