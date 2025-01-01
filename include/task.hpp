#ifndef task_hpp
#define task_hpp

#include <ESPNOW.hpp>
#include <class.hpp>
#include <objects.hpp>

//遥控数据发送
//发送的数据类型
struct CONTROLLER_DATA_t {
public:
    float lx,ly,rx,ry,lknob,rknob;
    
    bool LUshouder,LDshouder,RUshouder,RDshouder;

    bool l1sw,l2sw,r1sw,r2sw;

    bool lleft,lright,lup,ldown;

    bool rleft,rright,rup,rdown;
    //编码数据到数组
    void data_to_array(uint8_t* arr){
        *(float*)arr=ly;
        *(float*)(arr+4)=lx;
        *(float*)(arr+8)=ry;
        *(float*)(arr+12)=rx;
        *(float*)(arr+16)=lknob;
        *(float*)(arr+20)=rknob;
        uint8_t UButtonbit=(LUshouder<<0)|(LDshouder<<1)|(RUshouder<<2)|(RDshouder<<3)|(l1sw<<4)|(l2sw<<5)|(r1sw<<6)|(r2sw<<7);
        arr[24]=UButtonbit;
        uint8_t DButtonbit=(lleft<<0)|(lright<<1)|(lup<<2)|(ldown<<3)|(rleft<<4)|(rright<<5)|(rup<<6)|(rdown<<7);
        arr[25]=DButtonbit;
    }
    //解码
    void array_to_data(uint8_t* arr){
        ly=*(float*)arr;
        lx=*(float*)(arr+4);
        ry=*(float*)(arr+8);
        rx=*(float*)(arr+12);
        lknob=*(float*)(arr+16);
        rknob=*(float*)(arr+20);

        uint8_t UButtonbit;
        UButtonbit=arr[24];
        LUshouder=UButtonbit&0x01;
        LDshouder=(UButtonbit&0x02)>>1;
        RUshouder=(UButtonbit&0x04)>>2;
        RDshouder=(UButtonbit&0x08)>>3;
        l1sw=(UButtonbit&0x10)>>4;
        l2sw=(UButtonbit&0x20)>>5;
        r1sw=(UButtonbit&0x40)>>6;
        r2sw=(UButtonbit&0x80)>>7;
        
        uint8_t DButtonbit;
        DButtonbit=arr[25];
        lleft=DButtonbit&0x01;
        lright=(DButtonbit&0x02)>>1;
        lup=(DButtonbit&0x04)>>2;
        ldown=(DButtonbit&0x08)>>3;
        rleft=(DButtonbit&0x10)>>4;
        rright=(DButtonbit&0x20)>>5;
        rup=(DButtonbit&0x40)>>6;
        rdown=(DButtonbit&0x80)>>7;
    }
};


static CONTROLLER_DATA_t send_data;//遥控数据对象

//遥控数据发送线程
HXC::thread<void> controler_data_send_thread([](){
    uint8_t data[26];//暂存数组
    constexpr int send_frenquency=50;//发送频率
    while (true){
        /*读取数据*/
        send_data.ly=leftjoy.get_y();
        send_data.lx=leftjoy.get_x();
        send_data.ry=rightjoy.get_y();
        send_data.rx=rightjoy.get_x();
        send_data.lknob=leftK.get_value();
        send_data.rknob=rightK.get_value();
        send_data.l1sw=L1.get_state();
        send_data.l2sw=L2.get_state();
        send_data.r1sw=R1.get_state();
        send_data.r2sw=R2.get_state();
        send_data.LUshouder=ShoulderLU.get_state();
        send_data.LDshouder=ShoulderLD.get_state();
        send_data.RUshouder=ShoulderRU.get_state();
        send_data.RDshouder=ShoulderRD.get_state();
        send_data.lleft=LLButton.get_state();
        send_data.lright=LRButton.get_state();
        send_data.lup=LUButton.get_state();
        send_data.ldown=LDButton.get_state();
        send_data.rleft=RLButton.get_state();
        send_data.rright=RRButton.get_state();
        send_data.rup=RUButton.get_state();
        send_data.rdown=RDButton.get_state();
        send_data.data_to_array(data);//数据编码
        /*发送*/
        esp_now_send_package("controller_data",data,sizeof(data),broadcastMacAddress);
        delay(1000/send_frenquency);//遥控数据发送频率
    }
});







#endif