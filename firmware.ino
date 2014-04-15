#include "helio_functions.h"
#include <math.h>
int bat_level = 0;
int func_state = 0;
int bat_iter = 0;
int state_temp;
int bat_array[4] = {0,0,0,0};
Functions helio(0);

void setup() {
    Spark.variable("bat_level", &bat_level, INT);
    Spark.function("fn_r", function_router);
    
}

void loop() {
    func_state = helio.get_func_state();
    bat_array[bat_iter] = helio.battery_reader();
    if(bat_iter == 3) bat_iter = 0;
    else bat_iter++;
    bat_level = (bat_array[0] + bat_array[1] + bat_array[2] + bat_array[3]) / 4;
    
    switch(func_state){
        case-2: //firstrun
            if(!Spark.connected){
                RGB.control(false);
            }
            else{
                RGB.control(true);
                helio.set_func_state(0);
            }
        case -1: //inactive
            delay(50);
            break;
        case 0: //active hold color
            delay(20);
            break;
        case 1: //rainbow
            helio.rainbow_fun();
            delay(5);
            break;
        case 2: //Breathe
            helio.glow_LED(helio.get_glow_color());
            delay(20);
            break;
        case 3: //Bouncy
            helio.bouncy();
            delay(10);
            break;
        case 4: //Charging
            helio.charge();
            delay(10);
        case 10:
            Spark.sleep(60);
            delay(3600);
            break;
        }
}

int function_router(String command) {
    command.toUpperCase();
    
    if (command.startsWith("S_RGBL")) {
        helio.set_func_state(0);
        helio.set_rgb_led(command.substring(7, 13));
        helio.write_rgb_led(helio.get_global_r(), helio.get_global_g(), helio.get_global_b());
        return 1;
    }
    else if (command.startsWith("SAVE_COLOR")){
        return helio.save_color_to_mem();
    }
    else if (command.startsWith("RAINBOW")) {
        if(helio.get_func_state() == 1){
            helio.set_func_state(0);
            return 0;
        }
        else {
            helio.set_func_state(1);
            return 1;
        }
    }
    else if (command.startsWith("B_LED")) {
        return helio.blink_LED(command.substring(6, 12), 
                         atoi(command.substring(13,17).c_str()), 
                         atoi(command.substring(18).c_str()));
    }
    else if (command.startsWith("G_LED")) {
        if(helio.get_func_state() == 2 && (command.substring(6,12) != helio.get_glow_color())){
            helio.set_glow_color(command.substring(6,12).c_str()); 
            helio.set_func_state(2);
            return 2;
        }
        else if(helio.get_func_state() == 2 && (command.substring(6,12) == helio.get_glow_color())){
            helio.set_func_state(0);
            return 0;
        }
        else {
            helio.set_func_state(2);
            String temp = command.substring(6,12);
            helio.set_glow_color(temp);
            return 1;
        }
    }
    else if (command.startsWith("BOUNCY")) {
        if(helio.get_func_state() == 3){
            helio.set_func_state(0);
            return 0;
        }
        else {
            helio.set_func_state(3);
            return 1;
        }
    }
    else if (command.startsWith("NOTIF")){
        if(command.substring(6,7) == "G"){
            String glow_temp = helio.get_glow_color();
            state_temp = helio.get_func_state();
            helio.set_func_state(2);
            helio.glow_LED(command.substring(8,14));
            helio.set_glow_color(glow_temp);
            helio.set_func_state(state_temp);
            helio.write_rgb_led(helio.get_global_r(), helio.get_global_g(), helio.get_global_b());
            return 1;
        }
        else if(command.substring(6,7) == "B"){
            return helio.blink_LED(command.substring(8, 14), 500, 6);
        }
        else return -1;
    }
    else if (command.startsWith("T_ACT")) {
        return helio.toggle_activation_state();
    }
    else if (command.startsWith("BAT_READ")){
        return bat_level;
    }
    else if (command.startsWith("RESET")){
        NVIC_SystemReset();
    }
    else if (command.startsWith("CHARGE")){
        helio.set_func_state(4);
            return 1;
    }
    return -1;
}
