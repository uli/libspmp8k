ENTRY(start)

SECTIONS
{
	. = 0xA00000;
	.text :	{ *(.text) }
	.data : { *(.data) }
	_bss_start = .;
	.bss  : { *(.bss)  }
	_bss_end = .;
	end = .; _end = .; __end = .;
}
