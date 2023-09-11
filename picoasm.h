

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ----------- //
// YMF262Input //
// ----------- //

#define YMF262Input_wrap_target 0
#define YMF262Input_wrap 24

static const uint16_t YMF262Input_program_instructions[] = {
            //     .wrap_target
    0x2022, //  0: wait   0 pin, 2             ;wait 0 SMT2 
         
    0x2023, //  1: wait   0 pin, 3             ;wait 2 clock
    0x20a3, //  2: wait   1 pin, 3                   
    0x2023, //  3: wait   0 pin, 3                   
    0x20a3, //  4: wait   1 pin, 3   
                    
    0xe02e, //  5: set    x, 14                 ;read data  0-14                      
    0x2023, //  6: wait   0 pin, 3              ; wait clock low      
    0x4001, //  7: in     pins, 1               ;read data     
    0x20a3, //  8: wait   1 pin, 3              ; wait clock high     
    0x0046, //  9: jmp    x--, 6                    
     
    0x2023, // 10: wait   0 pin, 3              ;wait clock low             
    0x4002, // 11: in     pins, 2               ;read data 15
    
    0x2021, // 12: wait   0 pin, 1              ;wait 0 SMT1
         
    0xe02e, // 13: set    x, 14                 ;read data 0-14
    0x2023, // 14: wait   0 pin, 3              ;wait 2 clock 
    0x20a3, // 15: wait   1 pin, 3              
    0x2023, // 16: wait   0 pin, 3              
    0x20a3, // 17: wait   1 pin, 3              
         
    0x2023, // 18: wait   0 pin, 3              ;wait clock low     
    0x4001, // 19: in     pins, 1               ;read data
    0x20a3, // 20: wait   1 pin, 3              ;wait clock high
    0x0052, // 21: jmp    x--, 18                    
    
    0x2023, // 22: wait   0 pin, 3              ;wait clock low     
    0x4001, // 23: in     pins, 1               read data 15     
                 
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program YMF262Input_program = {
    .instructions = YMF262Input_program_instructions,
    .length = 24,
    .origin = -1,
};

static inline pio_sm_config YMF262Input_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + YMF262Input_wrap_target, offset + YMF262Input_wrap);
    return c;
}

static inline void YMF262Input_init(PIO pio, uint sm, uint offset, uint pin) {
    pio_sm_config c = YMF262Input_program_get_default_config(offset);
    
    sm_config_set_in_pins(&c,pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, false);
    pio_gpio_init(pio, pin);       //DATA
    pio_gpio_init(pio, pin + 1);  //SMT1
    pio_gpio_init(pio, pin + 2);  //SMT2
    pio_gpio_init(pio, pin + 3);  //CLK
    
    sm_config_set_in_shift(
      &c,  // INは右シフト
      true,  // [shift_right] true to shift ISR to right, false to shift ISR to left
      true, // [autopush] whether autopush is enabled
      32);  // [push_threshold]
      
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

#endif
