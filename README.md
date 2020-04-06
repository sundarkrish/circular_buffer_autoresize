# circular_buffer_autoresize
This is an implementation of a circular buffer with all bytes usable(Usual method is to leave a byte empty to differentiate buffer full and buffer empty conditions).During write to buffer operation, the buffer resizes to double its previous size whenenver a buffer full condition is reached and then performs a byte write.(Resizing is limited to contraints like heap size that is mentioned in the linker script).


IDE : MCUXPRESSO
BOARD USED : KL25Z FREEDOM BOARD

### Reference : 
- https://leap-embedded-system.com/?p=79
