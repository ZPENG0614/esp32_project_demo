/*
 * @LastEditors: qingmeijiupiao
 * @Description: 遥控器硬件封装类
 * @Author: qingmeijiupiao
 * @Date: 2024-11-07 15:41:37
 */

#ifndef CLASS_HPP
#define CLASS_HPP
#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include <functional>
#define analogprecision 12
Adafruit_ADS1115 ads1115;


class Rocker{
public:
    //X轴引脚,Y轴引脚,X轴方向(1,-1),Y轴方向(1,-1),死区(0-1)
    Rocker(Adafruit_ADS1115 *ADC,uint8_t x_chanel,uint8_t y_chanel,int8_t X_dir=1,int8_t Y_dir=1,float Dead_zone=0.02){
        x_dir = X_dir;
        y_dir = Y_dir;
        dead_zone = Dead_zone;
        _adcsensor = ADC;
        X_chanel = x_chanel;
        Y_chanel = y_chanel;
    }
    //更新和获取X轴值
    double get_x(){
        return X;
    }
    //更新和获取Y轴值
    double get_y(){
        return Y;
    }

    void calibration_center(){
        float X_center=0,Y_center=0;
        for(int i=0;i<10;i++){
            X_center+=_adcsensor->readADC_SingleEnded(X_chanel)*0.000125;
            Y_center+=_adcsensor->readADC_SingleEnded(Y_chanel)*0.000125;
            delay(10);
        }
        x_zero_point = X_center/10;
        y_zero_point = Y_center/10;
    }
    static void setup(){
        Wire.setPins(12,11);
        
        ads1115.begin(0x48,&Wire);
        ads1115.setDataRate(RATE_ADS1115_250SPS);
        ads1115.setGain(GAIN_ONE);
    }
    void callback(){
        float X_votage=_adcsensor->readADC_SingleEnded(X_chanel)*0.000125;
        float Y_votage=_adcsensor->readADC_SingleEnded(Y_chanel)*0.000125;
        float temp=x_dir*(X_votage-x_zero_point)/(2*x_zero_point);
        if(abs(temp)<dead_zone){
            temp=0;
        }
        X=temp;
        temp=y_dir*(Y_votage-y_zero_point)/(2*y_zero_point);
        if(abs(temp)<dead_zone){
            temp=0;
        }
        Y=temp;
    }
protected:

    double X,Y;
    uint8_t X_chanel,Y_chanel;
    float x_zero_point,y_zero_point;
    float dead_zone;
    int8_t x_dir,y_dir;
    Adafruit_ADS1115* _adcsensor;
};
//按键
class Button{
public:
    Button(uint8_t Pin,bool on_press_state=0){
        pin = Pin;
        press_state=on_press_state;
    }
    ~Button(){

    };
    bool get_state(){
        return state;
    }
        
    void setup(){
        pinMode(pin,INPUT_PULLUP);
        state = digitalRead(pin);
    }
    void add_press_callback(std::function<void(void)> press_callback){
        on_press_callback_function = press_callback;
    }
    void add_release_callback(std::function<void(void)> release_callback){
        on_release_callback_function = release_callback;
    }
    void add_change_callback(std::function<void(void)> change_callback){
        on_change_callback_function = change_callback;
    }
    void callback(){
        bool temp_state = digitalRead(pin);
        if(temp_state!=state){//有变化
            state = temp_state;
            if(temp_state==press_state){//按下
                if(on_press_callback_function!=nullptr){
                    on_press_callback_function();
                }
            }else{//释放
                if(on_release_callback_function!=nullptr){
                    on_release_callback_function();
                }
            }
        }      
    }
protected:

    std::function<void(void)> on_press_callback_function=nullptr;
    std::function<void(void)> on_release_callback_function=nullptr;
    std::function<void(void)> on_change_callback_function=nullptr;
    bool press_state;
    uint8_t pin;
    bool state;
};
//拨动开关
class Switch{
    public:
        Switch(uint8_t Pin){
            pinA = Pin;
        }
        void setup(){
            pinMode(pinA,INPUT_PULLDOWN);
        }
        void add_change_callback(std::function<void(void)> change_callback){
            on_change_callback_function = change_callback;
        }
        int get_state(){
            return state;
        }
        void callback(){
            int8_t temp_state = digitalRead(pinA);
            if(temp_state!=state){//有变化
                state = temp_state;
                if(on_change_callback_function!=nullptr){
                    on_change_callback_function();
                }
            }

        }
    protected:

        std::function<void(void)> on_change_callback_function=nullptr;
        int8_t state;
        uint8_t pinA;
};
//旋钮
class Knob{
    public:
        Knob(uint8_t Pin){
            pin = Pin;
        }
        void setup(){
            pinMode(pin,INPUT);
        }

        double get_value(){
            return value;
        }
        double callback(){
            value = analogRead(pin);
            value = (1<<analogprecision)-1-value;
            value = value/(1<<analogprecision);
            return value;
        }
    protected:

        double value;
        uint8_t pin;
        
};

#endif