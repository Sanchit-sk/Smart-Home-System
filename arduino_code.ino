#include<SoftwareSerial.h>
#include<LiquidCrystal.h>

LiquidCrystal lcd(12,11,8,7,6,5);
SoftwareSerial Transmission(3,2); //Tx,Rx
const int temprature_pin= A2;
const int LED=13;
const int LDR=A0;
const int PIR=10;
const int motor=4;
const int sunpin=A1;
const int control_output=9;

int LDR_digital,minlight = 2,pir_status,LM35_digital,temprature,sunlight, h = 9, m = 20, s = 0, flag = 21 ,esp_data,arduino_data,LED_data,motor_data,pre_data=0;
unsigned int millisec=0,my_millisec=0;   //requires large range

void LED_control();
void motor_control();
void LEDarr_control();
void time_calculate();
void lcd_display();
void action_display();
void wifi();

void setup() {
    Transmission.begin(115200);  //WiFi communication setup
    lcd.begin(20,4);
    pinMode(LED,OUTPUT);
    pinMode(PIR,INPUT);
    pinMode(motor,OUTPUT);
    Serial.begin(115200);    //serial communication setup
}

void loop() {
LDR_digital=analogRead(LDR);  //gives a digital value in the range(0,1023);
pir_status=digitalRead(PIR);
temprature=analogRead(temprature_pin);
//wifi(); 
LED_control();  
motor_control();
LEDarr_control();
time_calculate();
lcd_display();
action_display();
LED_data= digitalRead(LED);
motor_data=digitalRead(motor);
arduino_data=LED_data*3+motor_data*5;    //Data transmitted to Node MCU at the end of the code
Transmission.write(arduino_data);
delay(3000);
}

void LED_control()
{ 
Serial.print("Digital Value(LDR)= ");
Serial.println(LDR_digital);                               //Once the motion is detected, the led will stay on until the user command to turn it off 
if(pir_status==HIGH&&LDR_digital<=minlight)         
   { digitalWrite(LED,HIGH);
   }        
}

void motor_control()
{ 
  Serial.print("Temp - ");  
  Serial.println(temprature);                             //Once the motion is detected, the motor will stay on untill the user command to turn it off
  if(temprature>=20&&pir_status==HIGH)
  { digitalWrite(motor,HIGH);
  }
}

void LEDarr_control()
{ int minvalue=75;
   sunlight=analogRead(sunpin);
  Serial.print("sunlight= ");
  Serial.println(sunlight);
  if(sunlight<minvalue)
  { digitalWrite(control_output,LOW);
  }
  if(sunlight>minvalue)
  {digitalWrite(control_output,HIGH);
  }
}

void time_calculate()
{  s=s+1;
     if (s >= 60)
  {
    s = s-60;
    m = m + 1;
  }
  if (m == 60)
  {
    m = 0;
    h = h + 1;
    flag = flag + 1;
  }
  if (h == 13)
  {
    h = 1;
  } 
}

void lcd_display()
{ lcd.setCursor(0,0); 
  lcd.print("TIME:" );
  lcd.print(h);
  lcd.print(":");
  lcd.print(m);
  if (flag < 12) lcd.print(" AM");     //time and temprature display
  if (flag == 12) lcd.print(" PM");
  if (flag > 12) lcd.print(" PM");
  if (flag == 24) flag = 0;
  lcd.print(" TEMP:");
  lcd.print(temprature);

  lcd.setCursor(0,1);
  if(LED_data==HIGH&&motor_data==HIGH)
  {lcd.print("LED:ON  MOTOR:ON");
  }
  if(LED_data==HIGH&&motor_data==LOW)
  {lcd.print("LED:ON  MOTOR:OFF");
  }
  if(LED_data==LOW&&motor_data==HIGH)
  {lcd.print("LED:OFF  MOTOR:ON");     //LED and motor status display
  }
  if(LED_data==LOW&&motor_data==LOW)
  {lcd.print("LED:OFF  MOTOR:OFF");
  }
  lcd.setCursor(0,2);
  lcd.print("LAST ACTION:-");
}

void action_display()
{
  if(esp_data-pre_data!=0)
{ if(esp_data-pre_data>0)    //something was turned on
 {
  if(pir_status==LOW||LDR_digital>minlight)
  { if(esp_data-pre_data>=2&&esp_data-pre_data<=4)
    { lcd.setCursor(0,3);
      lcd.print("TURNED LED ON");
    }
  }  
   if(pir_status==LOW||temprature<20)
  { if(esp_data-pre_data>=4&&esp_data-pre_data<=6)
    { lcd.setCursor(0,3);
      lcd.print("TURNED MOTOR ON");
    }
  }
   if(pir_status==LOW||temprature<20&&pir_status==LOW||LDR_digital>minlight)
  { if(esp_data-pre_data>=6)
    { lcd.setCursor(0,3);
      lcd.print("TURNED LED & MOTOR ON");
    }
  }
 }
  
 if(esp_data-pre_data<0)    //something was turned off
 {
  if(pir_status==HIGH&&LDR_digital<=minlight)
  { if(esp_data-pre_data>=-4&&esp_data-pre_data<=-2)
    { lcd.setCursor(0,3);
      lcd.print("TURNED LED OFF");
    }
  }  
   if(pir_status==HIGH&&temprature>20)
  { if(esp_data-pre_data>=-6&&esp_data-pre_data<=-4)
    { lcd.setCursor(0,3);
      lcd.print("TURNED MOTOR OFF");
    }
  }
   if(pir_status==HIGH&&temprature>20&&LDR_digital<=minlight)
  { if(esp_data-pre_data<=-6)
    { lcd.setCursor(0,3);
      lcd.print("TURNED LED & MOTOR OFF");
    }
  }
}     
}
}
void wifi()
{ pre_data= esp_data;
  esp_data = Transmission.read();
  if(esp_data >= 7)
  { 
    digitalWrite(LED,HIGH);
    digitalWrite(motor,HIGH);
  }
  else{
    if(esp_data >= 4)
    {
      digitalWrite(LED,LOW);
      digitalWrite(motor,HIGH);
    }
    else{
      if(esp_data >= 2)
      {
       digitalWrite(LED,HIGH);
      digitalWrite(motor,LOW);
      }
    }
}
}
