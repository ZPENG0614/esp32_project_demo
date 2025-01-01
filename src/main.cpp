#include <Arduino.h>
#include <ESPNOW.hpp>
#include <HXCthread.hpp>
#include <WebService.hpp>
#include "objects.hpp"
#include "class.hpp"
#include "task.hpp"

void setup() {
  esp_now_setup();//初始化espnow通信
  Serial.begin(115200);//初始化串口通信
  controler_data_send_thread.start("controler_data_send_thread",4096);
}

void loop() {
       
}

