
extern void uart_putc(char c);
extern void uart_puts(char *str);

int main()
{
	uart_puts("Hello!\n");

	return 0;
}

