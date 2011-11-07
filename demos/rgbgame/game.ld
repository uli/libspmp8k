ENTRY(start)

SECTIONS
{
	. = 0xA00000;
	.text :	{ *(.text) }
	.data : { *(.data) }
	.bss  : { *(.bss)  }
}
