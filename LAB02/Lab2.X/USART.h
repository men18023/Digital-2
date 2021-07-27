/* 
 * File:   USART.h
 * Author: jonam
 *
 * Created on 24 de julio de 2021, , 7:43 PM
 */

#ifndef USART_H
#define	USART_H

#include <xc.h> 
#include <stdint.h>

void _baudios(void);
void config_tx(void);
void config_rc(void);
void Write_USART(uint8_t a);
void Write_USART_String(char *a);
uint8_t Read_USART(void);

#endif	/* USART_H */

