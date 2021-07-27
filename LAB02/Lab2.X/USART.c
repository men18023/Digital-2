#include <xc.h>
#include <pic16f887.h>
#include "USART.h"

void _baudios(void){
    SPBRG = 12; //9600 baudios para 8MHZ
}
//Configuracion dada en el datasheet
void config_tx(void){
    TXSTAbits.CSRC = 0;  //Clock terminal
    TXSTAbits.TX9 = 0;  //8 bits 
    TXSTAbits.TXEN = 1;  //Transmicion habilitada
    TXSTAbits.SYNC = 0;  //Asincrono
    TXSTAbits.BRGH = 0; 
    TXSTAbits.TRMT = 0; //Tsr full
    TXSTAbits.TX9D = 0;
}

void config_rc(void){
    RCSTAbits.SPEN = 1; //puerto serial 
    RCSTAbits.RX9 = 0;
    RCSTAbits.SREN = 0;
    RCSTAbits.CREN = 1; //Recibir  
    RCREG = 0;  
}
void Write_USART(uint8_t a){
    while(!TRMT);
    TXREG=a;
}
void Write_USART_String(char *a){
    uint8_t i;
    for(i=0;a[i]!='\0';i++){
        Write_USART(a[i]);
    }
}
uint8_t Read_USART(){
  while(!RCIF);
  return RCREG;
}