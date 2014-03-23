#include <math.h>
#define DESIRED_LED_FLASH_ADDRESS 0x80000
const int LED_PIN_COMMON_1 = D0;
const int LED_PIN_RED_1 = A0;
const int LED_PIN_GREEN_1 = A1;
const int LED_PIN_BLUE_1 = A4;
const int LED_PIN_COMMON_2 = D1;
const int LED_PIN_RED_2 = A5;
const int LED_PIN_GREEN_2 = A6;
const int LED_PIN_BLUE_2 = A7;
String state = "active";
int global_r, global_g, global_b;
int bat_level = 0;

void setup() {
    pinMode(LED_PIN_COMMON_1, OUTPUT);
    pinMode(LED_PIN_RED_1, OUTPUT);
    pinMode(LED_PIN_GREEN_1, OUTPUT);
    pinMode(LED_PIN_BLUE_1, OUTPUT);
    pinMode(LED_PIN_COMMON_2, OUTPUT);
    pinMode(LED_PIN_RED_2, OUTPUT);
    pinMode(LED_PIN_GREEN_2, OUTPUT);
    pinMode(LED_PIN_BLUE_2, OUTPUT);
    pinMode(A2, INPUT_PULLUP);

    uint8_t temp_rgb[3];
    delay(20);
    sFLASH_ReadBuffer(temp_rgb, DESIRED_LED_FLASH_ADDRESS, 3);
    delay(20);
    global_r = temp_rgb[0];
    global_g = temp_rgb[1];
    global_b = temp_rgb[2];
    
    Spark.variable("bat_level", &bat_level, INT);
    Spark.function("fn_r", function_router);
    
    digitalWrite(LED_PIN_COMMON_1, HIGH);
    digitalWrite(LED_PIN_COMMON_2, HIGH);
}

void loop() {
    bat_level = (analogRead(A2) - 3200) / 4;
    if (state == "inactive") {
        delay(50);
    }
    else if (state == "active") {
        write_rgb_led(global_r, global_g, global_b);
        delay(20);
    }
    else if (state == "sleep") {
        Spark.sleep(60);
        delay(3600);
    }
}

int function_router(String command) {
    command.toUpperCase();
    if (command.startsWith("S_RGBL")) {
        return set_rgb_led(command.substring(7, 13));
    }
    else if (command.startsWith("RAINBOW")) {
        return rainbow();
    }
    else if (command.startsWith("T_ACT")) {
        return toggle_activation_state();
    }
    else if (command.startsWith("B_LED")) {
        return blink_LED(command.substring(6, 12), 
                         atoi(command.substring(13,17).c_str()), 
                         atoi(command.substring(18).c_str()));
    }
    else if(command.startsWith("SAVE_COLOR")){
        return save_color_to_mem();
    }
    return -1;
}

int rainbow() {
    if (state == "active"){
        int temp_r = global_r;
        int temp_g = global_g;
        int temp_b = global_b;
        for (int i = 0; i <=32; i++) {
            global_r = 255 - sin(.2 * i + .5) * 127 / 2.1 + 128;
            global_g = 255 - sin(.2 * i + 2.094395) * 127 / 1.2 + 128;
            global_b = 255 - sin(.2 * i + 4.187902) * 127 + 128;
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


int blink_LED(String blink_color, int rate, int iter) {
    if (state == "active"){
        int temp_r = global_r;
        int temp_g = global_g;
        int temp_b = global_b;
        for (int i = 0; i < iter && iter > 0; i++) {
            set_rgb_led(blink_color);
            write_rgb_led(global_r, global_g, global_b);
            digitalWrite(LED_PIN_COMMON_1, HIGH);
            digitalWrite(LED_PIN_COMMON_2, HIGH);
            delay(rate);
            digitalWrite(LED_PIN_COMMON_1, LOW);
            digitalWrite(LED_PIN_COMMON_2, LOW);
            write_rgb_led(0,0,0);
            delay(rate);
        }
        global_r = temp_r;
        global_g = temp_g;
        global_b = temp_b;
        digitalWrite(LED_PIN_COMMON_1, HIGH);
        digitalWrite(LED_PIN_COMMON_2, HIGH);
        return 1;
    }
    return -1;
}

int toggle_activation_state() {
    if (state == "inactive") {
        digitalWrite(LED_PIN_COMMON_1, HIGH);
        digitalWrite(LED_PIN_COMMON_2, HIGH);
        state = "active";
        return 1;
    }
    else if (state == "active") {
        digitalWrite(LED_PIN_COMMON_1, LOW);
        digitalWrite(LED_PIN_COMMON_2, LOW);
        state = "inactive";
        return 0;
    }
    else {
        return -1;
    }#include <math.h>
#define DESIRED_LED_FLASH_ADDRESS 0x80000
const int LED_PIN_COMMON_1 = D0;
const int LED_PIN_RED_1 = A0;
const int LED_PIN_GREEN_1 = A1;
const int LED_PIN_BLUE_1 = A4;
const int LED_PIN_COMMON_2 = D1;
const int LED_PIN_RED_2 = A5;
const int LED_PIN_GREEN_2 = A6;
const int LED_PIN_BLUE_2 = A7;
String state = "active";
int global_r, global_g, global_b;
int bat_level = 0;

void setup() {
    pinMode(LED_PIN_COMMON_1, OUTPUT);
    pinMode(LED_PIN_RED_1, OUTPUT);
    pinMode(LED_PIN_GREEN_1, OUTPUT);
    pinMode(LED_PIN_BLUE_1, OUTPUT);
    pinMode(LED_PIN_COMMON_2, OUTPUT);
    pinMode(LED_PIN_RED_2, OUTPUT);
    pinMode(LED_PIN_GREEN_2, OUTPUT);
    pinMode(LED_PIN_BLUE_2, OUTPUT);
    pinMode(A2, INPUT_PULLUP);

    uint8_t temp_rgb[3];
    delay(20);
    sFLASH_ReadBuffer(temp_rgb, DESIRED_LED_FLASH_ADDRESS, 3);
    delay(20);
    global_r = temp_rgb[0];
    global_g = temp_rgb[1];
    global_b = temp_rgb[2];
    
    Spark.variable("bat_level", &bat_level, INT);
    Spark.function("fn_r", function_router);
    
    digitalWrite(LED_PIN_COMMON_1, HIGH);
    digitalWrite(LED_PIN_COMMON_2, HIGH);
}

void loop() {
    bat_level = (analogRead(A2) - 3200) / 4;
    if (state == "inactive") {
        delay(50);
    }
    else if (state == "active") {
        write_rgb_led(global_r, global_g, global_b);
        delay(20);
    }
    else if (state == "sleep") {
        Spark.sleep(60);
        delay(3600);
    }
}

int function_router(String command) {
    command.toUpperCase();
    if (command.startsWith("S_RGBL")) {
        return set_rgb_led(command.substring(7, 13));
    }
    else if (command.startsWith("RAINBOW")) {
        return rainbow();
    }
    else if (command.startsWith("T_ACT")) {
        return toggle_activation_state();
    }
    else if (command.startsWith("B_LED")) {
        return blink_LED(command.substring(6, 12), 
                         atoi(command.substring(13,17).c_str()), 
                         atoi(command.substring(18).c_str()));
    }
    else if(command.startsWith("SAVE_COLOR")){
        return save_color_to_mem();
    }
    return -1;
}

int rainbow() {
    if (state == "active"){
        int temp_r = global_r;
        int temp_g = global_g;
        int temp_b = global_b;
        for (int i = 0; i <=32; i++) {
            global_r = 255 - sin(.2 * i + .5) * 127 / 2.1 + 128;
            global_g = 255 - sin(.2 * i + 2.094395) * 127 / 1.2 + 128;
            global_b = 255 - sin(.2 * i + 4.187902) * 127 + 128;
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


int blink_LED(String blink_color, int rate, int iter) {
    if (state == "active"){
        int temp_r = global_r;
        int temp_g = global_g;
        int temp_b = global_b;
        for (int i = 0; i < iter && iter > 0; i++) {
            set_rgb_led(blink_color);
            write_rgb_led(global_r, global_g, global_b);
            digitalWrite(LED_PIN_COMMON_1, HIGH);
            digitalWrite(LED_PIN_COMMON_2, HIGH);
            delay(rate);
            digitalWrite(LED_PIN_COMMON_1, LOW);
            digitalWrite(LED_PIN_COMMON_2, LOW);
            write_rgb_led(0,0,0);
            delay(rate);
        }
        global_r = temp_r;
        global_g = temp_g;
        global_b = temp_b;
        digitalWrite(LED_PIN_COMMON_1, HIGH);
        digitalWrite(LED_PIN_COMMON_2, HIGH);
        return 1;
    }
    return -1;
}

int toggle_activation_state() {
    if (state == "inactive") {
        digitalWrite(LED_PIN_COMMON_1, HIGH);
        digitalWrite(LED_PIN_COMMON_2, HIGH);
        state = "active";
        return 1;
    }
    else if (state == "active") {
        digitalWrite(LED_PIN_COMMON_1, LOW);
        digitalWrite(LED_PIN_COMMON_2, LOW);
        state = "inactive";
        return 0;
    }
    else {
        return -1;
    }
}

int save_color_to_mem(){
    delay(20);
    sFLASH_EraseSector(DESIRED_LED_FLASH_ADDRESS);
    delay(20);
    uint8_t values_led[3] = { (uint8_t)global_r, (uint8_t)global_g, 
                              (uint8_t)global_b };
    sFLASH_WriteBuffer(values_led, DESIRED_LED_FLASH_ADDRESS, 3);
    return 1;
}

int set_rgb_led(String command) {
    if (state == "active") {
        int r_int = strtoul(command.substring(0,2).c_str(), NULL, 16);
        int g_int = strtoul(command.substring(2,4).c_str(), NULL, 16);
        int b_int = strtoul(command.substring(4,6).c_str(), NULL, 16);
        global_r = 255 - r_int / 1.9;
        global_g = 255 - g_int / 1.2;
        global_b = 255 - b_int;
        
        return 1;
    }
    else {
        return 0;
    }
}

void write_rgb_led(int red, int green, int blue) {
    analogWrite(LED_PIN_RED_1, red);
    analogWrite(LED_PIN_GREEN_1, green);
    analogWrite(LED_PIN_BLUE_1, blue);
    analogWrite(LED_PIN_RED_2, red);
    analogWrite(LED_PIN_GREEN_2, green);
    analogWrite(LED_PIN_BLUE_2, blue);
}

}

int save_color_to_mem(){
    delay(20);
    sFLASH_EraseSector(DESIRED_LED_FLASH_ADDRESS);
    delay(20);
    uint8_t values_led[3] = { (uint8_t)global_r, (uint8_t)global_g, 
                              (uint8_t)global_b };
    sFLASH_WriteBuffer(values_led, DESIRED_LED_FLASH_ADDRESS, 3);
    return 1;
}

int set_rgb_led(String command) {
    if (state == "active") {
        int r_int = strtoul(command.substring(0,2).c_str(), NULL, 16);
        int g_int = strtoul(command.substring(2,4).c_str(), NULL, 16);
        int b_int = strtoul(command.substring(4,6).c_str(), NULL, 16);
        global_r = 255 - r_int / 1.9;
        global_g = 255 - g_int / 1.2;
        global_b = 255 - b_int;
        
        return 1;
    }
    else {
        return 0;
    }
}

void write_rgb_led(int red, int green, int blue) {
    analogWrite(LED_PIN_RED_1, red);
    analogWrite(LED_PIN_GREEN_1, green);
    analogWrite(LED_PIN_BLUE_1, blue);
    analogWrite(LED_PIN_RED_2, red);
    analogWrite(LED_PIN_GREEN_2, green);
    analogWrite(LED_PIN_BLUE_2, blue);
}
