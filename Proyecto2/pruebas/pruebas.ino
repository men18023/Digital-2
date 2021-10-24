//***************************************************************************************************************************************
/* Código de ejemplo de videojuego:
 * Detección de colisiones con objetos rectangulares.
 * Emplea estructuras para definir clases e instanciar objetos.
 * Incorpora actualización de cuadros cada 42ms ( aprox. 24fps).
 * Movimiento de sprite utilizando botones integrados.
 * Basado en librería para el uso de la pantalla ILI9341 en modo 8 bits
 * Autor: Diego Morales
 * IE3027: Electrónica Digital 2 - 2021
 */
//***************************************************************************************************************************************
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
#include "font.h"
#include "lcd_registers.h"
#include <SD.h>
#include <SPI.h>

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};  
int x = PE_2;    // select the input pin for the potentiometer
//int ledPinx = PF_2;      // select the pin for the LED
int sensorValuex = 0;  // variable to store the value coming from the sensor
int y = PE_3;
//int ledPiny = PF_1;
int sensorValuey = 0;
int right = 0;
int left = 0;
int up = 0;
int down = 0;
int pushU = PC_6;
int pushD = PC_7;
int pushL = PA_7;
int pushR = PD_7;
int sensorp = 0;
int varp = 0;
int inicial = 1;
//***************************************************************************************************************************************
// Functions Prototypes
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
bool Collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void Sprite_Clean();
void juego();
void food();
void game_over();
void inicio();
void Map();
char GameOver = 3;

extern uint8_t marioBitmap[]; // cargar bitmap desde memoria flash
extern uint8_t tile2[];
extern uint8_t tile4[];
extern uint8_t backgrounf[];
int flag = 0;
int pts1 = 0;
int pts2 = 0;
int f_up1 = 0;
int f_down1 = 0;
int f_right1 = 0;
int f_left1 = 0;
int f_up2 = 0;
int f_down2 = 0;
int f_right2 = 0;
int f_left2 = 0;
int f_map = 2;
//int pts1 = 0;
//int pts1 = 0;
Sd2Card card;
SdVolume volume;
SdFile root;
int lecturaserial;
const int chipSelect = PA_3; //cs PIN

File pic;
int ascii_hex(int a);
void MapSD(char x[]);

struct Sprite { // estructura para sprites
  int x; // posicion x
  int y; // posicion y
  int width; // ancho de bitmap
  int height; // altura de bitmap
  int columns; // columna sprite sheet
  int index; // indice sprite sheet
  int flip; // voltear imagen
  int offset; // desfase
} head, bunny;

struct Rectangle{ // estructura para rectangulos
  int x; // posicion x
  int y; // posicion y
  int width; // ancho de rectángulo
  int height; // altura de rectángulo
  int color; // color de relleno  
} T, B, L, R, rect;


struct Line{ // estructura para rectangulos
  int x; // posicion x
  int y; // posicion y
  int l; // largo de la linea
  int c; // color de relleno  
} TT;

bool rectUp = false; // dirección rectángulo
bool collision_food = false; // detección de colisión
bool collision_T = false;
bool collision_B = false; // detección de colisión
bool collision_R = false;
bool collision_L = false; // detección de colisión
extern uint8_t uvg[];
unsigned long previousMillis = 0;  
const long interval = 42;
//***************************************************************************************************************************************
// Initialization
//***************************************************************************************************************************************
void setup() {
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Start");
  LCD_Init();
  LCD_Clear(0x00);
  
  pinMode(PUSH1, INPUT_PULLUP); // botones integrados con como entrada pull-up
  pinMode(PUSH2, INPUT_PULLUP);
  pinMode(pushU, INPUT);
  pinMode(pushD, INPUT);
  pinMode(pushL, INPUT);
  pinMode(pushR, INPUT);
    // declare the ledPin as an OUTPUT:
  //pinMode(led, OUTPUT);  
  //pinMode(left, OUTPUT);
  //pinMode(up, OUTPUT);  
  T.x = 9;
  T.y = 49;
  T.width = 301;
  T.height = 1;
  T.color = 0xFFFF;

  B.x = 9;
  B.y = 230;
  B.width = 301;
  B.height = 1;
  B.color = 0xFFFF;

  R.x = 310;
  R.y = 49;
  R.width = 1;
  R.height = 181;
  R.color = 0xFFFF;

  L.x = 9;
  L.y = 49;
  L.width = 1;
  L.height = 181;
  L.color = 0xFFFF;

  head.x = 16;
  head.y = 60;
  head.width = 16;
  head.height = 16;
  head.columns = 4;
  head.index = 2;
  head.flip = 0;
  head.offset = 0;


  bunny.width = 16;
  bunny.height = 16;
  bunny.columns = 4;
  bunny.index = 2;
  bunny.flip = 1;
  bunny.offset = 0;
  flag = 1;


  delay(1000);
}
//***************************************************************************************************************************************
// Loop
//***************************************************************************************************************************************
void loop() {

  if (GameOver == 3){
    LCD_Bitmap(0, 0, 320, 240, uvg);
    delay(2500);
    f_map = 2;
    GameOver = 2;
  }
  else if (GameOver == 2){
      if (f_map == 2){
        Map();
        f_map = 1;
      }
      juego_1();
      }
  else if (GameOver == 1){
      if (f_map == 1){
        Map();
        f_map = 0;
      }
      juego_2();
      }
}

void juego_1(){

  if (flag == 1){
    bunny.x = random(10,290);
    bunny.y = random(50,216);
    flag = 0;
  }
  
  unsigned long currentMillis = millis();
  // actualización de frame cada 42ms = 24fps
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    //bool up = !digitalRead(pushU);
    //bool left = !digitalRead(pushL); 
    int down = digitalRead(pushD); // lectura de entradas
    int right = digitalRead(pushR);
    int up = digitalRead(pushU);
    int left = digitalRead(pushL);
      delay(10);
    if (right) { // modificación de atributos de sprite
       f_right1 = 1;
       f_left1 = 0;
       f_up1 = 0;
       f_down1 = 0;
    }
    if (down) {
       f_right1 = 0;
       f_left1 = 0;
       f_up1 = 0;
       f_down1 = 1;
    } 
    if (up) { // modificación de atributos de sprite
       f_right1 = 0;
       f_left1 = 0;
       f_up1 = 1;
       f_down1 = 0;
    }
    if (left) {
       f_right1 = 0;
       f_left1 = 1;
       f_up1 = 0;
       f_down1 = 0;
    } 
    if (f_right1 == 1){
      head.x += 4;
      //head.index++;
      head.index = 1;
      head.flip = 0;
    }
    if (f_down1 == 1){
      head.y += 4;
      //head.index++;
      head.index = 2;
      head.flip = 1;
    }
    if (f_up1 == 1){
      head.y -= 4;
      //head.index++;
      head.index = 0;
      head.flip = 1;
    }
    if (f_left1 == 1){
      head.x -= 4;
      //head.index++;
      head.index = 1;
      head.flip = 1;
    }

    collision_food = Collision(head.x, head.y, head.width, head.height,
                          bunny.x, bunny.y, bunny.width, bunny.height); // detección de colisión
    if (collision_food) { // se reemplaza el color al colisionar
      Sprite_Clean();
      pts1++;
      flag = 1;
      //LCD_Print(pts1, 5, 0, 2, 0xFFFF, 0x2305);
    }
    else {
    }
    collision_T = Collision(head.x, head.y, head.width, head.height,
                          T.x, T.y, T.width, T.height);
    if (collision_T) { // se reemplaza el color al colisionar
      GameOver = 1;
      setup();
      //game_over();
    }
   else {
      //pts1 = pts1;
    }

    collision_R = Collision(head.x, head.y, head.width, head.height,
                          R.x, R.y, R.width, R.height);
    if (collision_R) { // se reemplaza el color al colisionar
      GameOver = 1;
      setup();
      //game_over();
    }
   else {
      //pts1 = pts1;
    }

        collision_L = Collision(head.x, head.y, head.width, head.height,
                          L.x, L.y, L.width, L.height);
    if (collision_L) { // se reemplaza el color al colisionar
      GameOver = 1;
      setup();
      //game_over();
    }
   else {
      //pts1 = pts1;
    }

    collision_B = Collision(head.x, head.y, head.width, head.height,
                          B.x, B.y, B.width, B.height);
    if (collision_B) { // se reemplaza el color al colisionar
      GameOver = 1;
      setup();
      //game_over();
    }
   else {
      //pts1 = pts1;
    }

    
    //FillRect(rect.x, rect.y, rect.width, rect.height, rect.color);
    if (head.index == 1 && head.flip == 0){ // dependiendo de la dirección, se colorea resto del sprite del frame anterior
      FillRect(head.x - 4, head.y, 4, head.height, 0x0000); // derecha
    }
    else if (head.index == 2 && head.flip == 1){
      FillRect(head.x, head.y - 4, head.width, 4, 0x0000); // abajo
    }
    else if (head.index == 0 && head.flip == 1){
      FillRect(head.x, head.y + 16, head.width, 4, 0x0000);// arriba
    }
    else if (head.index == 1 && head.flip == 1){
      FillRect(head.x + 16, head.y, 4, head.height, 0x0000); //izquierda
    }
    LCD_Sprite(head.x, head.y, head.width, head.height, tile2, head.columns, head.index, head.flip, head.offset);
    //food();
    //if (bunny.x != head.x && bunny.y != head.y){
    LCD_Sprite(bunny.x, bunny.y, bunny.width, bunny.height, tile4, bunny.columns, bunny.index, bunny.flip, bunny.offset);
    String p1 = String(pts1);
    String p2 = String(pts2);
    
    LCD_Print(p2, 150, 20, 2, 0xFFFF, 0x2305);
    LCD_Print(p1, 150, 0, 2, 0xFFFF, 0x2305);
    
  }
  }
void juego_2(){
  
  if (flag == 1){
    bunny.x = random(10,290);
    bunny.y = random(50,216);
    flag = 0;
  }
  
  unsigned long currentMillis = millis();
  // actualización de frame cada 42ms = 24fps
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    int down = digitalRead(pushD); // lectura de entradas
    int right = digitalRead(pushR);
    int up = digitalRead(pushU);
    int left = digitalRead(pushL);
      delay(10);
    if (right) { // modificación de atributos de sprite
       f_right2 = 1;
       f_left2 = 0;
       f_up2 = 0;
       f_down2 = 0;
    }
    if (down) {
       f_right2 = 0;
       f_left2 = 0;
       f_up2 = 0;
       f_down2 = 1;
    } 
    if (up) { // modificación de atributos de sprite
       f_right2 = 0;
       f_left2 = 0;
       f_up2 = 1;
       f_down2 = 0;
    }
    if (left) {
       f_right2 = 0;
       f_left2 = 1;
       f_up2 = 0;
       f_down2 = 0;
    } 
    if (f_right2 == 1){
      head.x += 4;
      //head.index++;
      head.index = 1;
      head.flip = 0;
    }
    if (f_down2 == 1){
      head.y += 4;
      //head.index++;
      head.index = 2;
      head.flip = 1;
    }
    if (f_up2 == 1){
      head.y -= 4;
      //head.index++;
      head.index = 0;
      head.flip = 1;
    }
    if (f_left2 == 1){
      head.x -= 4;
      //head.index++;
      head.index = 1;
      head.flip = 1;
    }

    collision_food = Collision(head.x, head.y, head.width, head.height,
                          bunny.x, bunny.y, bunny.width, bunny.height); // detección de colisión
    if (collision_food) { // se reemplaza el color al colisionar
      pts2++;
      flag = 1;
      //LCD_Print(pts1, 5, 0, 2, 0xFFFF, 0x2305);
    }
    else {
    }
    collision_T = Collision(head.x, head.y, head.width, head.height,
                          T.x, T.y, T.width, T.height);
    if (collision_T) { // se reemplaza el color al colisionar
      GameOver = 0;
      game_over();
    }
   else {
      //pts1 = pts1;
    }

    collision_R = Collision(head.x, head.y, head.width, head.height,
                          R.x, R.y, R.width, R.height);
    if (collision_R) { // se reemplaza el color al colisionar
      GameOver = 0;
      game_over();
    }
   else {
      //pts1 = pts1;
    }

        collision_L = Collision(head.x, head.y, head.width, head.height,
                          L.x, L.y, L.width, L.height);
    if (collision_L) { // se reemplaza el color al colisionar
      GameOver = 0;
      game_over();
    }
   else {
      //pts1 = pts1;
    }

    collision_B = Collision(head.x, head.y, head.width, head.height,
                          B.x, B.y, B.width, B.height);
    if (collision_B) { // se reemplaza el color al colisionar
      GameOver = 0;
      game_over();
    }
   else {
      //pts1 = pts1;
    }

    
    //FillRect(rect.x, rect.y, rect.width, rect.height, rect.color);
    //FillRect(rect.x, rect.y, rect.width, rect.height, rect.color);
    if (head.index == 1 && head.flip == 0){ // dependiendo de la dirección, se colorea resto del sprite del frame anterior
      FillRect(head.x - 4, head.y, 4, head.height, 0x0000); // derecha
    }
    else if (head.index == 2 && head.flip == 1){
      FillRect(head.x, head.y - 4, head.width, 4, 0x0000); // abajo
    }
    else if (head.index == 0 && head.flip == 1){
      FillRect(head.x, head.y + 16, head.width, 4, 0x0000);// arriba
    }
    else if (head.index == 1 && head.flip == 1){
      FillRect(head.x + 16, head.y, 4, head.height, 0x0000); //izquierda
    }
    LCD_Sprite(head.x, head.y, head.width, head.height, tile2, head.columns, head.index, head.flip, head.offset);
    //food();
    //if (bunny.x != head.x && bunny.y != head.y){
    LCD_Sprite(bunny.x, bunny.y, bunny.width, bunny.height, tile4, bunny.columns, bunny.index, bunny.flip, bunny.offset);
    String p2 = String(pts2);
    String p1 = String(pts1);
    LCD_Print(p1, 150, 0, 2, 0xFFFF, 0x2305);
    LCD_Print(p2, 150, 20, 2, 0xFFFF, 0x2305);
    
  }
  }

void game_over(){
  //LCD_Clear(0x0000);
  head.x = 130;
  head.y = 60;
  bunny.x = 150;
  bunny.y = 60;
  LCD_Bitmap(0, 0, 320, 240, uvg);
  //FillRect(0, 0, 320, 240, 0x0000);
  LCD_Sprite(head.x, head.y, head.width, head.height, tile2, head.columns, head.index, head.flip, head.offset);
  LCD_Sprite(bunny.x, bunny.y, bunny.width, bunny.height, tile4, bunny.columns, bunny.index, bunny.flip, bunny.offset);
  String gameOver = "GAME OVER";
  LCD_Print(gameOver, 90, 90, 2, 0xFFFF, 0x0000);
  if (pts2 > pts1){
    String winner1 = "PLAYER 2 WINS";
    LCD_Print(winner1, 50, 130, 2, 0xFFFF, 0x0000);
  }
  else if (pts2 < pts1){
    String winner2 = "PLAYER 1 WINS";
    LCD_Print(winner2, 50, 130, 2, 0xFFFF, 0x0000);
  }
  else{
    String tie = "ITS A TIE";
    LCD_Print(tie, 80, 120, 2, 0xFFFF, 0x0000);
  }
   
}

void Sprite_Clean(){
   Rect(bunny.x, bunny.y, bunny.width, bunny.height, 0xFFFF);
}

void Map(){
      
  FillRect(0,0,320,240, 0x2305);
  //LCD_Bitmap(0, 0, 320, 240, uvg);
  int snake_index = snake_index++;
  LCD_Sprite(250, 10, 32, 32, marioBitmap, 7, snake_index, 0, 0);
  String text1 = "Pts P1:";
  String text2 = "Pts P2:";
  LCD_Print(text1, 5, 0, 2, 0xFFFF, 0x2305);
  LCD_Print(text2, 5, 30, 2, 0xFFFF, 0x2305);
  FillRect(10,50,300,180, 0x0000);
  Rect(T.x, T.y, T.width , T.height, T.color);
  Rect(B.x, B.y, B.width , B.height, B.color);
  Rect(R.x, R.y, R.width , R.height, R.color);
  Rect(L.x, L.y, L.width , L.height, L.color);
  }
//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+w;
  y2 = y+h;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = w*h*2-1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c); 
      k = k - 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
  Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
    for (int j = 0; j < height; j++){
        k = (j*(ancho) + index*width -1 - offset)*2;
        k = k+width*2;
       for (int i = 0; i < width; i++){
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k+1]);
        k = k - 2;
       } 
    }
  }
  else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
    }
  }
  digitalWrite(LCD_CS, HIGH);
}
bool Collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
  return (x1 < x2 + w2) && (x1+ w1 > x2) && (y1 < y2 + h2) && (y1 + h1 > y2);
}

  int ascii_hex(int a) {
  switch (a) {
    case 48:
      return 0;
    case 49:
      return 1;
    case 50:
      return 2;
    case 51:
      return 3;
    case 52:
      return 4;
    case 53:
      return 5;
    case 54:
      return 6;
    case 55:
      return 7;
    case 56:
      return 8;
    case 57:
      return 9;
    case 97:
      return 10;
    case 98:
      return 11;
    case 99:
      return 12;
    case 100:
      return 13;
    case 101:
      return 14;
    case 102:
      return 15;
  }
}

  void MapSD(char x[]){
  pic = SD.open(x, FILE_READ);
  int hex1 = 0;
  int val1 = 0;
  int val2 = 0;
  int mapear = 0;
  int vert = 0;
  unsigned char maps[640];
  if(pic){
    Serial.println("Reading file...");
    while (pic.available()){
      mapear = 0;
      while (mapear < 640){
        hex1 = pic.read();
        if (hex1 == 120){
          val1 = pic.read();
          val2 = pic.read();
          val1 = ascii_hex(val1);
          val2 = ascii_hex(val2);
          maps[mapear]=val1*16+val2;
          mapear++;
        }
      }
      LCD_Bitmap(0, vert, 320, 1, maps);
      vert++;
    }
    pic.close();
  }
  else{
    Serial.println("File couldn't be read...");
    pic.close();
  }
}
