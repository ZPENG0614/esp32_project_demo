/*
 * @LastEditors: qingmeijiupiao
 * @Description: 对象文件
 * @Author: qingmeijiupiao
 * @Date: 2024-11-07 22:52:07
 */

#ifndef OBJECTS_HPP
#define OBJECTS_HPP
#include"class.hpp"
#include"ESPNOW.hpp"
#include <U8g2lib.h>
#include <Wire.h>

//OLED对象
U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 21, /* cs=*/ 45, /* dc=*/ 48, /* reset=*/ 47);


//摇杆
Rocker leftjoy(&ads1115,2,3,-1,-1,0.02);
Rocker rightjoy(&ads1115,0,1,1,1,0.02);

//旋钮
Knob leftK(4);
Knob rightK(5);

//拨杆开关
Switch R1(2);
Switch R2(41);
Switch L1(16);
Switch L2(7);
//肩键
Button ShoulderLU(15,HIGH);
Button ShoulderLD(6,HIGH);
Button ShoulderRU(42,HIGH);
Button ShoulderRD(1,HIGH);

//左下4个按键
Button LUButton(18);
Button LDButton(9);
Button LLButton(17);
Button LRButton(8);
//右下4个按键
Button RUButton(39);
Button RDButton(13);
Button RLButton(38);
Button RRButton(40);
//BOOT按钮
Button BOOTButton(0);


#endif // OBJECTS_HPP