#include <uart_buffer_rx.h>

static int32_t head_ptr_rx = -1; // invalid pointer index condition
static int32_t tail_ptr_rx = -1;


uint16_t BUFF_MAX_RX = 10; // Initial buffer length
uint16_t temp_BUFF_MAX_RX = 0; // To hold previous buffer maximum length
uint16_t temp_tail_ptr_rx = -1; // Holds previous tail ptr location

static buffer_rx_typedef  * buffer_rx = NULL; // invalid pointer position
static buffer_rx_typedef  * temp_buffer_rx = NULL; // stores previous buffer pointer.Used in case realloc fails


/*•	Function to check is buffer full
•	Function to check is buffer empty
•	Function to verify buffer is initialized
•	Function to verify the buffer pointer is valid
•	Function to initialize a buffer
•	Function to destroy a buffer*/

rx_error_status is_buffer_full_rx(void)
{

	if (is_buffer_init_rx() == 6)
	{
		if ((head_ptr_rx == tail_ptr_rx) && (buffer_rx[head_ptr_rx].not_empty == 1))
		{

			return RX_BUFFER_FULL;
		}
		else
		{

			return RX_BUFFER_NON_EMPTY; // used for both non-empty and not full conditions
		}

	}
	else
	{

		return RX_BUFF_INIT_FALSE;

	}

}


rx_error_status is_buffer_empty_rx(void)
{
	if (is_buffer_init_rx() == 6)
	{
		if ((head_ptr_rx == tail_ptr_rx) && (buffer_rx[head_ptr_rx].not_empty == 0))
		{

			return RX_BUFFER_EMPTY;
		}
		else
		{

			return RX_BUFFER_NON_EMPTY; // used for both non-empty and not full conditions
		}

	}
	else
	{

		return RX_BUFF_INIT_FALSE;

	}


}


rx_error_status is_buffer_init_rx(void)
{

	// Returns RX_BUFF_INIT_TRUE if head and the tail pointers  are not at -1 and buffer not poiting to null;
	if (!((head_ptr_rx == -1) || (tail_ptr_rx == -1) || (buffer_rx != NULL)))
	{
		return RX_BUFF_INIT_TRUE;
	}

	return RX_BUFF_INIT_FALSE;

}



rx_error_status verify_buffer_ptr_rx(void)
{
	// Checks if buffer head and tail pointers are in valid position
	if ((head_ptr_rx == -1) || (tail_ptr_rx == -1))
	{
		return RX_BUFF_PTR_INVALID;
	}

	return RX_BUFF_PTR_VALID;

}


void  buffer_init_rx(void)
{

	buffer_rx = (buffer_rx_typedef *)malloc(sizeof(buffer_rx_typedef) * BUFF_MAX_RX);
	// write algorithm required non-empty flag to be zero at the beginning
	memset(buffer_rx, 0, sizeof(buffer_rx_typedef) * BUFF_MAX_RX);
	// Buffer empty when init condition
    head_ptr_rx = 0;
    tail_ptr_rx = 0;

}


void buffer_destroy_rx(void)
{

	free(buffer_rx);
	// Buffer pointers are assigned invalid pointer condition when destroyed
    head_ptr_rx = -1;
    tail_ptr_rx = -1;

}

rx_error_status write_to_buffer_rx(unsigned char  val)
{

	if (is_buffer_init_rx() == 7)
	{
		return RX_BUFF_INIT_FALSE;
	}

    if (buffer_rx[head_ptr_rx].not_empty == 0) // write byte when empty_flag == 1
    {
        buffer_rx[head_ptr_rx].data = val;
        buffer_rx[head_ptr_rx].not_empty = 1; // The location is made non-empty after write
        head_ptr_rx++;
        head_ptr_rx %= BUFF_MAX_RX;

        return RX_NO_ERROR;
    }
    else
    {
    	// Option 1 : Send an error message
    	// Option 2 : Resize the buffer and write byte
    	int16_t i;
    	uint16_t length_to_copy;

    	length_to_copy = BUFF_MAX_RX - tail_ptr_rx;
    	temp_BUFF_MAX_RX = BUFF_MAX_RX;
    	BUFF_MAX_RX *= 2; // size doubles during resize
    	temp_buffer_rx = buffer_rx;
    	buffer_rx = (buffer_rx_typedef *)realloc(buffer_rx, BUFF_MAX_RX * sizeof(buffer_rx_typedef));


    	if (buffer_rx != NULL)
    	{

        	for (i = temp_BUFF_MAX_RX; i < (BUFF_MAX_RX - length_to_copy); i++)
        	{
        		buffer_rx[i].not_empty = 0;
        	}

        	temp_tail_ptr_rx = tail_ptr_rx;
        	tail_ptr_rx = BUFF_MAX_RX - length_to_copy;

        	for(i = 0; i < length_to_copy; i++)
        	{

        		buffer_rx[tail_ptr_rx + i] = buffer_rx[temp_tail_ptr_rx + i];
        		buffer_rx[temp_tail_ptr_rx + i].not_empty = 0;
        	}


            buffer_rx[head_ptr_rx].data = val;
            buffer_rx[head_ptr_rx].not_empty = 1; // The location is made non-empty after write
            head_ptr_rx++;
            head_ptr_rx %= BUFF_MAX_RX;

            return RX_NO_ERROR;

    	}
    	else
    	{
    		buffer_rx = temp_buffer_rx; // Revert if realloc fails
    		BUFF_MAX_RX /= 2;
    		return RX_RESIZE_FAIL;
    	}

    }

}

unsigned char read_from_buffer_rx()
{


    unsigned char c = 0;

    if (is_buffer_init_rx() == 7)
    {
    	return (unsigned char)RX_BUFF_INIT_FALSE;
    }

    if (buffer_rx[tail_ptr_rx].not_empty == 1)
    {
        c = buffer_rx[tail_ptr_rx].data;
        buffer_rx[tail_ptr_rx].not_empty = 0; // The location is made empty after read
        tail_ptr_rx++;
        tail_ptr_rx %= BUFF_MAX_RX;
        return c;

    }
    else
    {

    	return (unsigned char)RX_BUFFER_EMPTY; // returns 0 when trying to read an empty buffer

    }



}
