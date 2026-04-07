#include "uart_bsp.h"
#include "string.h"
#include "usart.h"
#include "Chassis_Control.h"

#define SBUS_HEAD1 0X20
#define SBUS_HEAD2 0X40
#define IBUS_MAX_CHANNLES 14

uint8_t   rx_buff[ BUFF_SIZE ];
static remoter_t remoter;

void sbus_frame_parse( remoter_t* remoter, uint8_t* buf ) {
    if ( ( buf[ 0 ] != SBUS_HEAD1 ) || ( buf[ 1 ] != SBUS_HEAD2 ) )
        return;

    if ( buf[ 1 ] == SBUS_HEAD2 )
        remoter->online = 1;
    else
        remoter->online = 0;

    for ( int i = 0; i < IBUS_MAX_CHANNLES; i++ )
        remoter->rc.ch[ i ] = ( uint16_t )( buf[ i * 2 + 3 ] << 8 | buf[ i * 2 + 2 ] );

    remoter->var.a          = remoter->rc.ch[ 4 ];
    remoter->var.b          = remoter->rc.ch[ 5 ];
    remoter->joy.left_hori  = remoter->rc.ch[ 3 ] - 1500;
    remoter->joy.left_vert  = remoter->rc.ch[ 2 ] - 1500;
    remoter->joy.right_hori = remoter->rc.ch[ 0 ] - 1500;
    remoter->joy.right_vert = remoter->rc.ch[ 1 ] - 1500;
    remoter->key.a          = remoter->rc.ch[ 6 ];
    remoter->key.b          = remoter->rc.ch[ 7 ];
    remoter->key.c          = remoter->rc.ch[ 8 ];
    remoter->key.d          = remoter->rc.ch[ 9 ];
}

void HAL_UARTEx_RxEventCallback( UART_HandleTypeDef* huart, uint16_t Size ) {
    if ( huart->Instance == UART5 ) {
        if ( Size <= BUFF_SIZE ) {
            HAL_UARTEx_ReceiveToIdle_DMA( &huart5, rx_buff, BUFF_SIZE * 2 );  // 接收完毕后重启
            sbus_frame_parse( &remoter, rx_buff );
						Handle_date();
					
            //			memset(rx_buff, 0, BUFF_SIZE);
        }
        else  // 接收数据长度大于BUFF_SIZE，错误处理
        {
            HAL_UARTEx_ReceiveToIdle_DMA( &huart5, rx_buff, BUFF_SIZE * 2 );  // 接收完毕后重启
            memset( rx_buff, 0, BUFF_SIZE );
        }
    }
}

void HAL_UART_ErrorCallback( UART_HandleTypeDef* huart ) {
    if ( huart->Instance == UART5 ) {
        HAL_UARTEx_ReceiveToIdle_DMA( &huart5, rx_buff, BUFF_SIZE * 2 );  // 接收发生错误后重启
        memset( rx_buff, 0, BUFF_SIZE );                                  // 清除接收缓存
    }
}

remoter_t* Get_remoter_Sbus( void ) {
    return &remoter;
}

void Handle_date(void)
{
//	if(remoter.key.b == SWD_UP)
//	{
//		chassis_mode = chassis_weak;
//	}
//	else if(remoter.key.b == SWD_DOWN)
//	{
//		// chassis_mode = chassis_down;
//		 chassis_mode = chassis_normal;
//	}
	
	if  		(remoter.key.c == SWC_UP) 		{ chassis_mode = chassis_weak;}
	else if	(remoter.key.c == SWC_MID)  	{ chassis_mode = chassis_down;}
	else if	(remoter.key.c == SWC_DOWN)  	{ chassis_mode = chassis_normal;}
	
	if  		(remoter.key.d == SWC_UP)     {chassis_mode = chassis_weak;}
}


