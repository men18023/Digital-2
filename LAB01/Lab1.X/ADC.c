#include <xc.h> 
#include <stdint.h> 
#include <pic16f887.h>
#include "ADC.h"

//lectura del valor en ADC
uint8_t valor_ADC()
{
    return ADRESH;
}
void conADC(void)
{
    ADCON0bits.CHS = 0; // CANAL AN0
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADCS0 = 1; 
    ADCON0bits.ADON = 1; 
    
    ADCON1bits.ADFM = 0; // justificacion
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;  //Vss y Vcc
}