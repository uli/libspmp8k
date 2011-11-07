
#ifndef __UART_H__
#define __UART_H__

#include "sptypes.h"

#define	IO_BASE		0x90000000
#define	UART_BASE	0x92B04000

#define UART_DATA		(*(volatile uint32_t*)(UART_BASE + 0x00))
#define UART_STATUS1	(*(volatile uint32_t*)(UART_BASE + 0x14))
#define UART_STATUS2	(*(volatile uint32_t*)(UART_BASE + 0x18))
#define UART_STATUS3	(*(volatile uint32_t*)(UART_BASE + 0x20))

#endif // __UART_H__

