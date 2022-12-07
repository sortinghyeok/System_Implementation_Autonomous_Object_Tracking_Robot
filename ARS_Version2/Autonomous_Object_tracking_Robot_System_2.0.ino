#include <Pixy2.h>
Pixy2 pixy;

#define EA 5
#define M_IN1 4
#define M_IN2 3
#define EB 11
#define M_IN3 13
#define M_IN4 12

#define echo 6
#define trig 7

#define line_pin_left 8
#define line_pin_middle 9
#define line_pin_right 10

#define fov_center 315/2

#define OBJECT_TRACKING 0
#define LINE_TRACKING 1
#define HAZARD_PREVENTION 2
#define REMOTE_CONTROL 255
#define queue_idx_size 10

int motorA_vector = 1;
int motorB_vector = 2;
int queue_idx = 0;
int prev_idx = 0;

float distance_circular_queue[10];
float sum = 0;
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
  int line_left;
  int line_middle;
  int line_right;
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

struct ProcessOutput{
  float speed;
  float direction;
  int on_or_off;
};

struct ModifedData{
  float speed;
  float direction;
  int priority_code;
};

void get_distance()
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

void stack_distance()
{
  for(int i = 0; i<queue_idx_size; i++)
  {
    delay(15);
    get_distance();
  }
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

float mean_filter(){
  float average_distance = 0;
  for(int i = 0; i<queue_idx_size; i++)
  {
    average_distance += distance_circular_queue[i];
  }
  float filtered_distance = average_speed/(float)queue_idx_size;
  return filtered_distance;
}

struct Lines line_location_process(){
  Lines lines;
  
  int line_left = digitalRead(line_pin_left);
  int line_middle = digitalRead(line_pin_middle);
  int line_right = digitalRead(line_pin_right);

  lines.line_left = line_left;
  lines.line_middle = line_middle;
  lines.line_right = line_right;

  return lines;
}

struct SensorData sensor_data_process(){
  SensorData sensor_data;
  sensor_data.lines_info = line_location_process();
  sensor_data.target_object_info = target_camera_data_parser();
  stack_distance();
  sensor_data.fov_distances = distance_circular_queue;

  return sensor_data;
}

float get_angle(uint16_t pos_x){
  int direction;
  if(pos_x <= fov_center)
    direction = -1;
  else
    direction = 1;
  float multiplied_number = 60.0/315.0;
  //Serial.print("number : ");
  //Serial.println(multiplied_number);
  float angle = pos_x * multiplied_number;
  //Serial.print("radius : ");
  //Serial.println(radius);
  
  if(angle > 30.0)
  {
    angle = angle - 30.0;
    return angle * 1.0;
  }
  else
  {
    angle = (30.0 - angles);
    return angle * (-1.0);
  }
}

struct ProcessOutput object_tracking_process(SensorData sensor_data){
  int on_or_off = 0;
  if(sensor_data.target_object_info.signature == 1)
    on_or_off = 1;
  else
    on_or_off = 0;

  uint16_t current_position_x = sensor_data.target_object_info.pos_x;
  float angle = get_angle(current_position_x);
  float revolving_ratio = angle/30.0;
  //float spin_radius = radius/2 * sin(radians(radius/2));

  ProcessOutput process_output;
  float distance = mean_filter();
  if(distance < 20.0)
  {
    on_or_off = 0;
    float distance_difference = distance - 10.0;
    if(distance_difference < 0.0)
    {
      process_output.speed = 0.0;
    }
    else
      process_output.speed = distance_difference;
  }
  else 
  {
    on_or_off = 1;
    process_output.speed = distance/3;
  }

  process_output.direction = revolving_ratio;
  process_output.on_or_off = on_or_off;
  
  return process_output;
}

struct ProcessOutput line_tracking_process(SensorData sensor_data){
  int on_or_off = 0;
  int direction = 0; // -1 : left, 0 : middle, 1 : right

  if(sensor_data.target_object_info.signature == 1)
    on_or_off = 0; //target이 있을 경우 라인 추적 off
  else
    on_or_off = 1;

  if(sensor_data.lines_info.line_middle)
    direction = 0;
  if(sensor_data.lines_info.line_left)
    direction -= 1;
  if(sensor_data.lines_info.line_right)
    direction += 1;

  ProcessOutput process_output;
 
  float revolving_ratio = (float)direction/3.0;
  process_output.direction = revolving_ratio;
  process_output.on_or_off = on_or_off;
  process_output.speed = 10.0;

  return process_output;
}

struct ProcessOutput hazard_prevention_process(SensorData sensor_data){
  int on_or_off = 0;
  float speed = 0.0;
  float revolving_ratio = 0.0;
  float distance = mean_filter();
  float instance_distance = distance_circular_queue[prev_idx];
  Serial.print("prev distance : ");
  Serial.println(distance);
  if (distance - 30.0 >= instance_distance)
  {
    stack_distance();
    distance = mean_filter();
  }
  Serial.print("after distance : ");
  Serial.println(distance);

  if (distance <= 20.0)
  {
    if(distance >= 10.0)
    {
      speed = distance / 3.0;
      on_or_off = 1;
    }
  }
  else if(distance <= 10.0)
  {
    speed = 0.0;
    on_or_off = 1;
  }
  else
  {
    on_or_off = 0;
  }

  uint16_t current_position_x = sensor_data.target_object_info.pos_x;
  float angle = get_angle(current_position_x);
  revolving_ratio = angle/30.0;

  ProcessOutput process_output;
  process_output.direction = revolving_ratio;
  process_output.on_or_off = on_or_off;
  process_output.speed = speed;

  return process_output;
}

struct ModifedData priority_maker(SensorData sensor_data){
  ProcessOutput output_from_object_process = object_tracking_process(sensor_data);
  ProcessOutput output_from_line_process = line_tracking_process(sensor_data);
  ProcessOutput output_from_hazard_process = hazard_prevention_process(sensor_data);

  ModifedData selected_structure;
  if(output_from_hazard_process.on_or_off == 1)
  {
    Serial.println("HAZARD PREVENTION SELETED");
    selected_structure.speed = output_from_hazard_process.speed;
    selected_structure.direction = output_from_hazard_process.direction;
    selected_structure.priority_code = HAZARD_PREVENTION;
  }
  else if(output_from_object_process.on_or_off == 1)
  {
    Serial.println("OBJECT TRACKING SELETED");
    selected_structure.speed = output_from_object_process.speed;
    selected_structure.direction = output_from_object_process.direction;
    selected_structure.priority_code = OBJECT_TRACKING;
  }
  else if(output_from_line_process.on_or_off == 1){
    Serial.println("LINE TRACKING SELECTED");
    selected_structure.speed = output_from_line_process.speed; 
    selected_structure.direction = output_from_line_process.direction;
    selected_structure.priority_code = LINE_TRACKING;
  }

  return selected_structure;
}

struct ModifedData set_reader(ModifedData selected_structure){
  ModifedData motor_vector;

  if(selected_structure.speed > 60.0)
  {
    motor_vector.speed = 0.0;
    motor_vector.direction = 0.0;
    Serial.print("reader speed : ");
    Serial.println(motor_vector.speed);
     Serial.print("reader direction : ");
    Serial.println(motor_vector.direction);
  }
  else
  {
    motor_vector.speed = selected_structure.speed;
    motor_vector.direction = selected_structure.direction;
    Serial.print("reader speed : ");
    Serial.println(motor_vector.speed);
    Serial.print("reader direction : ");
    Serial.println(motor_vector.direction);
  }
   
  if(selected_structure.priority_code > 2 && selected_structure.priority_code < 255)
  {
    motor_vector.priority_code = 0;
  }
  else
  {
    motor_vector.priority_code = selected_structure.priority_code;
  }  

  return motor_vector;
}

void active_motor_interface(ModifedData motor_vector){
  float speed = motor_vector.speed;
  float direction = motor_vector.direction;
  Serial.println("motor activated");
  digitalWrite(EA, HIGH);
  digitalWrite(EB, HIGH);
  if(direction <= 10.0)
  {
    if(direction >= -10.0)
    {
      digitalWrite(M_IN1, motorA_vector); //좌측 바퀴 정회전
      digitalWrite(M_IN2, motorA_vector); 
      digitalWrite(M_IN3, motorB_vector); //우측 바퀴 정회전
      digitalWrite(M_IN3, motorB_vector);
    }
  }
  else if(direction < 0)
  {
    digitalWrite(M_IN1, !motorA_vector); //좌측 바퀴 역회전
    digitalWrite(M_IN2, !motorA_vector); 
    digitalWrite(M_IN3, motorB_vector); //우측 바퀴 정회전
    digitalWrite(M_IN3, motorB_vector);
  }
  else
  {
    digitalWrite(M_IN1, motorA_vector); //좌측 바퀴 정회전
    digitalWrite(M_IN2, motorA_vector);  
    digitalWrite(M_IN3, !motorB_vector); //우측 바퀴 역회전
    digitalWrite(M_IN3, !motorB_vector);
  }

  float motor_direction_multiplier = abs(direction);
  Serial.println("motor speed : ");
  Serial.println(speed * motor_direction_multiplier);
  analogWrite(EA, speed * motor_direction_multiplier);
}

void test_sensor_input(){
  delay(500);
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
  enable_infrared_sensor_interface();
  enable_motor_signal();
}

void loop() {
  SensorData sensor_data = sensor_data_process();
  ModifedData selected_structure = priority_maker(sensor_data);
  active_motor_interface(set_reader(selected_structure));
}
