/*
 MelodyPlays a melodycircuit:
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
//Intro music
int melody_intro[] = { NOTE_B4, NOTE_B4, NOTE_B4, 0, NOTE_B4, NOTE_B4, NOTE_B4,NOTE_DS5, NOTE_B4, NOTE_B4, NOTE_A4, NOTE_G4, 
NOTE_A4, NOTE_B4, NOTE_B4, NOTE_B4, NOTE_B4, 0, NOTE_B4, NOTE_B4, NOTE_B4,NOTE_DS5, NOTE_B4, NOTE_B4, NOTE_A4, NOTE_G4, 
NOTE_A4, NOTE_B4}; //28

//Main menu melody
int melody_menu[] = {NOTE_C4, NOTE_F4, NOTE_A4, NOTE_F4, NOTE_F4, 
                NOTE_C4, NOTE_F4, NOTE_A4, NOTE_G4}; //9
                
//Failed shot
int melody_failed[] = {NOTE_DS8, NOTE_B0}; //LOSE SOUND 2


//Winner 
int melody_winner[] = {NOTE_B5, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_E5}; //8

//Game over music
int melody_over[] = {NOTE_DS4, NOTE_D4, NOTE_CS4, NOTE_CS3}; //GAME OVER 3


//No more bullets
int melody_bull[] = {NOTE_C8}; //1


// note durations: 4 = quarter note, 8 = eighth note, etc.:
//Intro music
int noteDurations_intro[] = {6,6,6,2,6, 6,6,4,4,4,2,
2,2,2,6,6,6,2,6,6,6,4,4,4, 2, 2, 2, 2};

//Main menu melody
int noteDurations_menu[] = {8,8,8,8,8,
                       4,4,8,8};
                       
//Failed shot
int noteDurations_failed[] = {4, 2};

//Winner
int noteDurations_winner[] = {4,4,4,4,4,4,4,4};

//Game over music
int noteDurations_over[] = {3, 3, 2, 1};

//No more bullets
int noteDurations_bull[] = {8};

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
        //Serial.println(input);
        if (input == 49){
          tone_function(melody_intro, noteDurations_intro, 28);
         
        }
        else if (input == 50){
          tone_function(melody_menu, noteDurations_menu, 9);
         
        }
        else if (input == 51){
          tone_function(melody_failed, noteDurations_failed, 3);
          
        }
        else if (input == 52){
          tone_function(melody_bull, noteDurations_bull, 1);
          
        }
        else if (input == 53){
          tone_function(melody_over, noteDurations_over, 4);
          
        }
    } 
}
