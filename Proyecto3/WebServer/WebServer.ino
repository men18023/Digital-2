/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server 
  Basándose en los ejemplos de: 
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <WebServer.h>
//************************************************************************************************
// Variables globales
//************************************************************************************************
uint8_t cont;
uint8_t x;

// SSID & Password
const char* ssid = "Perraso";  // Enter your SSID here
const char* password = "12345678";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)


uint8_t LED1pin = 2;
bool LED1status = LOW;
//************************************************************************************************
//Variables
//************************************************************************************************
//TIVA PARQUEO 1
uint8_t bandera = 0;
uint8_t parqueo_1 = 0;
uint8_t parqueo_2 = 0;
uint8_t parqueo_3 = 0;
uint8_t parqueo_4 = 0;
uint8_t parq_disp = 0;
//TIVA PARQUEO 2
uint8_t bandera2 = 0;
uint8_t parqueo_5 = 0;
uint8_t parqueo_6 = 0;
uint8_t parqueo_7 = 0;
uint8_t parqueo_8 = 0;
//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  pinMode(LED1pin, OUTPUT);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);
  
  // para display 7 segmentos
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient();
  disp(x);
  if (Serial.available() > 0) {
    bandera =Serial.read();
    Serial.println(bandera);

    //cont = 5;
    //x = cont;
    //Funcionamiento para que se muestre en el sitio web 
    if (bandera & 0x01){
      parqueo_1 = 1;
      //cont--;
      //x =  cont;
      
    }
    else {
      parqueo_1 = 0;
      //cont++;
      //x =  cont;
    }
    if (bandera & 0x02){
      parqueo_2 = 1;
      //cont--;
      //x =  cont;
    }
    else {
      parqueo_2 = 0;
      //cont--;
      //x =  cont;
    }
    if (bandera & 0x04){
      parqueo_3 = 1;
      //cont--;
      //x =  cont;
    }
    else {
      parqueo_3 = 0;
      //cont--;
      //x =  cont;
    }
    if (bandera & 0x08){
      parqueo_4 = 1;
      //cont--;
      //x =  cont;
    }
    else {
      parqueo_4 = 0;
      //cont--;
      //x =  cont;
    }
    Serial.println(parqueo_1);
  }
  if (Serial2.available() > 0) {
    bandera2 =Serial2.read();
    Serial2.println(bandera);
    //Funcionamiento para que se muestre en el sitio web 
    if (bandera2 & 0x01){
      parqueo_5 = 1;
    }
    else {
      parqueo_5 = 0;
    }
    if (bandera2 & 0x02){
      parqueo_6 = 1;
    }
    else {
      parqueo_6 = 0;
    }
    if (bandera2 & 0x04){
      parqueo_7 = 1;
    }
    else {
      parqueo_7 = 0;
    }
    if (bandera2 & 0x08){
      parqueo_8 = 1;
    }
    else {
      parqueo_8 = 0;
    }
    Serial2.println(parqueo_5);
  }
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {
  LED1status = LOW;
  Serial.println("GPIO2 Status: OFF");
  parq_disp = -(parqueo_1 + parqueo_2 + parqueo_3 + parqueo_4 + parqueo_5 + parqueo_6 + parqueo_7 + parqueo_8) + 8 ;
  x = parq_disp;
  server.send(200, "text/html", SendHTML2(parqueo_1, parqueo_2, parqueo_3, parqueo_4, parqueo_5, parqueo_6, parqueo_7, parqueo_8, parq_disp));
}
//************************************************************************************************
// Handler de led1on
//************************************************************************************************
void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO2 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status));
}
//************************************************************************************************
// Handler de led1off
//************************************************************************************************
void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO2 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status));
}
//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
//Pagina web y funcionamiento del parqueo disponible y ocupado 
String SendHTML2(uint8_t parq_1, uint8_t parq_2, uint8_t parq_3, uint8_t parq_4,uint8_t parq_5, uint8_t parq_6, uint8_t parq_7, uint8_t parq_8, uint8_t parq_disp) {
  String pagina = "<html>\n";
pagina +="<!doctype html>\n";
pagina +="<html lang=\"en\">\n";
pagina +="  <head>\n";
pagina +="    <meta charset=\"utf-8\">\n";
pagina +="    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
pagina +="    <link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-+0n0xVW2eSR5OomGNYDnhzAbDsOXxcvSN1TPprVMTNDbiYZCxYbOOl7+AMvyTG2x\" crossorigin=\"anonymous\">\n";
pagina +="    <title>Parqueos</title>\n";
pagina +="  </head>\n";
pagina +="  <body>\n";
pagina +="    <h1>CONTROL DE PARQUEOS</h1>    \n";
pagina +="    <table class=\"table table-hover\">\n";
pagina +="  <thead class=\"table table-hover\">\"\n";
pagina +="    <tr class=\"table-active\">\n";
pagina +="      <th scope=\"col\">Parqueo #</th>\n";
pagina +="      <th scope=\"col\">Estado</th>\n";
pagina +="    </tr>\n";
pagina +="  </thead>\n";
pagina +="  <tbody>\n";
pagina +="    <tr>\n";

if (parqueo_1 == 1){
  pagina +="    <th scope=\"row\">Parqueo#1</th>\n";
  pagina +="    <td class=\"table table-hover\">Ocupado</td>\n";
  pagina +="    </tr>\n";
  pagina +="    <tr>\n";
}
else {
  pagina +="    <th scope=\"row\">Parqueo#1</th>\n";
  pagina +="    <td class=\"table table-hover\">Disponible</td>\n";
  pagina +="    </tr>\n";
  pagina +="    <tr>\n";
}
if (parqueo_2 == 1){
  pagina +="    <th scope=\"row\">Parqueo#2</th>\n";
  pagina +="    <td class=\"table table-hover\">Ocupado</td>\n";
  pagina +="    </tr>\n";
  pagina +="    <tr>\n";
}
else {
  pagina +="    <th scope=\"row\">Parqueo#2</th>\n";
  pagina +="    <td class=\"table table-hover\">Disponible</td>\n";
  pagina +="    </tr>\n";
  pagina +="    <tr>\n";
}
if (parqueo_3 == 1){
  pagina +="      <th scope=\"row\">Parqueo#3</th>\n";
  pagina +="      <td class=\"table table-hover\">Ocupado</td>\n";
  pagina +="    </tr>\n";
  pagina +="    <tr>\n";
}
else {
  pagina +="      <th scope=\"row\">Parqueo#3</th>\n";
  pagina +="      <td class=\"table table-hover\">Disponible</td>\n";
  pagina +="    </tr>\n";
  pagina +="    <tr>\n";
}
if (parqueo_4 == 1){
  pagina +="      <th scope=\"row\">Parqueo#4</th>\n";
  pagina +="      <td class=\"table table-hover\">Ocupado</td>\n";
  pagina +="    </tr>\n";
  pagina +="    <tr>\n";
}
else {
  pagina +="      <th scope=\"row\">Parqueo#4</th>\n";
  pagina +="      <td class=\"table table-hover\">Disponible</td>\n";
  pagina +="    </tr>\n";
  pagina +="    <tr>\n";
}
if (parqueo_5 == 1){
  pagina +="    <th scope=\"row\">Parqueo#5</th>\n";
  pagina +="    <td class=\"table table-hover\">Ocupado</td>\n";
  pagina +="    </tr>\n";
  pagina +="    <tr>\n";
}
else {
  pagina +="    <th scope=\"row\">Parqueo#5</th>\n";
  pagina +="    <td class=\"table table-hover\">Disponible</td>\n";
  pagina +="    </tr>\n";
  pagina +="    <tr>\n";
}
if (parqueo_6 == 1){
  pagina +="    <th scope=\"row\">Parqueo#6</th>\n";
  pagina +="    <td class=\"table table-hover\">Ocupado</td>\n";
  pagina +="    </tr>\n";
  pagina +="    <tr>\n";
}
else {
  pagina +="    <th scope=\"row\">Parqueo#6</th>\n";
  pagina +="    <td class=\"table table-hover\">Disponible</td>\n";
  pagina +="    </tr>\n";
  pagina +="    <tr>\n";
}
if (parqueo_7 == 1){
  pagina +="    <th scope=\"row\">Parqueo#7</th>\n";
  pagina +="    <td class=\"table table-hover\">Ocupado</td>\n";
  pagina +="    </tr>\n";
  pagina +="    <tr>\n";
}
else {
  pagina +="    <th scope=\"row\">Parqueo#7</th>\n";
  pagina +="    <td class=\"table table-hover\">Disponible</td>\n";
  pagina +="    </tr>\n";
  pagina +="    <tr>\n";
}
if (parqueo_8 == 1){
  pagina +="    <th scope=\"row\">Parqueo#8</th>\n";
  pagina +="    <td class=\"table table-hover\">Ocupado</td>\n";
  pagina +="    </tr>\n";
  pagina +="    <tr>\n";
}
else {
  pagina +="    <th scope=\"row\">Parqueo#8</th>\n";
  pagina +="    <td class=\"table table-hover\">Disponible</td>\n";
  pagina +="    </tr>\n";
  pagina +="    <tr>\n";
}

pagina +="  </tbody>\n";
pagina +="  </tfoot>\n";
pagina +="  \t<tr>\n";
pagina += "<th class=table-active scope=row>Parqueos Libres</th>\n";
pagina += "<td class=table-info>";
pagina += (int) parq_disp; //Variable de la operacion para mostrar el resultado. 
pagina += "</td>\n";
pagina +="    </tr>\n";
pagina +="  </tfoot>\n";
pagina +="</table>\n";
pagina +="<script>\n";
pagina +="function timedRefresh(timeoutPeriod) {\n";
pagina +="\tsetTimeout(\"location.reload(true);\",timeoutPeriod);}\n";
pagina +="window.onload = timedRefresh(1000);\n";
pagina +="</script>\n";
pagina +="    <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-gtEjrD/SeCtmISkJkNUaaKMoLD0//ElJ19smozuHV6z3Iehds+3Ulb9Bn9Plx0x4\" crossorigin=\"anonymous\"></script>\n";
pagina +="    <script src=\"https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.2/dist/umd/popper.min.js\" integrity=\"sha384-IQsoLXl5PILFhosVNubq5LC7Qb9DXgDA9i+tQ8Zj3iwWAwPtgFTxbJ8NT4GN1R8p\" crossorigin=\"anonymous\"></script>\n";
pagina +="    <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.min.js\" integrity=\"sha384-Atwg2Pkwv9vp0ygtn1JAojH0nYbwNJLPhwyoVbhoPwBhjQPR5VtM2+xf0Uwh9KtT\" crossorigin=\"anonymous\"></script>\n";
pagina +="  </body>\n";
pagina +="</html>";
return pagina;
}


String SendHTML(uint8_t led1stat) {  
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #FFFFFF;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #000000;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP32 Web Server &#128664</h1>\n";
  ptr += "<h3>Ejemplo de Web Server</h3>\n";

  if (led1stat)
  {
    ptr += "<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
  }
  else
  {
    ptr += "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";
  }

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

void disp(uint8_t segment) {

  switch (segment) {

    case 0: //0
      digitalWrite(26, HIGH); //a
      digitalWrite(25, HIGH); //b
      digitalWrite(18, HIGH); //c
      digitalWrite(21, HIGH); //d
      digitalWrite(19, HIGH); //e
      digitalWrite(32, HIGH); //f
      digitalWrite(33, LOW); //g
      break;

    case 1: //1
      digitalWrite(26, LOW); //a
      digitalWrite(25, HIGH); //b
      digitalWrite(18, HIGH); //c
      digitalWrite(21, LOW); //d
      digitalWrite(19, LOW); //e
      digitalWrite(32, LOW); //f
      digitalWrite(33, LOW); //g
      break;
    case 2: //2
      digitalWrite(26, HIGH); //a
      digitalWrite(25, HIGH); //b
      digitalWrite(18, LOW); //c
      digitalWrite(21, HIGH); //d
      digitalWrite(19, HIGH); //e
      digitalWrite(32, LOW); //f
      digitalWrite(33, HIGH); //g
      break;
    case 3: //3
      digitalWrite(26, HIGH); //a
      digitalWrite(25, HIGH); //b
      digitalWrite(18, HIGH); //c
      digitalWrite(21, HIGH); //d
      digitalWrite(19, LOW); //e
      digitalWrite(32, LOW); //f
      digitalWrite(33, HIGH); //g
      break;
    case 4: //4
      digitalWrite(26, LOW); //a
      digitalWrite(25, HIGH); //b
      digitalWrite(18, HIGH); //c
      digitalWrite(21, LOW); //d
      digitalWrite(19, LOW); //e
      digitalWrite(32, HIGH); //f
      digitalWrite(33, HIGH);
      break;
    case 5: //5
      digitalWrite(26, HIGH); //a
      digitalWrite(25, LOW); //b
      digitalWrite(18, HIGH); //c
      digitalWrite(21, HIGH); //d
      digitalWrite(19, LOW); //e
      digitalWrite(32, HIGH); //f
      digitalWrite(33, HIGH);
      break;
    case 6: //6
      digitalWrite(26, HIGH); //a
      digitalWrite(25, LOW); //b
      digitalWrite(18, HIGH); //c
      digitalWrite(21, HIGH); //d
      digitalWrite(19, HIGH); //e
      digitalWrite(32, HIGH); //f
      digitalWrite(33, HIGH);
      break;
    case 7: //7
      digitalWrite(26, HIGH); //a
      digitalWrite(25, HIGH); //b
      digitalWrite(18, HIGH); //c
      digitalWrite(21, LOW); //d
      digitalWrite(19, LOW); //e
      digitalWrite(32, LOW); //f
      digitalWrite(33, LOW);
      break;
    case 8: //8
      digitalWrite(26, HIGH); //a
      digitalWrite(25, HIGH); //b
      digitalWrite(18, HIGH); //c
      digitalWrite(21, HIGH); //d
      digitalWrite(19, HIGH); //e
      digitalWrite(32, HIGH); //f
      digitalWrite(33, HIGH);
      break;
    default:
      digitalWrite(26, HIGH); //a
      digitalWrite(25, HIGH); //b
      digitalWrite(18, HIGH); //c
      digitalWrite(21, HIGH); //d
      digitalWrite(19, HIGH); //e
      digitalWrite(32, HIGH); //f
      digitalWrite(33, HIGH);
      break;

  }
}
