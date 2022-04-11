#include <DHT.h>  // Including library for dht11

#include <ESP8266WiFi.h>
 
String apiKey = "Y2RC4PREWRUL4SCR";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "ICE_Innovation_Lab";     //  wifi ssid 
const char *pass =  "beinnovative#";    // wifi password
const char* server = "api.thingspeak.com";

#define DHTPIN 0         //pin where the dht11 is connected
#define rainPin D1
#define buzzPin D2
#define soilMoisture A0
 
DHT dht(DHTPIN, DHT11);

WiFiClient client;
 
void setup() 
{
       Serial.begin(115200);
       delay(10);
       dht.begin();

       pinMode(rainPin,INPUT);
       pinMode(buzzPin,OUTPUT);
       pinMode(soilMoisture, INPUT);
 
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
 
void loop() 
{     
      int rainValue = digitalRead(rainPin);
      Serial.println(rainValue);

      if(rainValue == 0){
        digitalWrite(buzzPin,HIGH);
      }

      else{
        digitalWrite(buzzPin,LOW);
      }

      int soilValue = analogRead(soilMoisture);
      
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      
              if (isnan(h) || isnan(t) || isnan(rainValue) || isnan(soilValue)) 
                 {
                     Serial.println("Failed to read from sensors!");
                      return;
                 }

                         if (client.connect(server,80))   // establishing the connection with thingspeak server
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr +="&field3=";
                             postStr += String(rainValue);
                             postStr +="&field4=";
                             postStr += String(soilValue);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.println(t);
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.println(h);
                             Serial.print("Rain state: ");
                             Serial.println(rainValue);
                             Serial.print("Soil Moisture: ");
                             Serial.println(soilValue);
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  
  delay(1000);  // dealy half second and regenerate the temperature and send it to the thingspeak
}
