#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

SoftwareSerial mySerial(D2, D3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

char auth[] = "vy_I2z9N-rKUBdsF_eVQbP3CkbY-BZm0"; //auth code to send notification to the smartphone
char ssid[] = "iAmGroot";
char pass[] = "123456789";
String person;
void setup()
{
  Blynk.begin(auth, ssid, pass);
  Serial.begin(9600);
  while (!Serial);  
  delay(100);
  Serial.println("fingertest");
  pinMode(D7, OUTPUT);
  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
}

void loop()                  
{
  Blynk.run();
  getFingerprintIDez();
  delay(50);            
  digitalWrite(D7, LOW);
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
  {
  delay(3000);
  Serial.print("Not Found"); 
  Serial.print("Error"); 
  return finger.fingerID;
 }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;
  
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1; 
  
  // found a match!
  {
    digitalWrite(D7, HIGH);
    delay(3000);
    digitalWrite(D7, LOW);
    if(finger.fingerID == 1){
       person = "Bibek";
      }
    else if(finger.fingerID == 50){
      person = "Pukar";
      }
    Blynk.notify(person + " just entered your house");
    Serial.print("Found ID #"); Serial.print(finger.fingerID);
    Serial.print(" with confidence of "); Serial.println(finger.confidence);
    }
   }
