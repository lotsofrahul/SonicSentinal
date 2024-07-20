#define BLYNK_TEMPLATE_ID "TMPL3FEEjvjSx"
#define BLYNK_TEMPLATE_NAME "Sound level meter"
#define BLYNK_AUTH_TOKEN "asDkR_xUDnT1Z-w2JAYSGHWhffM6GraU"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <WiFiClient.h> 



const int sampleWindow = 50;                              // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
 
#define SENSOR_PIN A0
#define LED D7 


char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Nadhey";  // type your wifi name
char pass[] = "12345678";  // type your wifi password


BlynkTimer timer;

void sendSensor(){
   digitalWrite(LED,LOW);
   unsigned long startMillis= millis();                   // Start of sample window
   float peakToPeak = 0;                                  // peak-to-peak level
 
   unsigned int signalMax = 0;                            //minimum value
   unsigned int signalMin = 1024;                         //maximum value
 
                                                          // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(SENSOR_PIN);                    //get reading from microphone
      if (sample < 1024)                                  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;                           // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;                           // save just the min levels
         }
      }
   }
 
   peakToPeak = signalMax - signalMin;                    // max - min = peak-peak amplitude
   //Serial.println(peakToPeak);
   int db ;             //calibrate for deciBels
   if (peakToPeak >= 0 && peakToPeak <= 3) {
    db = map(peakToPeak, 0, 3, 45, 50);
   }
   else if(peakToPeak >= 4 && peakToPeak <= 13){
    db = map(peakToPeak, 4, 13, 51, 62);
   }
   else if(peakToPeak >= 14 && peakToPeak <= 45){
    db = map(peakToPeak, 14, 45, 63, 87);

   }

   else{
    db = map(peakToPeak, 46, 200, 88, 110);
   }
    Serial.println(db);
     if(db>=85)
     {
      Serial.println("The sound level is above 85dB");
      digitalWrite(LED,HIGH);
      delay(2000);
      digitalWrite(LED,LOW);
     }
    
     
     
  Blynk.virtualWrite(V5, db);
    
    Serial.print("Sound level (dB) : ");
    Serial.println(db);


  if(db > 85){
   // Blynk.email("shameer50@gmail.com", "Alert", "Temperature over 28C!");
    Blynk.logEvent("high_sound_alert","The sound level is above 85dB");
  }
}

void setup(){
   
  Blynk.begin(auth, ssid, pass);
    timer.setInterval(2500L, sendSensor);
  pinMode (SENSOR_PIN, INPUT); // Set the signal pin as input  
   pinMode (LED, OUTPUT);  
  Serial.begin(9600);
   
  Serial.println("Connecting to ");
  Serial.println(ssid);
    
  WiFi.begin(ssid, pass);
  
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");
    
}

void loop(){
  Blynk.run();
  timer.run();
}