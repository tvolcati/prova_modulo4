#include <WiFi.h>
#include <HTTPClient.h>

#define led_yellow 9 // Pin used to control yellow led
#define led_green 2 // Pin used to control green led
#define led_red 40 // Pin used to control red led

uint8_t push_count = 0;

const int buttonPin = 18;  // the number of the pushbutton pin
bool buttonState_bool = false; // variable for reading the pushbutton status

const int ldrPin = 4;  // the number of the pushbutton pin
int threshold=600;
int actualMillis = 0;
int lastPointYellow = 0;
unsigned long last_stop = 0;
unsigned long last_button_press = 0;
uint8_t state = 0;

void setup() {

  // Configure as OUTPUT ports of ESP32's leds that are being used
  pinMode(led_yellow,OUTPUT);
  pinMode(led_green,OUTPUT);
  pinMode(led_red,OUTPUT);

  // Inicialize ports of INPUT
  pinMode(buttonPin, INPUT); // Initialize the pushbutton pin as an input
  pinMode(ldrPin, INPUT); // Initialize LDR

  digitalWrite(led_yellow, LOW);
  digitalWrite(led_green, LOW);
  digitalWrite(led_red, LOW);

  Serial.begin(9600); // Configure bound rate as 9600
}

void loop() {
  Serial.println(push_count);
  int ldrstatus=analogRead(ldrPin);
  actualMillis = millis();
  bool buttonState = digitalRead(buttonPin);

  if(ldrstatus<=threshold){
    Serial.print("its dark turn on led");
    Serial.println(ldrstatus);
    if (actualMillis - lastPointYellow >= 1000){
    digitalWrite(led_yellow, HIGH);
    lastPointYellow = actualMillis;
    }else{
      digitalWrite(led_yellow, LOW);
    };
  }else{
    Serial.print("its bright turn off light");
    Serial.println(ldrstatus);


    switch (state){
    case 0:
      if (actualMillis - last_stop >= 3000){ //verify if has already passed 6 seconds
        last_stop = actualMillis; // capture last stop
        state = 1; //switch to next state
        digitalWrite(led_green, LOW); // turn down green led
      } else{
        digitalWrite(led_green, HIGH); //while 6 seconds do not pass, green led is HIGH
      }
      break;

    case 1:
      if (actualMillis - last_stop >= 2000){
        last_stop = actualMillis;
        state = 2;
        digitalWrite(led_yellow, LOW);
      }else{
        digitalWrite(led_yellow, HIGH);
      }
      break;

    case 2:
      if (actualMillis - last_stop >= 5000){
        last_stop = actualMillis;
        state = 0; // return to first state, creating a loop
        digitalWrite(led_red, LOW);
      }else{
        if(buttonState == HIGH){
         delay(200);
         push_count+=1;
         buttonState_bool = true;
         last_button_press = actualMillis;
          if(push_count >= 3){
              WiFi.begin("Wokwi-GUEST", "", 6); // Connect Wifi with "Wokwi-GUEST" SSID

              while (WiFi.status() != WL_CONNECTED) {
                delay(100);
                Serial.print(".");
              }
              Serial.println("Connected succesfully to Wifi"); // When wifi is not "not connectes" anymore, this message is gonna be printed to show wifi is ok


              if(WiFi.status() == WL_CONNECTED){ // If ESP32 is connected with internet
                HTTPClient http;

                String serverPath = "http://www.google.com.br/"; // Endpoint of HTTP request

                http.begin(serverPath.c_str());

                int httpResponseCode = http.GET(); // print result of request

                if (httpResponseCode>0) {
                  Serial.print("HTTP Response code: ");
                  Serial.println(httpResponseCode);
                  String payload = http.getString();
                  Serial.println(payload);
                  }
                else {
                  Serial.print("Error code: ");
                  Serial.println(httpResponseCode);
                  }
                  http.end();
                  push_count = 0;
                }

              else {
                Serial.println("WiFi Disconnected");
              }
          }
         }
         if(buttonState_bool){
          if(actualMillis - last_button_press >= 1000){
            last_stop = actualMillis;
            state = 0;
            digitalWrite(led_red, LOW);
            buttonState_bool = false;
          }
        }else{
        digitalWrite(led_red, HIGH);
        }
      }
    break;
  }

}
}