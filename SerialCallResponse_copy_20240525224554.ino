#include <AFMotor.h>
#include <Servo.h>

char inBytes[11];
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
Servo servo;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  servo.attach(10);
  servo.write(90);

  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  handShake();  // send a byte to establish contact until receiver responds
}

void loop() {
  delay(50);
  int avalableBytes = Serial.available();
  if (avalableBytes > 0) {
    if (avalableBytes == 11) {
      for (int i = 0; i < 11; i++) {
        inBytes[i] = Serial.read();
      }
      String in = String(inBytes);
      int direction1 = in.substring(0,1).toInt();
      int speed1 = in.substring(1,4).toInt();
      int direction2 = in.substring(4,5).toInt();
      int speed2 = in.substring(5,8).toInt();
      int angle = in.substring(8,11).toInt();

      if (direction1 > 0) {
        motor1.run(FORWARD);
      } else {
        motor1.run(BACKWARD);
      }

      if (direction2 > 0) {
        motor2.run(FORWARD);
      } else {
        motor2.run(BACKWARD);
      }

      motor1.setSpeed(speed1);
      motor2.setSpeed(speed2);
      servo.write(angle);

      Serial.write(1);
    } else {
      motor1.setSpeed(0);
      motor2.setSpeed(0);
      servo.write(90);
    }
  }
}

void handShake() {
  while (Serial.available() <= 0) {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);
  }
  Serial.read();
}

