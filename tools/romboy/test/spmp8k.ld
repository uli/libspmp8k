ENTRY(start)

SECTIONS
{
	diag_printf		= 0x50178;
	rb_putc			= 0x56CB0;

	. = 0x200000;
	.text :	{ *(.text) }
	.data : { *(.data) }
	.bss  : { *(.bss)  }
}
