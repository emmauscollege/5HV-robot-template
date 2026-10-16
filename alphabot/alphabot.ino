/*****************************************
   Alphabot Startcode
   met toestandsdiagrammen
   Emmauscollege
   v20261016CAM
   Met dank aan de documentatie op: 
   https://www.waveshare.com/wiki/AlphaBot2-Ar
 *****************************************/
 
/*****************************************
   variabelen 
 *****************************************/
// gebruikte pinnen
const int pinPWMA = 6;           //Left Motor Speed pin (ENA)
const int pinAIN2 = A0;          //Motor-L forward (IN2).
const int pinAIN1 = A1;          //Motor-L backward (IN1)
const int pinPWMB = 5;           //Right Motor Speed pin (ENB)
const int pinBIN1 = A2;          //Motor-R forward (IN3)
const int pinBIN2 = A3;          //Motor-R backward (IN4)
const int pinECHO = 2;           //ultrasonic distance sensor echo pin
const int pinTRIG = 3;           //ultrasonic distance sensor trigger pin

// variabelen om waarden van sensoren en actuatoren te onthouden
int afstandVoor = 0;

// variabelen voor de toestanden
const int VOORUIT = 1; // rij vooruit
const int DRAAI   = 2; // draai
int toestand = VOORUIT;
unsigned long toestandStartTijd = 0;

/*****************************************
   functies 
 *****************************************/

// Meet de afstand in centimeters met behulp van de ultrasone afstandssensor
int ultrasonicDistance(int pinTrigger, int pinEcho)         
{
  float distance_ms = 0;
  int distance_cm = 0;
  digitalWrite(pinTrigger, LOW);   // set trig pin low 2μs
  delayMicroseconds(2);
  digitalWrite(pinTrigger, HIGH);  // set trig pin 10μs , at last 10us 
  delayMicroseconds(10);
  digitalWrite(pinTrigger, LOW);   // set trig pin low
  distance_ms = pulseIn(pinEcho, HIGH);  // Read echo pin high level time(μs)
  distance_cm = distance_ms / 58;       //cm = μs /58       
  return distance_cm;
}

/*****************************************
   setup() en loop()
 *****************************************/

// deze functie wordt 1x uitgevoerd als de arduino start
void setup() {
  // start seriële communicatie en stuur opstartbericht naar Serial Monitor
  Serial.begin(9600);
  Serial.println("Alphabot start");

  // zet pinmodes
  pinMode(pinECHO, INPUT);    // ultrasonic echo input pin
  pinMode(pinTRIG, OUTPUT);   // ultrasonic trigger output pin 
  pinMode(pinPWMA, OUTPUT);   // linker motor snelheid analog output               
  pinMode(pinAIN2, OUTPUT);   // linker motor richting digital output   
  pinMode(pinAIN1, OUTPUT);   // linker motor richting digital output 
  pinMode(pinPWMB, OUTPUT);   // rechter motor snelheid analog output   
  pinMode(pinAIN1, OUTPUT);   // rechter motor richting digital output  
  pinMode(pinAIN2, OUTPUT);   // rechter motor richting digital output
}

// deze functie wordt oneindig herhaald
void loop() {
  // STAP 1: lees sensorwaarden
  afstandVoor = ultrasonicDistance(pinTRIG, pinECHO);
  Serial.print("afstandVoor: ");
  Serial.print(afstandVoor);
  Serial.println(" cm");

  // STAP 2: bepaal of de toestand moet veranderen
  if (toestand == VOORUIT) {                        // als toestand VOORUIT is
    if (afstandVoor < 50) {                         // afstand < 50 cm
      toestandStartTijd = millis();                 // reset tijd
      toestand = DRAAI;                             // zet nieuwe toestand
      Serial.println("Nieuwe toestand: DRAAI");
    }
    // schrijf hier je eigen code voor een 2e pijl uit toestand VOORUIT. Haal hiervoor /* en */ weg
    /*
    if () {                                         // voorwaarde voor 2e pijl uit toestand VOORUIT invullen
                                                    // code invullen
    }
    */
  }
  if (toestand == DRAAI) {                          // als toestand DRAAI is
    if (millis() - toestandStartTijd > 1000) {      // 1 seconde voorbij
      toestandStartTijd = millis();                 // reset tijd
      toestand = VOORUIT;                           // zet nieuwe toestand
      Serial.println("Nieuwe toestand: VOORUIT");
    }
  }

  // STAP 3: zet actuatoren zoals hoort bij de toestand
  if (toestand == VOORUIT) {
    int speed = 100;            // snelheid 0 t/m 255
    analogWrite(pinPWMA,Speed); // snelheid linkermotor
    analogWrite(pinPWMB,Speed); // snelheid rechtermotor
    digitalWrite(pinAIN1,LOW);  // linker motor vooruit
    digitalWrite(pinAIN2,HIGH); // linker motor vooruit
    digitalWrite(pinBIN1,LOW);  // rechter motor vooruit 
    digitalWrite(pinBIN2,HIGH); // rechter motor vooruit 
  }
  if (toestand == DRAAI) {
    int speed = 50;             // snelheid 0 t/m 255
    analogWrite(pinPWMA,Speed); // snelheid linkermotor
    analogWrite(pinPWMB,Speed); // snelheid rechtermotor
    digitalWrite(pinAIN1,LOW);  // linker motor vooruit
    digitalWrite(pinAIN2,HIGH); // linker motor vooruit
    digitalWrite(pinBIN1,HIGH); // rechter motor achteruit  
    digitalWrite(pinBIN2,LOW);  // rechter motor achteruit  
  }

  // vertraging om te zorgen dat berichten op de seriele monitor leesbaar blijven
  delay(50);
}