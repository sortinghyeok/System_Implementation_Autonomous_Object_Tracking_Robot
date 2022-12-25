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

#define fov_center 315/2

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
int prev_idx = 0;
float distance_circular_queue[10];

struct ProcessOutput{
  float speed;
  float direction;
  byte on_or_off;
}

void obstacle_distance_process(void)
{
  float return_time, distance;
  digitalWrite(trig, HIGH);
  delay(5);
  digitalWrite(trig, LOW);
  return_time = pulseIn(echo, HIGH);
  distance = ((float)(340*return_time)/10000)/2;
  distance_circular_queue[queue_idx] = distance;
  prev_idx = queue_idx;
  queue_idx = (queue_idx + 1) % queue_idx_size;
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

struct ProcessOutput object_tracking_process(SensorData sensor_data){
  byte on_or_off = 0;
  byte direction = 0; // 0 : left, 1 : right

  if(sensor_data.target_object_info.signature == 1)
    on_or_off = 1;
  else
    on_or_off = 0;

  Serial.print("\non_or_off");
  Serial.print(on_or_off);
  Serial.print("\signature");
  Serial.print(signature);
  float current_position_x = sensor_data.target_object_info.pos_x;
  if(current_position_x <= fov_center)
    direction = 0;
  else
    direction = 1;
  Serial.print("\ndirection");
  Serial.print(direction);
  float multiplied_number = 60.0/315.0;
  float radius = current_position_x * multiplied_number;
  Serial.print("\nmultiplied");
  Serial.print(multiplied_number);
  Serial.print("\nradius");
  
  if(direction == 1)
    radius = radius - 30;
  else
    radius = 30 - radius;

  float revolving_ratio = radius/90.0;
  Serial.print(radius);
  //float spin_radius = radius/2 * sin(radians(radius/2));

 //To Do : Kalman Filter
  ProcessOutput process_output;
  process_output.speed = 60; //To Do. get speed from kalman filter
  process_output.direction = revolving_ratio;
  process_output.on_or_off = on_or_off;
  
  return process_output;
}

float line_tracking_process(SensorData sensor_data){
  byte on_or_off = 0;
  byte direction = 0; // -1 : left, 0 : middle, 1 : right

  if(sensor_data.target_object_info.signature == 1)
    on_or_off = 0; //target이 있을 경우 라인 추적 off
  else
    on_or_off = 1;

  if(sensor_data.lines_info.line_middle)
    direction = 0;
  if(sensor_data.lines_info.line_left)
    direction -= 1;
  if(sensor_data.lines_info.line_right)
    direction += 1

  float revolving_ratio = (float)direction/3.0;
  return revolving_ratio;
}

void hazard_prevention_process(SensorData sensor_data){
  byte on_or_off = 0;
  byte is_obstacle = 0;
  float speed = 0;
  float revolving_ratio = 0;
  //To Do : Kalman Filter로 distance 구하기
  if(distance_circular_queue[prev_idx] < 20.0 && distance_circular_queue[prev_idx] > 10.0)
  {
    speed = distance_circular_queue[prev_idx]/4.0;
  }
  else if(distance_circular_queue[prev_idx] <= 10.0)
  {
    speed = 0;
  }

  if(sensor_data.target_object_info.signature == 0)
  {
    is_obstacle = 1;
  }

  while(prev_idx != 9)
  {
    obstacle_distance_process();
  }

  //To Do : Kalman Filter후 Direction Revolving Ratio 결정하여 Return
}

void priority_maker(){

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
  Serial.print(sensor_data.fov_distances[prev_idx]);
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
  object_tracking_process(sensor_data);
}
