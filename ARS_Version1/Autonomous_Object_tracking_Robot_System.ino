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
  Serial.print("Starting...\n");
  pixy.init();
}

void enable_ultrasonic_sensor_interface(){
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
  pinMode(line_pin_left, INPUT);
  pinMode(line_pin_middle, INPUT);
  pinMode(line_pin_right, INPUT);
}

struct Lines{
  byte line_left;
  byte line_middle;
  byte line_right;
};

struct TargetObject{
  uint16_t signature;
  uint16_t pos_x;
  uint16_t pos_y;
  uint16_t width;
  uint16_t height;
};

struct SensorData{
  Lines lines_info;
  TargetObject target_object_info;
  float* fov_distances;
};

#define queue_idx_size 10
int queue_idx = 0;
float distance_circular_queue[10];

void obstacle_distance_process(void)
{
  float return_time, distance;
  digitalWrite(trig, HIGH);
  delay(5);
  digitalWrite(trig, LOW);
  return_time = pulseIn(echo, HIGH);
  distance = ((float)(340*return_time)/10000)/2;
  distance_circular_queue[(queue_idx++)%queue_idx_size] = distance;
}

struct TargetObject target_camera_data_parser(){
  int i;
  TargetObject target_object;
  pixy.ccc.getBlocks();
  if(pixy.ccc.numBlocks){
    Serial.println(pixy.ccc.numBlocks); 
    target_object.signature = pixy.ccc.blocks[0].m_signature;
    target_object.pos_x = pixy.ccc.blocks[0].m_x;
    target_object.pos_y = pixy.ccc.blocks[0].m_x;
    target_object.width = pixy.ccc.blocks[0].m_width;
    target_object.height = pixy.ccc.blocks[0].m_height;

    // for (i=0; i<pixy.ccc.numBlocks; i++) { 
    //   //todo : 장애물 데이터의 파싱
    // }
    return target_object;
  }
}

struct Lines line_location_process(){
  Lines lines;
  
  byte line_left = digitalRead(line_pin_left);
  byte line_middle = digitalRead(line_pin_middle);
  byte line_right = digitalRead(line_pin_right);

  lines.line_left = line_left;
  lines.line_middle = line_middle;
  lines.line_right = line_right;

  return lines;
}

struct SensorData sensor_data_process(){
  SensorData sensor_data;
  sensor_data.lines_info = line_location_process();
  sensor_data.target_object_info = target_camera_data_parser();
  obstacle_distance_process();
  sensor_data.fov_distances = distance_circular_queue;

  return sensor_data;
}

void motor_control_test(){
  digitalWrite(EA, HIGH);

  digitalWrite(M_IN1, motorA_vector);
  digitalWrite(M_IN2, !motorA_vector);

  delay(5000);
  digitalWrite(EA, LOW);

  digitalWrite(M_IN1, LOW);
  digitalWrite(M_IN2, LOW);

  delay(2000);
  digitalWrite(EA, HIGH);

  digitalWrite(M_IN1, !motorA_vector);
  digitalWrite(M_IN2, motorA_vector);
}

void test_sensor_input(){
  delay(1000);
  SensorData sensor_data = sensor_data_process();
  Serial.print("\nline left : ");
  Serial.print(sensor_data.lines_info.line_left);
  Serial.print("\nline middle : ");
  Serial.print(sensor_data.lines_info.line_middle);
  Serial.print("\nline right : ");
  Serial.print(sensor_data.lines_info.line_right);
  Serial.print("\nsignature : ");
  Serial.print(sensor_data.target_object_info.signature);
  Serial.print("\npos_x : ");
  Serial.print(sensor_data.target_object_info.pos_x);
  Serial.print("\npos_y : ");
  Serial.print(sensor_data.target_object_info.pos_y);
  Serial.print("\nwidth : ");
  Serial.print(sensor_data.target_object_info.width);
  Serial.print("\nheight : ");
  Serial.print(sensor_data.target_object_info.height);
  Serial.print("\ndistance : ");
  Serial.print(sensor_data.fov_distances[0]);
}

void setup() {
  Serial.begin(9600);
  enable_camera_interface();
  enable_ultrasonic_sensor_interface();
  enable_motor_signal();
}

void loop() {
  delay(1000);
  SensorData sensor_data = sensor_data_process();
  //test_sensor_input();
}
