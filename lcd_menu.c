#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "uart-interrupt.h"
#include "open_interface.h"
#include <string.h>
#include <stdio.h>

typedef enum { HOME_MENU, MEDICATION_MENU, PATIENT_MENU, CONFIRM_MENU } MenuState;
MenuState current_menu = HOME_MENU;

char selected_medication[20] = "None    ";
char selected_patient[20] = "None       ";

void show_medication_menu() {
    lcd_clear();
    lcd_printf("Medication Menu\n1. Xanax\n2. Fent\n3. Percs");
    current_menu = MEDICATION_MENU;
}

void show_patient_menu() {
    lcd_clear();
    lcd_printf("Patient Menu\n1. Patient 1\n2. Patient 2\n3. Patient 3");
    current_menu = PATIENT_MENU;
}

void return_to_home_menu() {
    lcd_clear();
    lcd_printf("medBot Home Menu\n1. Select Medication2. Select Patient\n3. Confirm and Send\n4. Return to Home");
    current_menu = HOME_MENU;
}

void confirm_and_send() {
    lcd_clear();
    lcd_printf("Medication: %sPatient: %s1. Send medBot\n4. Return to menu", selected_medication, selected_patient);
    current_menu = CONFIRM_MENU;
}

void lcd_menu() {
    button_init();
    timer_init();
    lcd_init();
    uart_interrupt_init();

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    return_to_home_menu(); // Start at the home menu

    int last_button = 0;

    while (1) {
        int current_button = button_getButton();

        if (current_button != last_button) {
            switch (current_menu) {
                case HOME_MENU:
                    if (current_button == 1) {
                        show_medication_menu();
                    } else if (current_button == 2) {
                        show_patient_menu();
                    } else if (current_button == 3) {
                        confirm_and_send();
                    } else if (current_button == 4) {
                        return_to_home_menu();
                    }
                    break;

                case MEDICATION_MENU:
                    if (current_button == 1) {
                        lcd_clear();
                        lcd_printf("Selected: Xanax");
                        strcpy(selected_medication, "Xanax   ");
                        timer_waitMillis(1500);
                        return_to_home_menu();
                    } else if (current_button == 2) {
                        lcd_clear();
                        lcd_printf("Selected: Fentanyl");
                        strcpy(selected_medication, "Fentanyl");
                        timer_waitMillis(1500);
                        return_to_home_menu();
                    } else if (current_button == 3) {
                        lcd_clear();
                        lcd_printf("Selected: Percocet");
                        strcpy(selected_medication, "Percocet");
                        timer_waitMillis(1500);
                        return_to_home_menu();
                    } else if (current_button == 4) {
                        return_to_home_menu();
                    }
                    break;

                case PATIENT_MENU:
                    if (current_button == 1) {
                        lcd_clear();
                        lcd_printf("Selected: Patient 1");
                        strcpy(selected_patient, "Patient 1  ");
                        timer_waitMillis(1500);
                        return_to_home_menu();
                    } else if (current_button == 2) {
                        lcd_clear();
                        lcd_printf("Selected: Patient 2");
                        strcpy(selected_patient, "Patient 2  ");
                        timer_waitMillis(1500);
                        return_to_home_menu();
                    } else if (current_button == 3) {
                        lcd_clear();
                        lcd_printf("Selected: Patient 3");
                        strcpy(selected_patient, "Patient 3  ");
                        timer_waitMillis(1500);
                        return_to_home_menu();
                    } else if (current_button == 4) {
                        return_to_home_menu();
                    }
                    break;
                case CONFIRM_MENU:
                                    if (current_button == 1) {
                                       // code for medBot to proceed to the destination
                                        lcd_printf("Sending medBot!");
                                    } else if (current_button == 4) {
                                        return_to_home_menu();
                                    }
                                    break;
                            }

            last_button = current_button;
        }

        oi_free(sensor_data);
    }
}
