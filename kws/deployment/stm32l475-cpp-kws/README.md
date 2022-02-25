*Troubleshoot*

Building Errors

- In Drivers/CMSIS/Include/cmsis_gcc.h

  Move the following code out of the __cmsis_start function:
  
extern void _start(void) __NO_RETURN;

typedef struct {
uint32_t const* src;
uint32_t* dest;
uint32_t  wlen;
} __copy_table_t;

typedef struct {
uint32_t* dest;
uint32_t  wlen;
} __zero_table_t;

extern const __copy_table_t __copy_table_start__;
extern const __copy_table_t __copy_table_end__;
extern const __zero_table_t __zero_table_start__;
extern const __zero_table_t __zero_table_end__;
