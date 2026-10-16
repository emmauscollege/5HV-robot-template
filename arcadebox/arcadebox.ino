/*****************************************
   Arcadebox startcode
   met toestandsdiagrammen
   Emmauscollege
   v20261016CAM
 *****************************************/

/*****************************************
   libraries die je gebruikt 

   LCD met 2 regels van 16 karakters 
   (de RGB backlit kan niet van kleur worden veranderd)
   https://www.arduino.cc/reference/en/libraries/grove-lcd-rgb-backlight/

 *****************************************/
#include <Wire.h>
#include "src/Grove_LCD_RGB_Backlight/rgb_lcd.h"

/*****************************************
   variabelen
 *****************************************/
// gebruikte pinnen
const int pinLedBlauw   = 11; 
const int pinLedGroen   = 5; 
const int pinLedGeel    = 9; 
const int pinLedWit     = 3; 
const int pinKnopBlauw  = 6; 
const int pinKnopGroen  = 10; 
const int pinKnopGeel   = 4; 
const int pinKnopWit    = 8; 
const int pinSpeaker    = 12;

// object voor LCD-scherm
rgb_lcd lcd; // maak variabele lcd van het type rgb_lcd om het display te gebruiken

// variabelen om waarden van sensoren en actuatoren te onthouden
int waardeKnopBlauw = HIGH;
int waardeKnopGroen = HIGH;
int waardeKnopGeel  = HIGH;
int waardeKnopWit   = HIGH;
long int startTijd  = millis();

// variabelen voor de toestanden
const int TELAF = 1; // tel af tot spel start
const int SPEEL = 2; // speel het spel
const int WIN   = 3; // laat zien wie de winnaar is
int toestand = TELAF;
unsigned long toestandStartTijd = 0;

/*****************************************
   functies
 *****************************************/

/*****************************************
   setup() en loop()
 *****************************************/

void setup() {
  // enable console en stuur opstartbericht
  Serial.begin(9600);
  Serial.println("Arcade start");

  // zet pinmode voor leds
  pinMode(pinLedBlauw, OUTPUT);
  pinMode(pinLedGroen, OUTPUT);
  pinMode(pinLedGeel, OUTPUT);
  pinMode(pinLedWit, OUTPUT);
  
  // zet pinmode voor knoppen
  // de knoppen werken met een ingebouwde pullup weerstand in de arduino
  // de knop ingedrukt geeft HIGH, de knop los geeft LOW
  pinMode(pinKnopBlauw, INPUT_PULLUP);
  pinMode(pinKnopGroen, INPUT_PULLUP);
  pinMode(pinKnopGeel, INPUT_PULLUP);
  pinMode(pinKnopWit, INPUT_PULLUP);

  // zet pinmode voor speaker
  pinMode(pinSpeaker, OUTPUT);

  // init display
  lcd.begin(16,2);
  
}

void loop() {
  // STAP 1: lees sensorwaarden
  waardeKnopBlauw = digitalRead(pinKnopBlauw);
  waardeKnopGroen = digitalRead(pinKnopGroen);
  waardeKnopGeel = digitalRead(pinKnopGeel);
  waardeKnopWit = digitalRead(pinKnopWit);

  // STAP 2: bepaal of de toestand moet veranderen
  if (toestand == TELAF) {
    if (millis() - startTijd > 2000) {           // 2 seconden voorbij
      startTijd = millis();
      toestand = SPEEL;
      Serial.println("Nieuwe toestand: SPEEL");
    }
  }
  if (toestand == SPEEL) {
    if (waardeKnopBlauw == LOW ||
        waardeKnopGroen == LOW ||
        waardeKnopGeel  == LOW ||
        waardeKnopWit   == LOW   ) {             // een knop ingedrukt
      startTijd = millis();
      toestand = WIN;
      Serial.println("Nieuwe toestand: WIN");
    }
  }
  if (toestand == WIN) {
    if (millis() - startTijd > 1000) {           // 0,5 seconde voorbij
      startTijd = millis();
      toestand = TELAF;
      Serial.println("Nieuwe toestand: TELAF");
    }
  }

  // STAP 3: zet actuatoren
  if (toestand == TELAF) {
    digitalWrite(pinLedBlauw, HIGH);
    digitalWrite(pinLedGroen, HIGH);
    digitalWrite(pinLedGeel , HIGH);
    digitalWrite(pinLedWit  , HIGH); 
    lcd.setCursor(0, 0);
    lcd.print("TELAF:wacht    ");          
  }
  if (toestand == SPEEL) {
    digitalWrite(pinLedBlauw, LOW);
    digitalWrite(pinLedGroen, LOW);
    digitalWrite(pinLedGeel , LOW);
    digitalWrite(pinLedWit  , LOW); 
    lcd.setCursor(0, 0);
    lcd.print("SPEEL:druk knop");    
  }
  if (toestand == WIN) {
    tone(pinSpeaker, 500, 100);                  // speaker piept 100ms op 500hz, maar programma gaat door terwijl speaker piept
    lcd.setCursor(0, 0);
    lcd.print("WIN:n-joy sound");   
  }

  // kleine vertraging, 100 keer per seconde loopen is genoeg
  delay(10);
}
