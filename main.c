#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "uart-interrupt.h"
#include "open_interface.h"
#include <string.h>
#include <stdio.h>
#include "lcd_menu.h"


int main (void){
    lcd_menu();
}











/*  case PATIENT_ROOM_MENU:
            if (current_button == 1)
            {
                lcd_clear();
                lcd_printf("Dispensing %s", selected_medication);
                // Dispense medication logic here
                timer_waitMillis(1500);
                patient_room_options(); // Return to patient room options
            }
            else if (current_button == 2)
            {
                lcd_clear();
                lcd_printf("Returning to base");
                // Logic for sending medBot away
                timer_waitMillis(1500);
                return_to_home_menu();
            }
            break;


/*void send_cybot_to_patient_room()
{
    lcd_clear();
    lcd_printf("medBot en route\nPatient: %s", selected_patient);
    // Implement actual navigation logic here
    current_menu = PATIENT_ROOM_MENU;
}

void patient_room_options() {
    lcd_clear();
    lcd_printf("1. Dispense Med\n2. Send Away");
    current_menu = PATIENT_ROOM_MENU;
}
*/
// PATIENT_ROOM_MENU
