#ifndef helio_functions_h
#define helio_functions_h
#include "application.h"
#include <math.h>
class Functions{
  public: 
    Functions(int t);
    int global_r, global_g, global_b;
    String state;
    int func_state; //0=nothing, 1=rainbow, 2=breathe
    String glow_color;
    
    //Setter for function variable depending on value
    void set_func_state(int fm);
    
    //Getter for func_state variable
    int get_func_state();
    
    //Setter for breathe color
    void set_glow_color(String gc);
    
    //Getter for breathe color
    String get_glow_color();
    
    //Getter for global_r variable
    int get_global_r();
    
    //Getter for global_g vagable
    int get_global_g();
    
    //Getter for global_b variable
    int get_global_b();
    
    //Takes command from client and modifes it into an int that is stored to the global colors
    int set_rgb_led(String command);
    
    //Changes LED color based on values going RGB
    void write_rgb_led(int red, int green, int blue);
    
    //Cycles through various colors.
    int rainbow_fun();
    
    //Blink LEDs from chosen color, speed(ms), and how many times it will blink
    int blink_LED(String blink_color, int rate, int iter);
    
    //Breathes LED 
    int glow_LED(String command);
    
    //Sends semi random values to leds for intersting effects
    int bouncy();
    
    //Low power mode for charging, disconnect for two minutes
    void charge();
    
    //Turns off LEDs by changing string state, returns core led control back to core, resets func_state to 1
    int toggle_activation_state();
    
    //Saves current global color to memory
    int save_color_to_mem();
    
    //Samples battery for values and returns the value that has been averaged and constrained
    int battery_reader();
  
  private:
    
    //ratio for rgb
    void color_ratio(String color);
    
    
    int LED_PIN_COMMON_1;
    int LED_PIN_RED_1;
    int LED_PIN_GREEN_1;
    int LED_PIN_BLUE_1;
    int LED_PIN_COMMON_2;
    int LED_PIN_RED_2;
    int LED_PIN_GREEN_2;
    int LED_PIN_BLUE_2;
    
};

#endif
