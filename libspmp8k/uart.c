/*
 * uart.c - this file is part of libspmp8k
 * 
 * Copyright (C) 2010, alemaxx
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "uart.h"

#define uart_reg(n)	(*(volatile uint32_t*)(UART_BASE + n))

void uart_putc(char c)
{
	int timeout=4000;

	if (UART_STATUS1 & 0x1E) return;
	while (timeout > 0) {
		if ((UART_STATUS2 & 0x10) && ((UART_STATUS3 & 4) == 0)) {
			UART_DATA = c;
			break;
		}
		timeout--;
	}
//	if (timeout < 1) diag_printf("timeout\n");
}

int uart_getc(char *c)
{
	int timeout=4000;

	while (timeout > 0) {
		if ((UART_STATUS3 & 0x10) == 0) break;
		timeout--;
	}
	if (timeout < 1) return 0;

	if (UART_STATUS1 & 0x1E) return 0;

	*c = UART_DATA;
	return 1;
}

