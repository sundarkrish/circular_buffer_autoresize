#ifndef __UART_BUFFER_RX_H__
#define __UART_BUFFER_RX_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ERR_CODE_LIMIT 8

typedef struct
{
	unsigned char data;
	uint8_t not_empty;

} buffer_rx_typedef;


/*ERROR CODES :

RX_BUFFER_EMPTY  0
RX_BUFFER_NON_EMPTY 1
RX_NO_ERROR 2
RX_BUFFER_FULL 3
RX_BUFF_PTR_VALID 4
RX_BUFF_PTR_INVALID 5
RX_BUFF_INIT_TRUE 6
RX_BUFF_INIT_FALSE 7
RX_RESIZE_FAIL 8

*/

typedef enum
{

	RX_BUFFER_EMPTY = 0,
	RX_BUFFER_NON_EMPTY,
	RX_NO_ERROR,
	RX_BUFFER_FULL,
	RX_BUFF_PTR_VALID,
	RX_BUFF_PTR_INVALID,
	RX_BUFF_INIT_TRUE,
	RX_BUFF_INIT_FALSE,
	RX_RESIZE_FAIL
}rx_error_status;

rx_error_status is_buffer_full_rx(void);
rx_error_status is_buffer_empty_rx(void);
rx_error_status is_buffer_init_rx(void);
rx_error_status verify_buffer_ptr_rx(void);
void  buffer_init_rx(void);
void buffer_destroy_rx(void);
rx_error_status write_to_buffer_rx(unsigned char  val);
unsigned char read_from_buffer_rx();
rx_error_status write_to_buffer_rx(unsigned char );



#endif /* __UART_BUFFER_RX_H__ */
