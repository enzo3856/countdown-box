/* 
  Nokia Tune
  Connect a piezo buzzer or speaker to pin 11 or select a new pin.
  More songs available at https://github.com/robsoncouto/arduino-songs                                            
                                              
                                              Robson Couto, 2019
*/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

const int freq1 = 220; //Low Note A
const int freq2 = freq1 * 2; //Med
const int freq3 = freq1 * 3; //Hi
const int freq4 = freq1 * 4; //Very Hi

// change this to make the song slower or faster
int tempo = 180;

// change this to whichever pin you want to use
int buzzer = 12;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  // Nokia Ringtone 
  // Score available at https://musescore.com/user/29944637/scores/5266155

  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4, 
  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4, 
  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
  NOTE_A4, 2, 
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

// ********************************************//
// My code now //
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "Pred_font.h"
#include <LiquidCrystal.h>

//LCD pin to Arduino
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
//const int pin_BL = 10; 
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

//#define CLK_PIN   13
//#define DATA_PIN  11
#define CS_PIN    3 //for nano testing - default is 3

int button1 = 2;

// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary output pins
// MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
int i = 0;

void setup(){
  P.begin();
  P.setIntensity(3);
  P.setTextAlignment(PA_CENTER);
  P.setPause(1000);
  P.setSpeed(100);
  P.displayClear();
  P.setTextAlignment(PA_CENTER);
  //P.print("WAIT");
  P.setFont(predator);

  pinMode(button1, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);//buzzer
  
 lcd.begin(16, 2);
 lcd.setCursor(0,0);
 lcd.print("**   Standby  **");
 lcd.setCursor(0,1);
 //lcd.print("Time:");

}

void makeBeep(int intervalSeconds, int howlongSeconds, int freq) {
  delay(intervalSeconds * 1000);
  tone(buzzer, freq, howlongSeconds * 1000);
}

void sing(void) {
  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}

void loop() 
{
  //lcd.noDisplay();
  //sing();
  P.setFont(predator);
  if (digitalRead(button1)){
    P.begin();
    P.setIntensity(3);
    P.setTextAlignment(PA_CENTER);
    P.setPause(1000);
    P.setSpeed(100);
    P.displayClear();
    P.setTextAlignment(PA_CENTER);
    //P.print("WAIT");
    P.setFont(predator);

    lcd.setCursor(0,0);
    lcd.print("** Started    **");
    P.setFont(predator);
    for (i=3600 ; i>= 0 ; i--) {
      if (i<11){
        makeBeep(1, 1, freq3);
        delay(200);
      }
      if (P.displayAnimate()){
        P.setTextAlignment(PA_RIGHT);
        P.print(i);
        lcd.setCursor(0,1);
        lcd.print("Time:         ");
        lcd.setCursor(10,1);
        lcd.print (i);
        delay(500);
      }
     
    }
    P.displayClear();
    P.displayReset();
    P.setFont(nullptr);
    P.setTextAlignment(PA_CENTER);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Get Moving");
        for (i=0; i< 5; i++){
      if (P.displayAnimate())
        P.displayText("* * GET MOVING * *", PA_CENTER, 50, 50, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    }
    sing();
    delay(2000);
    lcd.setCursor(0,1);
    lcd.print("Hit Reset Button");
    while(!digitalRead(button1)){
      if (P.displayAnimate())
        P.displayText("Hit Button to Reset  * * GET MOVING * *  ", PA_CENTER, 50, 50, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    }
    P.displayClear();
    lcd.clear();
    delay(3000);
    lcd.setCursor(0,0);
    lcd.print("**   Standby  **");
  }

}
