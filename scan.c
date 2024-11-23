/*#define THRESHHOLD 50
 #define REJECTWIDTH 5
 #define REJECTDIST  80
 */

//#include "uart-interrupt.h"
//#include "open_interface.h"
//#include "servo.h"
//#include "Timer.h"
//#include "adc.h"
//#include "ping.h"
//#include "obstacle.h"
//#include "button.h"
//#include "movement.h"
//
//// Scan and distance thresholds
//#define SCAN_ANGLE_STEP 2
//#define IR_THRESHOLD 50
//#define TARGET_DISTANCE_CM 5
//
//typedef struct
//{
//    int start_angle;
//    int end_angle;
//    int distance;
//} ObjectInfo;
//
//ObjectInfo detected_objects[10];
//int object_count = 0;
//float distances[] = {9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 50};
//int ir_values[] = {1499, 1277, 1147, 1087, 987, 955, 919, 877, 842, 833, 803, 795, 779, 751, 731};
//
//void setup()
//{
//    lcd_init();
//   // uart_init(115200);
//    uart_interrupt_init();
//    adc_init();
//    ping_init();
//    servo_init();
//    oi_t *sensor_data = oi_alloc();
//    oi_init(sensor_data);
//    // uart_sendStr("System setup complete.\n");
//}
//
//int adc_distance()
//{
//    int i;
//    int raw_ir = adc_read();
//    if (raw_ir >= ir_values[0])
//    {
//        return distances[0];
//    }
//    if (raw_ir <= ir_values[14])
//    {
//        return distances[14];
//    }
//    for (i = 0; i < 14; i++)
//    {
//        if (raw_ir <= ir_values[i] && raw_ir > ir_values[i + 1])
//        {
//            float slope = (distances[i + 1] - distances[i])
//                    / (float) (ir_values[i + 1] - ir_values[i]);
//            return distances[i] + slope * (raw_ir - ir_values[i]);
//        }
//    }
//    return -1;
//}
//
//void scan()
//{
//    int angle;      // Variable declared outside of for loop
//    int i;          // Variable declared outside of for loop
//
//    int in_object = 0;
//    int start_angle = -1;
//
//    for (angle = 0; angle <= 180; angle += SCAN_ANGLE_STEP)
//    {
//        servo_move(angle);  // Move the servo to the specified angle
//        timer_waitMillis(150); // Allow the servo to stabilize before taking a reading
//        int ir_distance = adc_distance(); // Convert raw IR to distance
//
//        char message[20];
//        sprintf(message, "Angle %d, IR Distance %d\n\r", angle, ir_distance);
//        uart_sendStr(message);
//
//        // Detect objects based on IR distance
//        if (ir_distance < IR_THRESHOLD && !in_object)
//        {
//            // Detected the start of an object
//            in_object = 1;
//            start_angle = angle;
//        }
//        else if (ir_distance >= IR_THRESHOLD && in_object)
//        {
//            // Detected the end of an object
//            in_object = 0;
//            detected_objects[object_count].start_angle = start_angle;
//            detected_objects[object_count].end_angle = angle;
//            detected_objects[object_count].distance = ir_distance;
//            object_count++;
//        }
//    }
//}
// /*   uart_sendStr("\n");
//       // Display start and end angles of each detected object
//   //    uart_sendStr("Detected Objects:\n");
//       uart_sendStr("Object   Start Angle   End Angle");
//     //  uart_sendStr("--------------------------------\n");
//       for (i = 0; i < object_count; i++) {
//           char message[50];
//           sprintf(message,  "Object %d   Start Angle%d   End Angle%d\n\r", i + 1, detected_objects[i].start_angle, detected_objects[i].end_angle);
//           uart_sendStr(message);
//       }
//       uart_sendStr("\n");
//*/
//
//    void display_smallest_object() {
//        if (object_count == 0) {
//            uart_sendStr("\nNo objects detected.\n");
//            return;
//        }
//
//        int i;  // Variable declared outside of the for loop
//        int smallest_index = 0;
//        float smallest_width = detected_objects[0].end_angle - detected_objects[0].start_angle;
//
//        // Find the smallest object by comparing their angular widths
//        for (i = 1; i < object_count; i++) {
//            float width = detected_objects[i].end_angle - detected_objects[i].start_angle;
//            if (width < smallest_width) {
//                smallest_width = width;
//                smallest_index = i;
//            }
//        }
//
//        // Display the details of the smallest detected object
//        uart_sendStr("\nSmallest object detected:\n");
//        char message[100];
//        sprintf(message, "Start Angle: %d, End Angle: %d, Distance: %d cm\n", detected_objects[smallest_index].start_angle, detected_objects[smallest_index].end_angle, detected_objects[smallest_index].distance);
//        uart_sendStr(message);
//
//    }






    /*
     float avg, prevAvg;
     float maxDistance = 0;
     int maxAngle = 0;
     int i;
     char message[90];
     struct obstacle *obs = NULL;
     int obsCount = 0;
     int obsOpen;

     oi_t *sensor_data = oi_alloc();
     oi_init(sensor_data);
     timer_init();
     lcd_init();
     uart_interrupt_init();
     ping_init();
     adc_init();
     servo_init();

     // if button 3 is pressed and confirmed send medBot
     while (1)
     {
     uart_sendStr("Starting scan with object detection...\n\r");
     obsCount = 0;
     obsOpen = 0;
     prevAvg = 610;
     maxDistance = 0;  // Reset maximum distance for each scan
     maxAngle = -1;    // Initialize with an invalid angle
     command_byte = -1;

     for (i = 0; i <= 180; i += 2) {  // Include 180 and use 2° step for faster scanning
     if (command_byte == 'q') {
     uart_sendStr("Stopping scan as 'q' command received.\n\r");
     break;
     }

     // Move servo to the current angle
     servo_move(i);

     // Wait for the servo to settle
     timer_waitMillis(50);

     // Measure distance using sonar and read IR sensor
     avg = ping_getDistance();
     int ir_value = adc_read();

     // Debug output raw readings
     sprintf(message, "Angle: %d, Sonar Distance: %.2f cm, IR Value: %d\n\r", i, avg, ir_value);
     uart_sendStr(message);

     // Update largest distance and corresponding angle
     if (avg > maxDistance) {
     maxDistance = avg;
     maxAngle = i;
     }

     // Object detection logic based on thresholds
     if (!obsOpen && prevAvg != 0 && avg - prevAvg > THRESHHOLD) {
     obsOpen = 1;
     obsCount++;
     obs = realloc(obs, sizeof(struct obstacle) * obsCount);
     obs[obsCount - 1].start_angle = i;
     uart_sendStr("Object start detected.\n\r");
     } else if (obsOpen && avg - prevAvg < THRESHHOLD) {
     obsOpen = 0;
     obs[obsCount - 1].end_angle = i;
     uart_sendStr("Object end detected.\n\r");
     }

     prevAvg = avg;
     }

     uart_sendStr("Scan completed.\n\r");

     // Debug output the largest object data
     if (maxAngle >= 0) {
     sprintf(message, "Largest object detected at angle %d with distance %.2f cm.\n\r", maxAngle, maxDistance);
     uart_sendStr(message);

     // Move toward the detected object
     float targetDistance = maxDistance - 5;  // Stop 5 cm short of the object
     if (targetDistance > 0) {
     sprintf(message, "Moving to %.2f cm away from object at angle %d.\n\r", targetDistance, maxAngle);
     uart_sendStr(message);

     // Rotate the robot to face the detected angle
     if (maxAngle <= 90) {
     turn_left(sensor_data, maxAngle);
     } else {
     turn_right(sensor_data, 180 - maxAngle);
     }

     // Gradually approach the object
     double distance_remaining = targetDistance * 10;  // Convert to mm
     while (distance_remaining > 20) {  // Stop when close enough
     move_forward(sensor_data, distance_remaining / 2);
     distance_remaining /= 2;
     }
     } else {
     uart_sendStr("Object is already within 5 cm, no movement needed.\n\r");
     }
     } else {
     uart_sendStr("No valid object detected during scan.\n\r");
     }

     }
     */





#include <stdint.h>
#include <stdio.h> // Include for sprintf
#include "servo.h"
#include "lcd.h"
#include "Timer.h"
#include "button.h"
#include "ping.h"
#include "uart-interrupt.h"
#include "adc.h"
#include "movement.h"
#include "open_interface.h"

// Scan and distance thresholds
#define SCAN_ANGLE_STEP 2
#define IR_THRESHOLD 50
#define TARGET_DISTANCE_CM 5

typedef struct {
    int start_angle;
    int end_angle;
    int distance;
} ObjectInfo;

ObjectInfo detected_objects[10];
int object_count = 0;

typedef enum { MANUAL, AUTONOMOUS_SCAN, AUTONOMOUS_TURN, AUTONOMOUS_MOVE } Mode;
Mode current_mode = MANUAL;

float distances[] = {9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 50};
int ir_values[] = {1499, 1277, 1147, 1087, 987, 955, 919, 877, 842, 833, 803, 795, 779, 751, 731};

//right= 322000
  //      left = 1319500

void setup() {
    lcd_init();
    //uart_init(115200);
    uart_interrupt_init();
    adc_init();
    ping_init();
    servo_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    uart_sendStr("System setup complete.\n");
}

int adc_distance() {
    int i;
    int raw_ir = adc_read();
    if (raw_ir >= ir_values[0]) {
        return distances[0];
    }
    if (raw_ir <= ir_values[14]) {
        return distances[14];
    }
    for (i = 0; i < 14; i++) {
        if (raw_ir <= ir_values[i] && raw_ir > ir_values[i + 1]) {
            float slope = (distances[i + 1] - distances[i]) / (float)(ir_values[i + 1] - ir_values[i]);
            return distances[i] + slope * (raw_ir - ir_values[i]);
        }
    }
    return -1;
}

void control_movement(oi_t *sensor_data, char command) {
    switch (command) {
    case 'w':
        move_forward_with_bump(sensor_data, 10); // Move forward with bump handling
        break;
    case 'a':
        turn_counter_clockwise(sensor_data, 15);
        break;
    case 's':
        move_backwards(sensor_data, 10);
        break;
    case 'd':
        turn_clockwise(sensor_data, 15);
        break;
    default:
        uart_sendStr("Invalid movement command.\n");
        break;
    }
}

void perform_180_ir_scan() {
    int angle;      // Variable declared outside of for loop
    int i;          // Variable declared outside of for loop

    int in_object = 0;
    int start_angle = -1;

    // Iterate from 0 to 180 in steps defined by SCAN_ANGLE_STEP
    for (angle = 0; angle <= 180; angle += SCAN_ANGLE_STEP) {
        servo_move(angle);  // Move the servo to the specified angle
        timer_waitMillis(150);  // Allow the servo to stabilize before taking a reading
        int ir_distance = adc_distance(); // Convert raw IR to distance

        char message[20];
        snprintf(message, sizeof(message), "%d,%d;", angle, ir_distance);
        uart_sendStr(message);

        // Detect objects based on IR distance
        if (ir_distance < IR_THRESHOLD && !in_object) {
            // Detected the start of an object
            in_object = 1;
            start_angle = angle;
        } else if (ir_distance >= IR_THRESHOLD && in_object) {
            // Detected the end of an object
            in_object = 0;
            detected_objects[object_count].start_angle = start_angle;
            detected_objects[object_count].end_angle = angle;
            detected_objects[object_count].distance = ir_distance;
            object_count++;
        }
    }

    uart_sendStr("\n");
    // Display start and end angles of each detected object
    uart_sendStr("\nDetected Objects:\n");
    uart_sendStr("Object\tStart Angle\tEnd Angle\n");
    uart_sendStr("--------------------------------\n");
    for (i = 0; i < object_count; i++) {
        char message[50];
        snprintf(message, sizeof(message), "Object %d\t%d\t\t%d\n", i + 1, detected_objects[i].start_angle, detected_objects[i].end_angle);
        uart_sendStr(message);
    }
    uart_sendStr("\n");
}

void display_smallest_object() {
    if (object_count == 0) {
        uart_sendStr("\nNo objects detected.\n");
        return;
    }

    int i;  // Variable declared outside of the for loop
    int smallest_index = 0;
    float smallest_width = detected_objects[0].end_angle - detected_objects[0].start_angle;

    // Find the smallest object by comparing their angular widths
    for (i = 1; i < object_count; i++) {
        float width = detected_objects[i].end_angle - detected_objects[i].start_angle;
        if (width < smallest_width) {
            smallest_width = width;
            smallest_index = i;
        }
    }

    // Display the details of the smallest detected object
    uart_sendStr("\nSmallest object detected:\n");
    char message[100];
    snprintf(message, sizeof(message),
             "Start Angle: %d, End Angle: %d, Distance: %d cm\n",
             detected_objects[smallest_index].start_angle,
             detected_objects[smallest_index].end_angle,
             detected_objects[smallest_index].distance);
    uart_sendStr(message);

    // Prepare the system for the next action, such as turning towards the detected object
    current_mode = AUTONOMOUS_TURN;
}


void navigate_to_smallest_object(oi_t *sensor_data, int smallest_index) {
    if (smallest_index < 0 || smallest_index >= object_count) {
        uart_sendStr("Invalid smallest index. Cannot navigate.\n");
        return;
    }

    // Calculate the target angle as the midpoint of the detected object's start and end angles
    int target_angle = (detected_objects[smallest_index].start_angle + detected_objects[smallest_index].end_angle) / 2;

    // Ensure the target angle is within the servo's valid range (0 to 180 degrees)
    if (target_angle < 0 || target_angle > 180) {
        char error_message[80];
        snprintf(error_message, sizeof(error_message), "Invalid calculated angle: %d degrees. Aborting navigation.\n", target_angle);
        uart_sendStr(error_message);
        return;
    }

    float new_distance = detected_objects[smallest_index].distance - 7; // Adjusting for sensor placement
    char message[80];

    // Determine turning direction and move the servo
    if (current_mode == AUTONOMOUS_TURN) {
        snprintf(message, sizeof(message), "Turning to Mid Angle: %d degrees\n", target_angle);
        uart_sendStr(message);
        lcd_printf("Turning to %d deg", target_angle);

        // Move the servo to the calculated target angle
        servo_move(target_angle);
        timer_waitMillis(500); // Wait for servo to reach the position

        // Set next mode to move towards the target
        current_mode = AUTONOMOUS_MOVE;
    } else if (current_mode == AUTONOMOUS_MOVE) {
        snprintf(message, sizeof(message), "Moving towards object at %d degrees\n", target_angle);
        uart_sendStr(message);
        lcd_printf("Moving to %d cm", (int)new_distance);

        if (new_distance > TARGET_DISTANCE_CM) {
            int move_distance = (new_distance > 12) ? 15 : 5;
            move_forward_with_bump(sensor_data, move_distance);

            // After each movement, update current mode to scan again
            current_mode = AUTONOMOUS_SCAN;
        } else {
            uart_sendStr("Arrived within target distance of the smallest object.\n");
            lcd_printf("Arrived at %d cm", new_distance);
            current_mode = MANUAL;  // Stop after reaching the object
        }
    }
}


void handle_uart_data(char command, oi_t *sensor_data) {
    switch (command) {
        case 'w': case 'a': case 's': case 'd':
            control_movement(sensor_data, command);
            uart_sendStr("Movement command executed.\n");
            break;
        case 't':
            if (current_mode == MANUAL) {
                current_mode = AUTONOMOUS_SCAN;
                uart_sendStr("Switched to Autonomous Mode. Press 'h' to start scanning.\n");
            } else {
                current_mode = MANUAL;
                uart_sendStr("Switched to Manual Mode.\n");
            }
            break;
        case 'h':
            if (current_mode == AUTONOMOUS_SCAN) {
                perform_180_ir_scan(); // Perform a 180-degree IR scan
                display_smallest_object();
                uart_sendStr("Press 'h' again to move towards the smallest object.\n");
                current_mode = AUTONOMOUS_TURN;
            } else if (current_mode == AUTONOMOUS_TURN || current_mode == AUTONOMOUS_MOVE) {
                navigate_to_smallest_object(sensor_data, 0);  // Navigate to the smallest detected object
            }
            break;
        case 'q':
            uart_sendStr("Disconnecting from Client.\n");
            break;
        default:
            uart_sendStr("Invalid command received.\n");
            break;
    }
}


void handle_bump_sensor(oi_t *sensor_data) {
    oi_update(sensor_data);
    if (sensor_data->bumpLeft) {
        handle_bump(sensor_data, 'L');
    } else if (sensor_data->bumpRight) {
        handle_bump(sensor_data, 'R');
    }
}








