/*
 Melody
 Plays a melody
 Circuit:
 * 8-ohm speaker on digital pin 8
 created 21 Jan 2010
 modified 30 Aug 2011
 by Tom Igoe
 modified 7 Feb 2014
 by Mark Easley
 This example code is in the public domain.
 Modified code to include more sounds for the passive buzzer and Tiva-c
*/

#include "pitches.h"

void tone_function(int mel[], int duration[], int quant);
int input = 0;
// Menu music
int melody_menu[] = { NOTE_B4, NOTE_B4, NOTE_B4, 0, NOTE_B4, NOTE_B4,
                      NOTE_B4,NOTE_DS5, NOTE_B4, NOTE_B4, NOTE_A4, NOTE_G4, 
                      NOTE_A4, NOTE_B4, NOTE_B4, NOTE_B4, NOTE_B4, 0,   
                      NOTE_B4, NOTE_B4, NOTE_B4,NOTE_DS5, NOTE_B4, NOTE_B4, 
                      NOTE_A4, NOTE_G4, NOTE_A4, NOTE_B4}; // 28

// Start melody
int melody_start[] = {NOTE_C4, NOTE_F4, NOTE_A4, NOTE_F4, NOTE_F4, 
                      NOTE_C4, NOTE_F4, NOTE_A4, NOTE_G4}; // 9
                
// Collision w/ wall
int melody_collision[] = {NOTE_DS8, NOTE_B0}; // 2


//Game over music
int melody_over[] = {NOTE_DS4, NOTE_D4, NOTE_CS4, NOTE_CS3}; // 4


// Point
int melody_point[] = {NOTE_C8}; //1


// note durations: 4 = quarter note, 8 = eighth note, etc.:
// Menu music
int noteDurations_menu[] = {6,6,6,2,6, 6,6,4,4,4,
                            2,2,2,2,6,6,6,2,6,6,
                            6,4,4,4, 2, 2, 2, 2};

// Start melody
int noteDurations_start[] = {8,8,8,8,8,
                              4,4,8,8};
                       
// Collision
int noteDurations_collision[] = {4, 2};



// Game over music
int noteDurations_over[] = {3, 3, 2, 1};

// Point
int noteDurations_point[] = {8};

void setup(){
  Serial.begin(9600); 
    delay(2000);  
 
    Serial.println("Type something!");
}
void tone_function(int mel[], int duration[], int quant) {
  //int sizeMel = sizeof mel/sizeof mel[0];
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 30; thisNote++) {
    // to calculate the note duration, take one second
    // divided by the note type. //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int durations = 1000/duration[thisNote];
    tone(PF_4, mel[thisNote],durations); //important to use PF_4 'cause of the PWM signal
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = durations * 1.30; 
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(PF_4);
    if(thisNote == (quant-1)){
      break;
    }
  }
}

void loop() {
   if(Serial.available()){
        input = Serial.read();
        if (input == 49){
          tone_function(melody_menu, noteDurations_menu, 28);
         
        }
        else if (input == 50){
          tone_function(melody_start, noteDurations_start, 9);
         
        }
        else if (input == 51){
          tone_function(melody_collision, noteDurations_collision, 3);
          
        }
        else if (input == 52){
          tone_function(melody_point, noteDurations_point, 1);
          
        }
        else if (input == 53){
          tone_function(melody_over, noteDurations_over, 4);
          
        }
    } 
}
