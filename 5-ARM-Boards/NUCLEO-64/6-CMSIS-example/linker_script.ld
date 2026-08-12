ENTRY(Reset_Handler)

MEMORY
{
    FLASH  (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
}

SECTIONS
{
	.text : { *(.isr_vector);
			 *(.text*);
			} > FLASH

	.rodata  : { 
			*(.rodata*);
			*(.srodata*); 
			. = ALIGN(4);
    		_etext = .;
			_sidata = .;
			}	> FLASH
			
	
	.data  : AT ( _sidata ) {  
				_sdata = .;
				*(.data*);
				*(.sdata*);
				_edata = .;
			} > RAM 

	.bss (NOLOAD) : { 
			_sbss = .;
			__bss_start__ = _sbss;
			*(.bss*) 
			_ebss = .;
			__bss_end__ = _ebss;
			}> RAM

	.stack (NOLOAD) :  {
		. = ORIGIN(RAM) + LENGTH(RAM);
		stack_top = .;
		_estack = stack_top;
	} > RAM
}
