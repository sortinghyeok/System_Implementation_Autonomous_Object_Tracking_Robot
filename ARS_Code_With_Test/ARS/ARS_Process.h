#pragma once
#include "ARS_Init.h"
#include <cmath>
#include <time.h>
#include <stdlib.h>
#define queue_size 10
#define fov_center 315.0f/2.0f
#define HAZARD_PREVENTION 0
#define OBJECT_TRACKING 1
#define LINE_TRACKING 2
#define REMOTE_OFF -1
#define REGULAR_ROTATION 1
#define REVERSE_ROTATION 2

float getDistance(float returnTime);
void stackDistance(float* distances, int& currentIdx);
float averageFilter(int& length, float prevAverage, float newValue);
SensorData sensorDataProcess(float* fovDistances, int& currentIdx);
float getAngle(int posX);
ProcessOutput objectTrackingProcess(SensorData sensorData, float distance);
ProcessOutput lineTrackingProcess(SensorData sensor_data);
ProcessOutput hazardPreventionProcess(SensorData sensor_data, float distance);
ModifiedData priorityMaker(ProcessOutput outputFromObjectProcess, ProcessOutput outputFromLineProcess, ProcessOutput outputFromHazardProcess);
ModifiedData setReader(ModifiedData selectedStructure);
AppliedData activeMotorInterface(ModifiedData motor_vector);