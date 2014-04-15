#include "helio_functions.h"
#define DESIRED_LED_FLASH_ADDRESS 0x80000

Functions::Functions(int t){ //setup
    LED_PIN_COMMON_1 = D0;
    LED_PIN_RED_1 = A0;
    LED_PIN_GREEN_1 = A1;
    LED_PIN_BLUE_1 = A4;
    LED_PIN_COMMON_2 = D2;
    LED_PIN_RED_2 = A5;
    LED_PIN_GREEN_2 = A6;
    LED_PIN_BLUE_2 = A7;  
    state = "active";
    func_state = -2; //-2=first run, -1=inactive 0=nothing, 1=rainbow, 2=breathe, 3=bouncy, 4=charging
    pinMode(LED_PIN_COMMON_1, OUTPUT);
    pinMode(LED_PIN_RED_1, OUTPUT);
    pinMode(LED_PIN_GREEN_1, OUTPUT);
    pinMode(LED_PIN_BLUE_1, OUTPUT);
    pinMode(LED_PIN_COMMON_2, OUTPUT);
    pinMode(LED_PIN_RED_2, OUTPUT);
    pinMode(LED_PIN_GREEN_2, OUTPUT);
    pinMode(LED_PIN_BLUE_2, OUTPUT);
    pinMode(A2, INPUT);
    
    uint8_t temp_rgb[3];
    delay(20);
    sFLASH_ReadBuffer(temp_rgb, DESIRED_LED_FLASH_ADDRESS, 3);
    delay(20);
    global_r = temp_rgb[0];
    global_g = temp_rgb[1];
    global_b = temp_rgb[2];
    //RGB.control(true);
    write_rgb_led(global_r, global_g, global_b);
    digitalWrite(LED_PIN_COMMON_1, HIGH);
    digitalWrite(LED_PIN_COMMON_2, HIGH);
}

void Functions::set_func_state(int fs){
    func_state = fs;
}

int Functions::get_func_state(){
    return func_state;
}

void Functions::set_glow_color(String gc){
    glow_color = gc;
}

String Functions::get_glow_color(){
    return glow_color;
}

int Functions::get_global_r(){
    return global_r;
}

int Functions::get_global_g(){
    return global_g;
}

int Functions::get_global_b(){
    return global_b;
}

int Functions::set_rgb_led(String command) {
    func_state = 0;
    int r_int = strtoul(command.substring(0,2).c_str(), NULL, 16);
    int g_int = strtoul(command.substring(2,4).c_str(), NULL, 16);
    int b_int = strtoul(command.substring(4,6).c_str(), NULL, 16);
    global_r = 255 - r_int / 2.0;
    global_g = 255 - g_int;
    global_b = 255 - b_int;
    return 1;
    
}

void Functions::write_rgb_led(int red, int green, int blue) {
    analogWrite(LED_PIN_RED_1, red);
    analogWrite(LED_PIN_GREEN_1, green);
    analogWrite(LED_PIN_BLUE_1, blue);
    analogWrite(LED_PIN_RED_2, red);
    analogWrite(LED_PIN_GREEN_2, green);
    analogWrite(LED_PIN_BLUE_2, blue);
    RGB.color(255-global_r, 255-global_g, 255-global_b);
}

int Functions::save_color_to_mem(){
    delay(20);
    sFLASH_EraseSector(DESIRED_LED_FLASH_ADDRESS);
    delay(20);
    uint8_t values_led[3] = { (uint8_t)global_r, (uint8_t)global_g, 
                              (uint8_t)global_b };
    sFLASH_WriteBuffer(values_led, DESIRED_LED_FLASH_ADDRESS, 3);
    return 1;
}

int Functions::rainbow_fun() {
    if (state == "active"){
        int temp_r = global_r;
        int temp_g = global_g;
        int temp_b = global_b;
        for (int i = 0; i <=128; ++i) {
            if(func_state != 1){
                global_r = temp_r;
                global_g = temp_g;
                global_b = temp_b;
                write_rgb_led(global_r, global_g, global_b);
                break;
            }
            global_r = 255 - sin(.05 * i + .5) * 127 / 2 + 128;
            global_g = 255 - sin(.05 * i + 2.094395) * 127 + 128;
            global_b = 255 - sin(.05 * i + 4.187902) * 127 + 128;
            write_rgb_led(global_r, global_g, global_b);
            delay(50);
        } 
        global_r = temp_r;
        global_g = temp_g;
        global_b = temp_b;
        return 1;
    }
        
    return -1;
}

int Functions::blink_LED(String blink_color, int rate, int iter) {
    if (state == "active"){
        int temp_r = global_r;
        int temp_g = global_g;
        int temp_b = global_b;
        for (int i = 0; i < iter && iter > 0; ++i) {
            if(state == "inactive"){
                global_r = temp_r;
                global_g = temp_g;
                global_b = temp_b;
                write_rgb_led(global_r, global_g, global_b);
                break;
            }
            color_ratio(blink_color);
            write_rgb_led(global_r, global_g, global_b);
            digitalWrite(LED_PIN_COMMON_1, HIGH);
            digitalWrite(LED_PIN_COMMON_2, HIGH);
            delay(rate);
            digitalWrite(LED_PIN_COMMON_1, LOW);
            digitalWrite(LED_PIN_COMMON_2, LOW);
            write_rgb_led(0,0,0);
            RGB.color(0,0,0);
            delay(rate);
        }
        global_r = temp_r;
        global_g = temp_g;
        global_b = temp_b;
        digitalWrite(LED_PIN_COMMON_1, HIGH);
        digitalWrite(LED_PIN_COMMON_2, HIGH);
        write_rgb_led(global_r, global_g, global_b);
        return 1;
    }
    return -1;
}

int Functions::glow_LED(String command) {
    if (state == "active"){
        glow_color = command;
        int temp_r = global_r;
        int temp_g = global_g;
        int temp_b = global_b;
        int r_int = strtoul(command.substring(0,2).c_str(), NULL, 16);
        int g_int = strtoul(command.substring(2,4).c_str(), NULL, 16);
        int b_int = strtoul(command.substring(4,6).c_str(), NULL, 16);
        int x = 1;
        for (int i = 0; i > -1; i = i + x){
            if(func_state != 2){
                    global_r = temp_r;
                    global_g = temp_g;
                    global_b = temp_b;
                    write_rgb_led(global_r, global_g, global_b);
                    break;
            }
                global_r = 255 - (i * r_int / 255) / 2;
                global_g = 255 - (i * g_int / 255);
                global_b = 255 - (i * b_int / 255);
            write_rgb_led(global_r, global_g, global_b);
            if (i == 250) x = -1;// switch direction at peak
             delay(10);
        }
        global_r = temp_r;
        global_g = temp_g;
        global_b = temp_b;
        return 1;
    }
    return -1;
}

int Functions::bouncy(){
    if (state == "active"){
        int temp_r = global_r;
        int temp_g = global_g;
        int temp_b = global_b;
        for (int i = 0; i <=32; ++i){
            if(func_state != 3){
                global_r = temp_r;
                global_g = temp_g;
                global_b = temp_b;
                write_rgb_led(global_r, global_g, global_b);
                break;
            }
            int r = rand() % 100;
            int g = rand() % 100;
            int b = rand() % 100;
            int t = rand() % 100;
            global_r = 255 - (r + 135) /2;
            global_g = 255 - (g + 40);
            global_b = 255 - (b + 0);
            write_rgb_led(global_r, global_g, global_b);
            delay(t);
            return 1;
        }
    }
    return -1;
}

void Functions::charge(){
    if (state == "active"){
        int temp_r = global_r;
        int temp_g = global_g;
        int temp_b = global_b;
        int x = 1;
        for (int i = 0; i > -1; i = i + x){
            if(func_state != 4){
                global_r = temp_r;
                global_g = temp_g;
                global_b = temp_b;
                break;
            }
            global_r = 255 - i /2;
            global_g = 255;
            global_b = 255;
            write_rgb_led(global_r, global_g, global_b);
            if (i == 250) x = -1;// switch direction at peak
             delay(10);
        }
        Spark.sleep(SLEEP_MODE_DEEP, 360);
    }
}

int Functions::toggle_activation_state() {
    if (state == "inactive") {
        digitalWrite(LED_PIN_COMMON_1, HIGH);
        digitalWrite(LED_PIN_COMMON_2, HIGH);
        RGB.control(true);
        func_state = 0;
        state = "active";
        return 1;
    }
    else if (state == "active") {
        digitalWrite(LED_PIN_COMMON_1, LOW);
        digitalWrite(LED_PIN_COMMON_2, LOW);
        RGB.control(false);
        func_state = -1;
        state = "inactive";
        return 0;
    }
    else {
        return -1;
    }
}

int Functions::battery_reader(){
    int bat_temp = map(analogRead(A2), 2900, 3490, 0, 135);
    if(bat_temp > 100) bat_temp = 100;
    if(bat_temp < 0) bat_temp = 0;
    return bat_temp;
}

void Functions::color_ratio(String color){
    global_r = 255 - strtoul(color.substring(0,2).c_str(), NULL, 16) / 2.0;
    global_g = 255 - strtoul(color.substring(2,4).c_str(), NULL, 16);
    global_b = 255 - strtoul(color.substring(4,6).c_str(), NULL, 16);
    // global_r = 255 - r_int / 2.0;
    // global_g = 255 - g_int;
    // global_b = 255 - b_int;
}
