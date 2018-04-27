#define MOTOR_A_ENABLE 3
#define MOTOR_A_IN1 7
#define MOTOR_A_IN2 2

#define MOTOR_B_ENABLE  6
#define MOTOR_B_IN3  4
#define MOTOR_B_IN4  5

#define ENASpeed 100
#define ENBSpeed 100

#define SENSOR_LEFT_1_PIN 8
#define SENSOR_LEFT_2_PIN 9
#define SENSOR_RIGHT_1_PIN 10
#define SENSOR_RIGHT_2_PIN 11

#include <SoftwareSerial.h>

SoftwareSerial btSerial(13, 12);

#define DELIMETER '#'

String command = "";
int mode = 0;  // 0 = remote control, 1 = follow line
int speedMotor = 200;

void setup()
{
  pinMode(MOTOR_A_ENABLE, OUTPUT);
  pinMode(MOTOR_A_IN1, OUTPUT);
  pinMode(MOTOR_A_IN2, OUTPUT);

  pinMode(MOTOR_B_ENABLE, OUTPUT);
  pinMode(MOTOR_B_IN3, OUTPUT);
  pinMode(MOTOR_B_IN4, OUTPUT);

  pinMode(SENSOR_LEFT_1_PIN, INPUT);
  pinMode(SENSOR_LEFT_2_PIN, INPUT);
  pinMode(SENSOR_RIGHT_1_PIN, INPUT);
  pinMode(SENSOR_RIGHT_2_PIN, INPUT);

  analogWrite(MOTOR_A_ENABLE, speedMotor);
  analogWrite(MOTOR_B_ENABLE, speedMotor);

  delay(100);
  btSerial.begin(9600);
  delay(100);
  Serial.begin(9600);
  delay(100);
  turnOffMotors();
}

void loop()
{
  if (btSerial.available()) {
    char readChar = btSerial.read();

    if (readChar != DELIMETER)
      command =  command + readChar;
    else
      doCommand();

    Serial.write(readChar);
  }

  if (Serial.available()) {
    btSerial.write(Serial.read());
  }

  if (mode == 1) {
    int sensorLeft1Value = digitalRead(SENSOR_LEFT_1_PIN);
    int sensorLeft2Value = digitalRead(SENSOR_LEFT_2_PIN);
    int sensorRight1Value = digitalRead(SENSOR_RIGHT_1_PIN);
    int sensorRight2Value = digitalRead(SENSOR_RIGHT_2_PIN);

    int leftSensor = sensorLeft1Value || sensorLeft2Value;
    int rightSensor = sensorRight1Value || sensorRight2Value;

    if (!leftSensor && !rightSensor) {
      moveForward();
    }
    if (leftSensor && !rightSensor) {
      turnRight();
    }
    if (!leftSensor && rightSensor) {
      turnLeft();
    }
    if (leftSensor && rightSensor) {
      turnOffMotors();
    }
  }
}

void doCommand()
{
  if (command == "forward_down")
  {
    moveForward();
  }
  else if (command == "forward_up")
  {
    turnOffMotors();
  }
  else if (command == "backward_down")
  {
    moveBackward();
  }
  else if (command == "backward_up")
  {
    turnOffMotors();
  }
  else if (command == "left_down")
  {
    turnLeft();
  }
  else if (command == "left_up")
  {
    turnOffMotors();
  }
  else if (command == "right_down")
  {
    turnRight();
  }
  else if (command == "right_up")
  {
    turnOffMotors();
  }
  else if (command.indexOf("=") > 0)  {
    String variableName = command.substring(0, command.indexOf("="));
    String variableValueString = command.substring(command.indexOf("=") + 1);

    Serial.println("variableName: " + variableName);
    Serial.println("variableValueString: " + variableValueString);

    if (variableName == "speed")
    {
      int variableValue = variableValueString.toInt();
      speedMotor = variableValue;

      analogWrite(MOTOR_A_ENABLE, speedMotor);
      analogWrite(MOTOR_B_ENABLE, speedMotor);
    }
    else if (variableName == "mode")
    {
      int variableValue = variableValueString.toInt();
      mode = variableValue;

      turnOffMotors();
    }
  }

  command = "";
}

void turnOffMotors() {
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, LOW);

  digitalWrite(MOTOR_B_IN3, LOW);
  digitalWrite(MOTOR_B_IN4, LOW);
}

void moveForward() {
  digitalWrite(MOTOR_A_IN1, HIGH);
  digitalWrite(MOTOR_A_IN2, LOW);

  digitalWrite(MOTOR_B_IN3, HIGH);
  digitalWrite(MOTOR_B_IN4, LOW);
}

void moveBackward() {
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, HIGH);

  digitalWrite(MOTOR_B_IN3, LOW);
  digitalWrite(MOTOR_B_IN4, HIGH);
}

void turnLeft() {
  digitalWrite(MOTOR_A_IN1, HIGH);
  digitalWrite(MOTOR_A_IN2, LOW);

  digitalWrite(MOTOR_B_IN3, LOW);
  digitalWrite(MOTOR_B_IN4, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, LOW);

  digitalWrite(MOTOR_B_IN3, HIGH);
  digitalWrite(MOTOR_B_IN4, LOW);
}


