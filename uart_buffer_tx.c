#include <uart_buffer_tx.h>

static int32_t head_ptr_tx = -1; // invalid pointer index condition
static int32_t tail_ptr_tx = -1;


uint16_t BUFF_MAX_TX = 10; // Initial buffer length
uint16_t temp_BUFF_MAX_TX = 0; // To hold previous buffer maximum length
uint16_t temp_tail_ptr_tx = -1; // Holds previous tail ptr location

static buffer_tx_typedef  * buffer_tx = NULL; // invalid pointer position
static buffer_tx_typedef  * temp_buffer_tx = NULL; // stores previous buffer pointer.Used in case realloc fails


/*•	Function to check is buffer full
•	Function to check is buffer empty
•	Function to verify buffer is initialized
•	Function to verify the buffer pointer is valid
•	Function to initialize a buffer
•	Function to destroy a buffer*/

tx_error_status is_buffer_full_tx(void)
{

	if (is_buffer_init_tx() == 6)
	{
		if ((head_ptr_tx == tail_ptr_tx) && (buffer_tx[head_ptr_tx].not_empty == 1))
		{

			return TX_BUFFER_FULL;
		}
		else
		{

			return TX_BUFFER_NON_EMPTY; // used for both non-empty and not full conditions
		}

	}
	else
	{

		return TX_BUFF_INIT_FALSE;

	}

}


tx_error_status is_buffer_empty_tx(void)
{
	if (is_buffer_init_tx() == 6)
	{
		if ((head_ptr_tx == tail_ptr_tx) && (buffer_tx[head_ptr_tx].not_empty == 0))
		{

			return TX_BUFFER_EMPTY;
		}
		else
		{

			return TX_BUFFER_NON_EMPTY; // used for both non-empty and not full conditions
		}

	}
	else
	{

		return TX_BUFF_INIT_FALSE;

	}


}


tx_error_status is_buffer_init_tx(void)
{

	// Returns TX_BUFF_INIT_TRUE if head and the tail pointers  are not at -1 and buffer not poiting to null;
	if (!((head_ptr_tx == -1) || (tail_ptr_tx == -1) || (buffer_tx == NULL)))
	{
		return TX_BUFF_INIT_TRUE;
	}

	return TX_BUFF_INIT_FALSE;

}



tx_error_status verify_buffer_ptr_tx(void)
{
	// Checks if buffer head and tail pointers are in valid position
	if ((head_ptr_tx == -1) || (tail_ptr_tx == -1))
	{
		return TX_BUFF_PTR_INVALID;
	}

	return TX_BUFF_PTR_VALID;

}


void  buffer_init_tx(void)
{

	free(buffer_tx);
	buffer_tx = (buffer_tx_typedef *)malloc(sizeof(buffer_tx_typedef) * BUFF_MAX_TX);
	// write algorithm required non-empty flag to be zero at the beginning
	memset(buffer_tx, 0, sizeof(buffer_tx_typedef) * BUFF_MAX_TX);
	// Buffer empty when init condition
    head_ptr_tx = 0;
    tail_ptr_tx = 0;

}


void buffer_destroy_tx(void)
{

	free(buffer_tx);
	// Buffer pointers are assigned invalid pointer condition when destroyed
    head_ptr_tx = -1;
    tail_ptr_tx = -1;

}

tx_error_status write_to_buffer_tx(unsigned char  val)
{

	if (is_buffer_init_tx() == 7)
	{
		return TX_BUFF_INIT_FALSE;
	}

    if (buffer_tx[head_ptr_tx].not_empty == 0) // write byte when empty_flag == 1
    {
        buffer_tx[head_ptr_tx].data = val;
        buffer_tx[head_ptr_tx].not_empty = 1; // The location is made non-empty after write
        head_ptr_tx++;
        head_ptr_tx %= BUFF_MAX_TX;

        return TX_NO_ERROR;
    }
    else
    {
    	// Option 1 : Send an error message
    	// Option 2 : Resize the buffer and write byte
    	int16_t i;
    	uint16_t length_to_copy;

    	length_to_copy = BUFF_MAX_TX - tail_ptr_tx;
    	temp_BUFF_MAX_TX = BUFF_MAX_TX;
    	BUFF_MAX_TX *= 2; // size doubles during resize
    	temp_buffer_tx = buffer_tx;
    	buffer_tx = (buffer_tx_typedef *)realloc(buffer_tx, BUFF_MAX_TX * sizeof(buffer_tx_typedef));


    	if (buffer_tx != NULL)
    	{

        	for (i = temp_BUFF_MAX_TX; i < (BUFF_MAX_TX - length_to_copy); i++)
        	{
        		buffer_tx[i].not_empty = 0;
        	}

        	temp_tail_ptr_tx = tail_ptr_tx;
        	tail_ptr_tx = BUFF_MAX_TX - length_to_copy;

        	for(i = 0; i < length_to_copy; i++)
        	{

        		buffer_tx[tail_ptr_tx + i] = buffer_tx[temp_tail_ptr_tx + i];
        		buffer_tx[temp_tail_ptr_tx + i].not_empty = 0;
        	}


            buffer_tx[head_ptr_tx].data = val;
            buffer_tx[head_ptr_tx].not_empty = 1; // The location is made non-empty after write
            head_ptr_tx++;
            head_ptr_tx %= BUFF_MAX_TX;

            return TX_NO_ERROR;

    	}
    	else
    	{
    		buffer_tx = temp_buffer_tx; // Revert if realloc fails
    		BUFF_MAX_TX /= 2;
    		return TX_RESIZE_FAIL;
    	}

    }

}

unsigned char read_from_buffer_tx()
{


    unsigned char c = 0;

    if (is_buffer_init_tx() == 7)
    {
    	return (unsigned char)TX_BUFF_INIT_FALSE;
    }

    if (buffer_tx[tail_ptr_tx].not_empty == 1)
    {
        c = buffer_tx[tail_ptr_tx].data;
        buffer_tx[tail_ptr_tx].not_empty = 0; // The location is made empty after read
        tail_ptr_tx++;
        tail_ptr_tx %= BUFF_MAX_TX;
        return c;

    }
    else
    {

    	return (unsigned char)TX_BUFFER_EMPTY; // returns 0 when trying to read an empty buffer

    }



}


// Only for test purpose.Note: Exposing head and tail ptr makes code very vulnerable.
// Use only for test purpose at inevitable cases.
uint32_t remove_wrap_test_tx(void)
{

	return tail_ptr_tx;
}


uint32_t write_wrap_test_tx(void)
{

	return head_ptr_tx;
}

uint16_t current_buffer_length_tx(void)
{

	return BUFF_MAX_TX;
}
