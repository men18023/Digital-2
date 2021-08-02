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
    BAUDCTLbits.BRG16 = 0;
    
    SPBRG = 25;         //para el baud rate de 9600
    SPBRGH = 1;
   
    //
    RCSTAbits.SPEN = 1; 
    RCSTAbits.RX9 = 0; //8 bits
    RCSTAbits.CREN = 1; //recepcion
    
    TXSTAbits.TX9 = 0; // 8bits
    TXSTAbits.TXEN = 1; // transmision
      
    //INTERRUPCIONES
    INTCONbits.GIE = 1; //global 
    INTCONbits.PEIE = 1;
    PIR1bits.TXIF = 0;  //limpiar interrupciones
    PIR1bits.RCIF = 0;
    
    return;
}