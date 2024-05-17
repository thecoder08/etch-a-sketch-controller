#include <Stepper.h>

typedef struct {
  long int x;
  long int y;
} Point;

Point head = {
  .x = 0,
  .y = 0
};
Point newPoint = {
  .x = 0,
  .y = 0
};

char pointBuffer[30];
int index = 0;

Stepper stepperX = Stepper(2048, 4, 6, 5, 7);
Stepper stepperY = Stepper(2048, 8, 10, 9, 11);

void setup() {
  // put your setup code here, to run once:
  stepperX.setSpeed(10);
  stepperY.setSpeed(10);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    if (Serial.available() > 0) {
      pointBuffer[index] = Serial.read();
      if (pointBuffer[index] == '\n') {
        char* pEnd;
        newPoint.x = strtol(pointBuffer, &pEnd, 10);
        newPoint.y = strtol(pEnd, NULL, 10);
        Serial.print("stepping ");
        Serial.print(newPoint.x);
        Serial.print(" ");
        Serial.println(newPoint.y);
        stepperX.step(-(newPoint.x - head.x));
        head.x = newPoint.x;
        stepperY.step(-(newPoint.y - head.y));
        head.y = newPoint.y;
        index = 0;
      }
      index++;
    }
}
