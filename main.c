/*
 * main.c
 *
 *  Created on: Apr 5, 2020
 *      Author: SUNDAR
 */


#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include <MKL25Z4.h>
#include "uart_buffer_rx.h"
#include "uart_buffer_tx.h"

int main(void)
{

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    int16_t a = 0;
    int16_t b = 0;

    PRINTF("Buffer before init check : %d\n\r", is_buffer_init_rx() == RX_BUFF_INIT_FALSE);

    buffer_init_rx();

    PRINTF("Buffer after init check : %d\n\r", is_buffer_init_rx() == RX_BUFF_INIT_TRUE);

    while((write_to_buffer_rx('H') == RX_NO_ERROR) && (a < 600))
    {

    	a++;
    }

    PRINTF("Buffer full check : %d\n\r" , is_buffer_full_rx() == RX_BUFFER_FULL);

    while(read_from_buffer_rx() > ERR_CODE_LIMIT)
    {

    	b++;
    }

    PRINTF("Buffer empty check : %d\n\r" , is_buffer_empty_rx() == RX_BUFFER_EMPTY);

    PRINTF("a value : %d\n\r", a);
    PRINTF("b value : %d\n\r", b);

    while(1);



    return 0;
}


