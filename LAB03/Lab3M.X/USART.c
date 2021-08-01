/*
 * File:   USART.c
 * Author: jonam
 *
 * Created on 1 de agosto de 2021, 02:47 PM
 */


#include <xc.h>

void configUSART(void){
//configurar transmisor y receptor asincrono
    TXSTAbits.SYNC = 0; //asincrono
    TXSTAbits.BRGH = 1; //high speed
    BAUDCTLbits.BRG16 = 1; //8bits baud rate generator is used
    
    SPBRG = 103;         //para el baud rate de 9600
    SPBRGH = 0;
   
    //serial port enabled (Configures RX/DT and TX/CK pins as serial)
    RCSTAbits.SPEN = 1; 
    RCSTAbits.RX9 = 0; //8 bits
    RCSTAbits.CREN = 1; //habilitar recepcion
    
    TXSTAbits.TX9 = 0; // 8bits
    TXSTAbits.TXEN = 1; //habilitar transmision
      
    //HABILITAR INTERRUPCIONES
    INTCONbits.GIE = 1; //global 
    INTCONbits.PEIE = 1;
    //PIE1bits.TXIE = 1; //interrupciones transmision
    //PIE1bits.RCIE = 1; //interrupciones receptor
    PIR1bits.TXIF = 0;  //limpiar interrupciones
    PIR1bits.RCIF = 0;
    
    return;
}