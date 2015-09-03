#ifndef __UART_H
#define __UART_H


void uart_init(void);
void uart_send_data(char *data);
void uart_send_byte(char byte);

#endif
