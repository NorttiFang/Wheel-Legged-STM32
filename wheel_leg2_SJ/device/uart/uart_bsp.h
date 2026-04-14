#ifndef __UART_BSP_H__
#define __UART_BSP_H__

#include "main.h"

#define BUFF_SIZE 32

#define SWA_UP 1000
#define SWA_DOWN 2000

#define SWB_UP 1000
#define SWB_DOWN 2000

#define SWC_UP 1000
#define SWC_MID 1500
#define SWC_DOWN 2000

#define SWD_UP 1000
#define SWD_DOWN 2000

extern uint8_t rx_buff[ BUFF_SIZE ];

typedef struct {
    uint16_t online;

    struct {
        int16_t ch[ 14 ];
    } rc;

    struct {
        /* STICK VALUE */
        int16_t left_vert;
        int16_t left_hori;
        int16_t right_vert;
        int16_t right_hori;
    } joy;

    struct {
        /* VAR VALUE */
        int16_t a;
        int16_t b;
    } var;

    struct {
        /* KEY VALUE */
        int16_t a;
        int16_t b;
        int16_t c;
        int16_t d;

    } key;
} remoter_t;

remoter_t* Get_remoter_Sbus( void );
void Handle_date(void);

#endif /*__UART_BSP_H__ */
