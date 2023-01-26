// =================
// PROJNAME
// ARDUINO UNO
// =================

#include <LiquidCrystal_I2C.h>

// Ultrasonic
const int TRIG_PIN = 9;
const int ECHO_PIN = 8;

// Temperature
const int pinTemp = A0;

// lcd
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Buzzer
int ctr = 0;
const int pinBuzzer = 4;

// Led
const int pinLedRed = 5;
const int pinLedOrange = 6;
const int pinLedGreen = 7;

// sim
bool statFlag = true;



// ============================
// Start
// ============================
void setup() 
{
  // init serial
  Serial.begin(9600);

  // init ultrasonic
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // init lcd
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 

  // init buzzer
  pinMode(pinBuzzer, OUTPUT);

  // init led
  pinMode(pinLedRed, OUTPUT);
  pinMode(pinLedOrange, OUTPUT);
  pinMode(pinLedGreen, OUTPUT);

  // init sim
  lcd.clear();
  lcd.setCursor(0, 0);        
  lcd.print("Bantay Baha"); 
  lcd.setCursor(0, 1);        
  lcd.print("Initializing");       
}



// ============================
// Loop
// ============================
void loop()
{
  // Read Ultrasonic 
  long duration, distanceCm, distanceIn;
 
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN,HIGH);
  distanceCm = duration / 29.1 / 2 ;
  distanceIn = duration / 74 / 2;

  delay(200);

  // Read Temperature
  int tempRead = analogRead(pinTemp);
  float voltage = tempRead * (5000 / 1024.0);
  float temperature = voltage / 10;

  delay(200);

  // Buzzer & Led
  if (distanceCm <= 5)
  {
    if (ctr >= 3)
    {
      digitalWrite(pinBuzzer, HIGH);
      digitalWrite(pinLedRed, HIGH);
      digitalWrite(pinLedOrange, LOW);
      digitalWrite(pinLedGreen, LOW);

      float waterPercent = 10.0 - distanceCm;
      waterPercent = waterPercent / 10.0;
      waterPercent = waterPercent * 100;
  
      if (statFlag)
      {
        Serial.print("AT\r");
        delay(200);
        Serial.print("AT+CMGF=1\r");
        delay(200);
        Serial.print("AT+CMGS=\"09166159067\"\r");
        delay(200);
        Serial.print("Red Alert! Water level is increase by " + String(waterPercent) + "% at your place. Current Temperature is " + String(temperature) + "C.");
        delay(200);
        Serial.write(26);
        delay(2000);
        statFlag = true;
      }
    }
    else
    {
      ctr = ctr + 1;
    }
  }
  else
  {
    if (distanceCm <= 10)
    {
      digitalWrite(pinLedRed, LOW);
      digitalWrite(pinLedOrange, HIGH);
      digitalWrite(pinLedGreen, LOW);

      float waterPercent = 10.0 - distanceCm;
      waterPercent = waterPercent / 10.0;
      waterPercent = waterPercent * 100;

      if (statFlag)
      {
        Serial.print("AT\r");
        updateSerial();
        delay(200);

        Serial.print("AT+CMGF=1\r");
        delay(200);
        Serial.print("AT+CMGS=\"09166159067\"\r");
        delay(200);
        Serial.print("Orange Alert! Water level is increase by " + String(waterPercent) + "% at your place. Current Temperature is " + String(temperature) + "C.");
        delay(200);
        Serial.write(26);
        delay(2000);
        statFlag = true;
      }
    }
    else
    {
      digitalWrite(pinLedRed, LOW);
      digitalWrite(pinLedOrange, LOW);
      digitalWrite(pinLedGreen, HIGH);
    }

    ctr = 0;
    digitalWrite(pinBuzzer, LOW);
  }

  // Display
  lcd.clear();
  lcd.setCursor(0, 0);        
  lcd.print("Water: " + String(distanceCm) + " cm"); 
  lcd.setCursor(0, 1);        
  lcd.print("Temp : " + String(temperature) + " c");        
  delay(200);
}
