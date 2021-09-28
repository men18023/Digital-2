// Laboratorio 7 - Manejo de Memoria SD
// Jonathan Menéndez, 18023
// link video: https://youtu.be/4NJCcQzfPuE
// link repositorio:

//librerias utilizadas
#include <SPI.h>
#include <SD.h>

// Variables
Sd2Card card;
SdVolume volume;
SdFile root;
char get;

const int chipSelect = PA_3; //cs PIN

// para mostrar del SD
File dataFile;

void setup() {

  Serial.begin(9600);       //comunicacion serial
  while (!Serial) {         //
    ;
  }
  SPI.setModule(0);         //para poder comunicarse con la SD
  delay(2500);              //abre la terminal serial
  Serial.print("\nInitializing SD card...\n");
  pinMode(PA_3, OUTPUT);     // pin 12/ para utilizar como cs PIN


  if (!card.init(SPI_HALF_SPEED, chipSelect)) {     //inicio en configuración para prueba de SD
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present.\n");
  }

  if (!volume.init(card)) {       //abre los datos en orden
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card\n");
    return;
  }

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);              //abre la infromacion en la SD
  root.ls(LS_R | LS_DATE | LS_SIZE);  //muestra los datos en un orden
  delay(1000);                        
  root.close();                       //cerrar la root


  if (!SD.begin(chipSelect)) {        //Inicio la comunicacion a la SD en read y write
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("\nCard initialized as read/write.\n");

  Serial.println("Envie el numero del documento que desea ver:");
  Serial.println("(1) si quiere mostrar PELOTA.TXT");
  Serial.println("(2) si quiere mostrar MADRID.TXT");
  Serial.println("(3) si quiere mostrar LIVER.TXT");
}

void loop() {
  get = Serial.read(); //asigna la lectura del serial a la variable
  //char  a = (get, DEC);
  switch (get) {  //utiliza el valor leido del serial para mostrar la imagen en ascii.txt
    case '1':
      dataFile = SD.open("pelota.txt");
      if (dataFile) {

        // read from the file until there's nothing else in it:
        while (dataFile.available()) {
          Serial.write(dataFile.read());
        }
        // close the file:
        dataFile.close();
      } else {
        // if the file didn't open, print an error:
        Serial.println("error opening pelota.txt");
      }
      Serial.println("Envie el numero de la documento que desea ver:");
      Serial.println("(1) si quiere mostrar PELOTA.TXT");
      Serial.println("(2) si quiere mostrar MADRID.TXT");
      Serial.println("(3) si quiere mostrar LIVER.TXT");
      break;

    case '2':
      dataFile = SD.open("madrid.txt");
      if (dataFile) {

        // read from the file until there's nothing else in it:
        while (dataFile.available()) {
          Serial.write(dataFile.read());
        }
        // close the file:
        dataFile.close();
      } else {
        // if the file didn't open, print an error:
        Serial.println("error opening madrid.txt");
      }
      Serial.println("Envie el numero de la documento que desea ver:");
      Serial.println("(1) si quiere mostrar PELOTA.TXT");
      Serial.println("(2) si quiere mostrar MADRID.TXT");
      Serial.println("(3) si quiere mostrar LIVER.TXT");
      break;

    case '3':
      dataFile = SD.open("liver.txt");
      if (dataFile) {

        // read from the file until there's nothing else in it:
        while (dataFile.available()) {
          Serial.write(dataFile.read());
        }
        // close the file:
        dataFile.close();
      } else {
        // if the file didn't open, print an error:
        Serial.println("error opening liver.txt");
      }
      Serial.println("Envie el numero de la documento que desea ver:");
      Serial.println("(1) si quiere mostrar PELOTA.TXT");
      Serial.println("(2) si quiere mostrar MADRID.TXT");
      Serial.println("(3) si quiere mostrar LIVER.TXT");
      break;
  }
}
