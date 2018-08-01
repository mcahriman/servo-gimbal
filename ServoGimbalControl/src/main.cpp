#include <Arduino.h>
#include <Servo.h>

#define  SERVO_PITCH_PIN 10
#define  SERVO_YAW_PIN 9
#define  INPUT_BUFFER_SIZE 16 
#define  COMMAND_SEPARATOR '\n'

Servo servo;


void setServoAngle(int pin, int servo_pos) {
    servo.attach(pin);
    servo.writeMicroseconds(servo_pos);
    delay(300);
    servo.detach();
}

void yield_commmand(char* command_buffer) {

    int yaw_value;
    int pitch_value;

    switch(command_buffer[0]) {
        case 'Y': //yaw
            sscanf(command_buffer, "Y%i", &yaw_value);
            Serial.println(command_buffer); 
            setServoAngle(SERVO_YAW_PIN, yaw_value);
            break;
        case 'P': //pitch
            sscanf(command_buffer, "P%i", &pitch_value);
            setServoAngle(SERVO_PITCH_PIN, pitch_value);
            break;
        case 'W':
            sscanf(command_buffer, "W%i_%i", &yaw_value, &pitch_value);
            setServoAngle(SERVO_PITCH_PIN, pitch_value);
            setServoAngle(SERVO_YAW_PIN, yaw_value);
            break;
        default:
            Serial.println(command_buffer);
            Serial.println("No comprendo, cenior");
    }
    command_buffer[0]=0;
}

void poll_serial() {
    static char command_buffer[INPUT_BUFFER_SIZE];
    static int counter = 0;

    if(Serial.available()) {
        command_buffer[counter] = Serial.read();
        if(command_buffer[counter] == COMMAND_SEPARATOR || counter > INPUT_BUFFER_SIZE - 1) {
            command_buffer[counter] = '\0';
            counter = 0;
            yield_commmand(command_buffer);
        } else {
            counter++;
        }
    }
}

void loop() {
    poll_serial();
}

void setup() {
    Serial.begin(9600);
    Serial.println("Accepting instructions");

}
