/*
  Jack Pop's Erschreck-O-Mat
  Version: 1.0
  Stand: Juli 2025

  Dieser Arduino-Sketch ist Code für ein kleines, feines Elektronikprojekt,
  bei dem ein Arduino Micro Pro mit einem Ultraschall-Abstandssensor (HC-SR04),
  einem Mp3-Modul (DF Player) und einem Mini-Lautsprecher verbunden wird, 
  um immer dann einen Sound abzuspielen, wenn sich etwas vor dem Abstandssensor bewegt.

  Dokumentation:
  https://github.com/le-jackpop/erschreck-o-mat/

  Haftungsausschluss:
  Dieses Projekt dient ausschließlich zu Bildungs- und Demonstrationszwecken.
  Der Nachbau erfolgt auf eigene Gefahr. Es wird keinerlei Haftung für Schäden 
  an Personen, Tieren oder Gegenständen übernommen. Elektronische Arbeiten 
  sollten nur mit entsprechender Fachkenntnis durchgeführt werden.

  Lizenz: MIT License

  Copyright (c) 2025 Jack Pop
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*/

#include "SoftwareSerial.h"

// === PIN-Belegung ===
const int ECHO_PIN = 2;         // Echo-Pin vom Ultraschallsensor
const int TRIGGER_PIN = 3;      // Trigger-Pin vom Ultraschallsensor
const int VCC_PIN = 4;          // Stromversorgung von Ultraschallsensor (muss auf HIGH gesetzt sein)
SoftwareSerial mySerial(6, 5);  // Software-Serial vom DF Player → TX, RX

// === einstellbare Variablen ===
const int triggerAbstand = 4;  // Ab welchem Abstand soll Sound losgehen? (in cm)
const int soundVolume = 25;    // Wie laut soll Sound angespielt werden? (Werte möglich von 0 - 30)

// === Referenzabstand in cm ===
long referenceDistance = 0;

// === Hilfsfunktion zur Abstandsmessung ===
long measureDistance() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration / 58;  // µs → cm
  return distance;
}

// === DFPlayer Befehlskonstanten & Variablen ===
#define Start_Byte 0x7E
#define Version_Byte 0xFF
#define Command_Length 0x06
#define End_Byte 0xEF
#define Acknowledge 0x01
bool playedOnce = false;  // prüft, ob Sound schon einmal abgespielt wurde

// === DFPlayer-Befehlsfunktionen ===
void execute_CMD(byte CMD, byte Par1, byte Par2) {
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
  byte Command_line[10] = {
    Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
    Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte
  };

  for (byte k = 0; k < 10; k++) {
    mySerial.write(Command_line[k]);
  }
}

void module_init() {
  execute_CMD(0x0C, 0, 0);  // reset
  delay(1500);
  set_eq(0);
  delay(200);
  set_volume(soundVolume);
  delay(200);
  play_mp3(0);  // 3-Sekunden-Countdown-Sound
}

void set_eq(uint8_t eq) {
  execute_CMD(0x07, 0, eq);
}

void set_volume(uint8_t volume) {
  execute_CMD(0x06, 0, volume);
}

void play_mp3(uint8_t track) {
  execute_CMD(0x12, 0, track);  // 0000.mp3 = track 0
  delay(500);
}

void setup() {
  // Serielle Ausgabe für Debugging:
  // Serial.begin(115200);

  mySerial.begin(9600);  // Serielle Kommunikation über Software für DF Player
  module_init();         // DF Player initialisieren

  pinMode(VCC_PIN, OUTPUT);
  digitalWrite(VCC_PIN, HIGH);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  delay(3500);  // Warten, bis 3-Sekunden-Countdown gespielt wurde

  // Referenzabstand beim Start messen
  referenceDistance = measureDistance();

  // Serial.print("Referenzabstand: ");
  // Serial.print(referenceDistance);
  // Serial.println(" cm");

  delay(500);  // Sensor kurz Pause geben vor der ersten Messung im Loop
}

void loop() {
  if (playedOnce) {
    return;  // Wenn Sound schon abgespielt wurde, soll Gerät nichts mehr machen
  }

  long currentDistance = measureDistance();
  long difference = abs(currentDistance - referenceDistance);

  // Serial.print("Aktuell: ");
  // Serial.print(currentDistance);
  // Serial.print(" cm → Differenz: ");
  // Serial.println(difference);

  if (difference >= triggerAbstand) {
    play_mp3(1);        // Sound abspielen bei Veränderung
    playedOnce = true;  // blockiert zukünftige Messungen
  }

  delay(300);  // etwas warten, um Sensor & Ton nicht zu überlasten
}
