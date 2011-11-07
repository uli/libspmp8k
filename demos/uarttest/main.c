
#include "uart.h"

extern int printf(const char *format, ...);

int main()
{
	char c;
	int len, i;

	printf("uart t%xst \r\n", 0xe);
	printf("type something or hit return to quit:\n\r");
	while (1) {
		if (uart_getc(&c)) {
			uart_putc(c);
			if (c == '\r') break;
		}
	}

	return 0;
}

