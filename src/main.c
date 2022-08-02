/**
 * @file main.c
 * @author Hamid Salehi (hamsame.dev@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Key.h"
#include "KeyPort.h"

#define LED         DDRA
#define LED0        GPIOA->OutputData.Bit0
#define LED1        GPIOA->OutputData.Bit1
#define LED2        GPIOA->OutputData.Bit2

const Key_PinConfig keyConfig1 = {
    GPIOD, GPIO_PIN_0
};
const Key_PinConfig keyConfig2 = {
    GPIOD, GPIO_PIN_1
};
const Key_PinConfig keyConfig3 = {
    GPIOD, GPIO_PIN_2
};


// Callbacks
#if KEY_MULTI_CALLBACK
    Key_HandleStatus Key1_onPressed(Key* key, Key_State state);
    
    Key_HandleStatus Key2_onPressed(Key* key, Key_State state);
    Key_HandleStatus Key2_onReleased(Key* key, Key_State state);
    
    Key_HandleStatus Key3_onPressed(Key* key, Key_State state);
    Key_HandleStatus Key3_onReleased(Key* key, Key_State state);
    Key_HandleStatus Key3_onHold(Key* key, Key_State state);
#else
    Key_HandleStatus Key1_onChange(Key* key, Key_State state);
    Key_HandleStatus Key2_onChange(Key* key, Key_State state);
    Key_HandleStatus Key3_onChange(Key* key, Key_State state);
#endif // KEY_MULTI_CALLBACK

static Key key1;
static Key key2;
static Key key3;

void main(void)
{

    LED = 0xFF;
    LED0 = 0;
    LED1 = 0;
    LED2 = 0;
    
    // Timer/Counter 1 initialization
    // Clock source: System Clock
    // Clock value: 1000.000 kHz
    // Mode: CTC top=OCR1A
    // OC1A output: Disconnected
    // OC1B output: Disconnected
    // Noise Canceler: Off
    // Input Capture on Falling Edge
    // Timer Period: 50 ms
    // Timer1 Overflow Interrupt: Off
    // Input Capture Interrupt: Off
    // Compare A Match Interrupt: On
    // Compare B Match Interrupt: Off
    TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
    TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
    TCNT1H=0x00;
    TCNT1L=0x00;
    ICR1H=0x00;
    ICR1L=0x00;
    OCR1AH=0xC3;
    OCR1AL=0x4F;
    OCR1BH=0x00;
    OCR1BL=0x00;

    // Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (1<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);

    
    Key_init(&KEY_DRIVER);  
    Key_add(&key1, &keyConfig1);
    Key_add(&key2, &keyConfig2);
    Key_add(&key3, &keyConfig3);
#if KEY_MULTI_CALLBACK    
    Key_onPressed(&key1, Key1_onPressed);
    
    Key_onPressed(&key2, Key2_onPressed);
    Key_onReleased(&key2, Key2_onReleased);
    
    Key_onPressed(&key3, Key3_onPressed);
    Key_onReleased(&key3, Key3_onReleased);
    Key_onHold(&key3, Key3_onHold);
#else
    Key_onChange(&key1, Key1_onChange);
    Key_onChange(&key2, Key2_onChange);
    Key_onChange(&key3, Key3_onChange);
#endif    

    // Global enable interrupts
    sei();

while (1)
    {
        // Please write your application code here
    }
}

ISR(TIMER1_COMPA_vect)
{
  Key_irq();
}

// Callbacks
#if KEY_MULTI_CALLBACK
Key_HandleStatus Key1_onPressed(Key* key, Key_State state) {
    LED0 = !LED0;
    return Key_Handled;  
}

Key_HandleStatus Key2_onPressed(Key* key, Key_State state) {
    LED1 = 1;
    return Key_NotHandled;
}
Key_HandleStatus Key2_onReleased(Key* key, Key_State state) {
    LED1 = 0;
    return Key_Handled;
}

Key_HandleStatus Key3_onPressed(Key* key, Key_State state) {
    LED2 = 1;     
    return Key_NotHandled;
}
Key_HandleStatus Key3_onReleased(Key* key, Key_State state) {
    LED2 = 0;             
    return Key_NotHandled;
}
Key_HandleStatus Key3_onHold(Key* key, Key_State state) {
    LED2 = !LED2;             
    return Key_NotHandled;
}
#else
Key_HandleStatus Key1_onChange(Key* key, Key_State state) {
    if (Key_State_Pressed == state) {
        LED0 = !LED0;
        return Key_Handled;
    }
    return Key_NotHandled; 
}
Key_HandleStatus Key2_onChange(Key* key, Key_State state) {
    switch (state) {
        case Key_State_Pressed:
            LED1 = 1;
            return Key_NotHandled;
        case Key_State_Released:
            LED1 = 0;
            return Key_Handled;
        default:
            return Key_NotHandled;
    }
}
Key_HandleStatus Key3_onChange(Key* key, Key_State state) {
    switch (state) {
        case Key_State_Pressed:
            LED2 = 1;
            break;
        case Key_State_Released:
            LED2 = 0;
            break;
        case Key_State_Hold:
            LED2 = !LED2;
            break;
    }       
    return Key_NotHandled;
}
#endif

