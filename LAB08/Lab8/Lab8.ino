/*  draw text's APP
    drawChar(INT8U ascii,INT16U poX, INT16U poY,INT16U size, INT16U fgcolor);
    drawString(char *string,INT16U poX, INT16U poY,INT16U size,INT16U fgcolor);
*/
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"

#include <TFTv2.h>
#include <SPI.h>
void setup()
{	
    Serial.begin(115200);			   // Don't remove

    Tft.begin(PE_1,PE_2,PE_3,PE_4);                // CS,DC,BL,RESET pin
    Tft.TFTinit();                                 // init TFT library
    Tft.backlight_on();                            // turn on the background light
    Tft.fillRectangle(0, 0, 240 , 320, BLUE);
   // Tft.drawChar('S',0,0,1,RED,BLACK);                // draw char: 'S', (0, 0), size: 1, color: RED
    
    //Tft.drawChar('E',10,10,2,BLUE,BLACK);             // draw char: 'E', (10, 10), size: 2, color: BLUE
    
    //Tft.drawChar('E',20,40,3,GREEN,BLACK);            // draw char: 'E', (20, 40), size: 3, color: GREEN
    
    //Tft.drawChar('E',30,80,4,YELLOW,BLACK);           // draw char: 'E', (30, 80), size: 4, color: YELLOW
    
    //Tft.drawChar('D',40,120,4,YELLOW,BLACK);          // draw char: 'D', (40, 120), size: 4, color: YELLOW
    
    //Tft.drawString("Hello",0,180,3,CYAN,BLACK);       // draw string: "hello", (0, 180), size: 3, color: CYAN
    
    //Tft.drawString("World!!",60,220,4,WHITE,BLACK);    // draw string: "world!!", (80, 230), size: 4, color: WHITE
    

}

void loop()
{
  
}
