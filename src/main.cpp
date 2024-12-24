#include <Arduino.h>
#include <ESPNOW.hpp>
#include <HXCthread.hpp>
#include <WebService.hpp>


void setup() {
  esp_now_setup();//初始化espnow通信
  Serial.begin(115200);//初始化串口通信
  esp32_ap_setup();
  esp32_web_create();
}

void loop() {
       


}

