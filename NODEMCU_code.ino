#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
SoftwareSerial Transmission(12,13); //Tx,Rx D6,D7
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "mjRse02omAZEW_P_jJ15sTtstjDIvMj1";

char auth[]=" uXZli_av7_axMISZHD6saYUeP8GBC_6i";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] ="SK";
char pass[] = "11111111";
int LED,MOTOR,DATA,arduino_data;
void setup()
{
  
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  Transmission.begin(115200);
}

void loop()
{
  delay(500);
  Blynk.run();
  LED=digitalRead(4); //D2 GP4
  MOTOR=digitalRead(5); //D1  GP5
  Serial.print(LED);
  Serial.print(MOTOR);
  DATA=LED*3+MOTOR*5;
  Serial.print(DATA);
  Transmission.write(DATA);
 /* arduino_data= Transmission.read();
  if(arduino_data>=7);
  { digitalWrite(4,HIGH);
    digitalWrite(5,HIGH);
  }
   if(arduino_data>=4);
  { digitalWrite(4,LOW);
    digitalWrite(5,HIGH);
  }
   if(arduino_data>=2);
  { digitalWrite(4,HIGH);
    digitalWrite(5,LOW);
  }
   if(arduino_data>=0);
  { digitalWrite(4,LOW);
    digitalWrite(5,LOW);
  }*/
}
