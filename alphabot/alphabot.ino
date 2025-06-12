/*****************************************
   Alphabot Startcode met toestanden
   Emmauscollege - Verbeterde versie
 *****************************************/

// gebruikte pinnen
const int pinPWMA = 6;
const int pinAIN2 = A0;
const int pinAIN1 = A1;
const int pinPWMB = 5;
const int pinBIN1 = A2;
const int pinBIN2 = A3;
const int pinECHO = 2;
const int pinTRIG = 3;

// sensorgegevens
int afstandVoor = 0;

// toestanden
const int VOORUIT = 1;
const int DRAAI   = 2;
const int CHECK   = 3;
const int DRAAIRECHTS = 4;
int toestand = VOORUIT;
unsigned long toestandStartTijd = 0;

// afstand meten
int utrasonicDistance(int pinTrigger, int pinEcho) {
  float distance_ms = 0;
  int distance_cm = 0;
  digitalWrite(pinTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrigger, LOW);
  distance_ms = pulseIn(pinEcho, HIGH);
  distance_cm = distance_ms / 58;
  return distance_cm;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Alphabot start");

  pinMode(pinECHO, INPUT);
  pinMode(pinTRIG, OUTPUT);
  pinMode(pinPWMA, OUTPUT);
  pinMode(pinAIN2, OUTPUT);
  pinMode(pinAIN1, OUTPUT);
  pinMode(pinPWMB, OUTPUT);
  pinMode(pinBIN1, OUTPUT);
  pinMode(pinBIN2, OUTPUT);
}

void loop() {
  afstandVoor = utrasonicDistance(pinTRIG, pinECHO);
  Serial.print("afstandVoor: ");
  Serial.print(afstandVoor);
  Serial.println(" cm");

  // TOESTANDEN
  if (toestand == VOORUIT) {
    if (afstandVoor < 7 && toestand != DRAAIRECHTS) {
      toestandStartTijd = millis();
      toestand = DRAAI;
      Serial.println("Nieuwe toestand: DRAAI");
    } else if (afstandVoor >= 7 && millis() - toestandStartTijd >= 800) {
      toestandStartTijd = millis();
      toestand = CHECK;
      Serial.println("Nieuwe toestand: CHECK");
    }
  }

  else if (toestand == CHECK) {
    if (millis() - toestandStartTijd > 350) {
      toestandStartTijd = millis();
      
      if (afstandVoor < 7) {
        toestand = DRAAIRECHTS;
      }
      if (afstandVoor > 7) {
        toestand= VOORUIT;
         Serial.println("Nieuwe toestand: VOORUIT");
      }
    }
  }

  else if (toestand == DRAAI) {
    // Tijd instellen voor de draai naar links
    if (millis() - toestandStartTijd > 1000) {
      toestandStartTijd = millis();
      toestand = VOORUIT;
      Serial.println("Nieuwe toestand: VOORUIT");
    }
  }
  else if (toestand == DRAAIRECHTS) {
    // Tijd instellen voor de draai naar rechts (iets korter dan de draai naar links)
    if (millis() - toestandStartTijd > 37) {
      toestandStartTijd = millis();
      toestand= VOORUIT;
    }
  }

  // ACTUATOREN
  if (toestand == VOORUIT) {
    int Speed = 100;
    analogWrite(pinPWMA, Speed);
    analogWrite(pinPWMB, Speed);
    digitalWrite(pinAIN1, LOW);
    digitalWrite(pinAIN2, HIGH);
    digitalWrite(pinBIN1, LOW);
    digitalWrite(pinBIN2, HIGH);
  }

  else if (toestand == DRAAI) {
    // Draai naar links
    int Speed = 50;
    unsigned long draaiTijd = 1000;  // Tijd voor de draai naar links

    analogWrite(pinPWMA, Speed);
    analogWrite(pinPWMB, Speed);
    digitalWrite(pinAIN1, LOW);
    digitalWrite(pinAIN2, HIGH);
    digitalWrite(pinBIN1, HIGH);
    digitalWrite(pinBIN2, LOW);
    
    if (millis() - toestandStartTijd > draaiTijd) {
      toestandStartTijd = millis();
      toestand = VOORUIT;
      Serial.println("Nieuwe toestand: VOORUIT");
    }
  }
  
  else if (toestand == DRAAIRECHTS) {
    // Draai naar rechts
    int Speed = 100;
    unsigned long draaiTijd = 1000;  // Tijd voor de draai naar rechts (korter dan links)
     Serial.println("Nieuwe toestand: DraaiRechts");

    analogWrite(pinPWMA, Speed);
    analogWrite(pinPWMB, Speed);
    digitalWrite(pinAIN1, LOW);
    digitalWrite(pinAIN2, HIGH);
    digitalWrite(pinBIN1, HIGH);
    digitalWrite(pinBIN2, LOW);

    if (millis() - toestandStartTijd > draaiTijd) {
      toestandStartTijd = millis();
      toestand = VOORUIT;
      Serial.println("Nieuwe toestand: VOORUIT");
    }
  }

  else if (toestand == CHECK) {
    // tijdens CHECK mag de robot even niets doen
    int Speed = 50;
    analogWrite(pinPWMA, Speed);
    analogWrite(pinPWMB, Speed);
    digitalWrite(pinAIN1, HIGH);
    digitalWrite(pinAIN2, LOW);
    digitalWrite(pinBIN1, LOW);
    digitalWrite(pinBIN2, HIGH);
  }

  delay(50);
}
