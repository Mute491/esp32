

#include <Arduino.h>

#include "bypass.h"

#include "esp_wifi.h"

#include "ap.h"


#define AP_NAME "M#TE"
#define AP_PASSWD "tryhackme123456"

void setup() {
  Serial.begin(115200);
  apInit(AP_NAME, AP_PASSWD);

}

void loop() {
  apHandler();
  // put your main code here, to run repeatedly:

}
