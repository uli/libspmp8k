ENTRY(start)
STARTUP(start.o)

GROUP (-lgame -lc)

SECTIONS
{
	. = 0xA00000;
	.text :	{ *(.text) }
	.ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) }
	PROVIDE_HIDDEN (__exidx_start = .);
	.ARM.exidx   : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) }
	PROVIDE_HIDDEN (__exidx_end = .);
	. = ALIGN(4);
	.preinit_array     :
	{
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array*))
		PROVIDE_HIDDEN (__preinit_array_end = .);
	}
	.init_array     :
	{
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(.init_array*))
		PROVIDE_HIDDEN (__init_array_end = .);
	}
	.fini_array     :
	{
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(.fini_array*))
		PROVIDE_HIDDEN (__fini_array_end = .);
	}
	. = ALIGN(4);
	.data : { *(.data) }
	. = ALIGN(4);
	_bss_start = .;
	.bss  : { *(.bss)  }
	_bss_end = .;
	end = .; _end = .; __end = .;
}
