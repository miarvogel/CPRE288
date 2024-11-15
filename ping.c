#include "ping.h"
#include "Timer.h"
#include "stdlib.h"

volatile unsigned long START_TIME = 0xFFFF;
volatile unsigned long END_TIME = 0;
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse
volatile unsigned int overflow_count = 0;

void ping_init (void){
    SYSCTL_RCGCGPIO_R |= 0x2;
    while(!(SYSCTL_RCGCGPIO_R & 0x2));

    GPIO_PORTB_AFSEL_R |= 0x8;
    GPIO_PORTB_PCTL_R &= (~0xF000);
    GPIO_PORTB_PCTL_R |= 0x7000;
    GPIO_PORTB_DEN_R |= 0x8;

    SYSCTL_RCGCTIMER_R |=0x8;
    while(!(SYSCTL_RCGCTIMER_R & 0x8));

    TIMER3_CTL_R &= (~0x100);
    while(TIMER3_CTL_R & 0x100);

    TIMER3_CFG_R |= 0x4;
    TIMER3_TBMR_R &= (~0x10);
    TIMER3_TBMR_R |= 0x7;
    TIMER3_CTL_R |= 0xc00;
    TIMER3_TBPR_R |= 0xFF;
    TIMER3_TBILR_R |= 0xFFFF;
    TIMER3_IMR_R |= 0x400;

    NVIC_PRI9_R = (NVIC_PRI9_R & 0xFFFFFF1F) | 0x00000020;
    NVIC_EN1_R |= 0x10;

    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable();

    TIMER3_CTL_R |= 0x100;
    while(!(TIMER3_CTL_R & 0x100));
}

void ping_trigger (void){
    STATE = LOW;
    TIMER3_CTL_R &= (~0x100);
    TIMER3_IMR_R &= (~0x400);

    GPIO_PORTB_AFSEL_R &= (~0x8);
    GPIO_PORTB_DIR_R |= 0x8;

    GPIO_PORTB_DATA_R &= (~0x8);
    GPIO_PORTB_DATA_R |= 0x8;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= (~0x8);
    timer_waitMicros(5);

    TIMER3_ICR_R |= 0x400;
    GPIO_PORTB_DIR_R &= (~0x8);
    GPIO_PORTB_AFSEL_R |= 0x8;
    TIMER3_IMR_R |= 0x400;
    TIMER3_CTL_R |= 0x100;
    while(!(TIMER3_CTL_R & 0x100));
}

void TIMER3B_Handler(void){


  // YOUR CODE HERE
  // As needed, go back to review your interrupt handler code for the UART lab.
  // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
  // includes checking the source of the interrupt and clearing the interrupt status bit.
  // Checking the source: test the MIS bit in the MIS register (is the ISR executing
  // because the input capture event happened and interrupts were enabled for that event?
  // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
  // The rest of the code in the ISR depends on actions needed when the event happens.

  if(TIMER3_MIS_R & 0x400)
  {
      TIMER3_ICR_R |=0x400;
      if(STATE == LOW)
      {
          START_TIME = TIMER3_TBR_R;
          STATE = HIGH;
      }else if(STATE == HIGH)
      {
          END_TIME = TIMER3_TBR_R;
          STATE = DONE;

      }
  }



}

float ping_getDistance (void){
    unsigned long dif;
    //char message[100];

    ping_trigger();
    while(STATE != DONE);
    if(START_TIME>END_TIME)
        dif = START_TIME - END_TIME;
    else
        dif = START_TIME + (0xFFFFFF - END_TIME);

    return dif * 0.00107;//scaled to meters

}

void ping_getPulseWidth(char *target, size_t targetSize){
    unsigned long width;
    char message[20];

    ping_trigger();
        while(STATE != DONE);
        if(START_TIME>END_TIME)
        {
            width = START_TIME - END_TIME;
            sprintf(message, "%lu \n No overflow",width);
        }
        else
        {
            width = START_TIME + (0xFFFFFF - END_TIME);
            overflow_count++;
            sprintf(message, "%lu \n overflow count %d",overflow_count ,width);

        }

        strncpy(target, message, targetSize);
        timer_waitMillis(10);
}
