#include <Pixy2.h>
Pixy2 pixy;

#define EA 5
#define M_IN1 4
#define M_IN2 3

#define echo 6
#define trig 7

#define line_pin_left 8
#define line_pin_middle 9
#define line_pin_right 10

int motorA_vector = 1;

void enable_camera_interface(){
  Serial.begin(115200);
  Serial.print("Starting...\n");
  pixy.init();
}

void enable_ultrasonic_sensor_interface(){
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

void enable_motor_signal()
{
  pinMode(EA, OUTPUT);
  pinMode(M_IN1, OUTPUT);
  pinMode(M_IN2, OUTPUT);
}

void enable_infrared_sensor_interface(){
  Serial.begin(19200);
  pinMode(line_pin_left, INPUT);
  pinMode(line_pin_middle, INPUT);
  pinMode(line_pin_right, INPUT);
}

int read_ultrasonic(void)
{
  float return_time, howlong;
  digitalWrite(trig, HIGH);
  delay(5);
  digitalWrite(trig, LOW);
  return_time = pulseIn(echo, HIGH);
  howlong = ((float)(340*return_time)/10000)/2;
  return howlong;
}

void obstacle_distance_process(){
  int line_left = digitalRead(line_pin_left);
  int line_middle = digitalRead(line_pin_middle);
  int line_right = digitalRead(line_pin_right);
  Serial.print("line_left : ");
  Serial.print(line_left);
  Serial.print("\nline_middle : ");
  Serial.print(line_middle);
  Serial.print("\nline_right : ");
  Serial.print(line_right);
}

void setup() {
  // put your setup code here, to run once:
  enable_camera_interface();
  enable_ultrasonic_sensor_interface();
  enable_motor_signal();
}

void loop() {
  // digitalWrite(EA, HIGH);

  // digitalWrite(M_IN1, motorA_vector);
  // digitalWrite(M_IN2, !motorA_vector);

  // delay(5000);
  // digitalWrite(EA, LOW);

  // digitalWrite(M_IN1, LOW);
  // digitalWrite(M_IN2, LOW);

  // delay(2000);
  // digitalWrite(EA, HIGH);

  // digitalWrite(M_IN1, !motorA_vector);
  // digitalWrite(M_IN2, motorA_vector);

  Serial.print("Check the value!");
  Serial.print(read_ultrasonic());
  Serial.print("\n");
  delay(300);

  int i;
  pixy.ccc.getBlocks();
  if(pixy.ccc.numBlocks){
    Serial.print("Detected");
    Serial.println(pixy.ccc.numBlocks); 
    for (i=0; i<pixy.ccc.numBlocks; i++) { 
      Serial.print(" block "); 
      Serial.print(i); 
      Serial.print(": "); 
      pixy.ccc.blocks[i].print(); 
    }
  }

  delay(300);

  obstacle_distance_process();
  delay(300);
}
