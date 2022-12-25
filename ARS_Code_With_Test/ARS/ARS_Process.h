#pragma once
#include "ARS_Init.h"
#include <cmath>
#include <time.h>
#include <stdlib.h>
#define queue_size 10
#define fov_center 315.0f/2.0f

float getDistance(float returnTime);
void stackDistance(float* distances, int& currentIdx);
float averageFilter(int& length, float prevAverage, float newValue);
SensorData sensorDataProcess(float* fovDistances, int& currentIdx);
float getAngle(int posX);
ProcessOutput objectTrackingProcess(SensorData sensorData, int& queueLength, float prevAverage);
ProcessOutput lineTrackingProcess(SensorData sensor_data);