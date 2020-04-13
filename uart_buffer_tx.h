#ifndef __UART_BUFFER_TX_H__
#define __UART_BUFFER_TX_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ERR_CODE_LIMIT 8

typedef struct
{
	unsigned char data;
	uint8_t not_empty;

} buffer_tx_typedef;


/*ERROR CODES :

TX_BUFFER_EMPTY  0
TX_BUFFER_NON_EMPTY 1
TX_NO_ERROR 2
TX_BUFFER_FULL 3
TX_BUFF_PTR_VALID 4
TX_BUFF_PTR_INVALID 5
TX_BUFF_INIT_TRUE 6
TX_BUFF_INIT_FALSE 7
TX_RESIZE_FAIL 8

*/

typedef enum
{

	TX_BUFFER_EMPTY = 0,
	TX_BUFFER_NON_EMPTY,
	TX_NO_ERROR,
	TX_BUFFER_FULL,
	TX_BUFF_PTR_VALID,
	TX_BUFF_PTR_INVALID,
	TX_BUFF_INIT_TRUE,
	TX_BUFF_INIT_FALSE,
	TX_RESIZE_FAIL
}tx_error_status;

tx_error_status is_buffer_full_tx(void);
tx_error_status is_buffer_empty_tx(void);
tx_error_status is_buffer_init_tx(void);
tx_error_status verify_buffer_ptr_tx(void);
void  buffer_init_tx(void);
void buffer_destroy_tx(void);
tx_error_status write_to_buffer_tx(unsigned char  val);
unsigned char read_from_buffer_tx();
tx_error_status write_to_buffer_tx(unsigned char );
uint32_t write_wrap_test_tx(void);
uint32_t write_wrap_test_tx(void);
uint16_t current_buffer_length_tx(void);

#endif /* __UART_BUFFER_TX_H__ */
