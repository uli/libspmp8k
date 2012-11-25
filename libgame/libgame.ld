ENTRY(start)

SECTIONS
{
	. = 0xA00000;
	.text :	{ *(.text) }
	.ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) }
	PROVIDE_HIDDEN (__exidx_start = .);
	.ARM.exidx   : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) }
	PROVIDE_HIDDEN (__exidx_end = .);
	. = ALIGN(4);
	.data : { *(.data) }
	. = ALIGN(4);
	_bss_start = .;
	.bss  : { *(.bss)  }
	_bss_end = .;
	end = .; _end = .; __end = .;
}
