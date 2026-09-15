#include "MIDIUSB.h"
#include "PitchToNote.h"

#define A0 18
#define A1 19
#define A2 20
#define A3 21
#define A4 22
#define A5 23
#define A6 24
#define A7 25
#define A8 26
#define A9 27

void setup() {
  Serial.begin(9600);
}

// buttons and keys
const uint8_t buttons[] = {A0, A1, A2, A3, A4, A5, A6, A7};
const byte notePitches[] = {C5, D5, E5, F5, G5, A5b, B5, C6};


uint8_t pressedButtons = 0x00;
uint8_t previousButtons = 0x00;

uint8_t velocity = 127; // 64 = normal, 127 = fastest
uint8_t playThreshold = 300; 

void loop() {
  readButtons();
  playNotes();
}

void readButtons()
{
  for (int i = 0; i < sizeof(buttons); i++)
  {
    if (analogRead(buttons[i]) < playThreshold)
    {
      bitWrite(pressedButtons, i, 1);
      delay(100);
    }
    else
      bitWrite(pressedButtons, i, 0);
  }
}

void playNotes()
{
  for (int i = 0; i < sizeof(buttons); i++)
  {
    if (bitRead(pressedButtons, i) != bitRead(previousButtons, i))
    {
      if (bitRead(pressedButtons, i))
      {
        bitWrite(previousButtons, i , 1);
        noteOn(0, notePitches[i], velocity);
        MidiUSB.flush();
      }
      else
      {
        bitWrite(previousButtons, i , 0);
        noteOff(0, notePitches[i], 0);
        MidiUSB.flush();
      }
    }
  }
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}