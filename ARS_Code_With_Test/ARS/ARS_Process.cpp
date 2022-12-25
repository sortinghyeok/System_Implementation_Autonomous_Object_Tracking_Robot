#include "ARS_Process.h"
#include "ARS_Init.h"

float getDistance(float returnTime)
{
	return ((float)(340.0f * returnTime) / 10000.0f) / 2.0f;
}

void stackDistance(float* distances, int& currentIdx)
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < queue_size; i++)
	{
		distances[currentIdx] = getDistance((rand()*100)%60+1);
		currentIdx = (currentIdx + 1) % queue_size;
	}
}

float averageFilter(int& length, float prevAverage, float newValue)
{
	if (length == 1) {
		length++;
		return newValue;
	}

	float oldWeight = (length - 1) / (float)length;
	float newWeight = 1 / (float)length;
	float res = (prevAverage * oldWeight) + (newValue * newWeight);

	if (length < 10)
		length++;
	return res;
}

SensorData sensorDataProcess(float* fovDistances, int& currentIdx)
{
	SensorData sensorData;
	Lines linesInfo = Lines();
	TargetObject targetObjectInfo = TargetObject();
	ProcessOutput processOutput;
	
	sensorData.linesInfo = linesInfo;
	sensorData.targetObjectInfo = targetObjectInfo;
	sensorData.fovDistances = fovDistances;

	int& currentIdxLoc = currentIdx;
	/*sensor_data.target_object_info = target_camera_data_parser();*/
	stackDistance(fovDistances, currentIdxLoc);
	return sensorData;
}

float getAngle(int posX) {
	int direction;
	float center = (float)fov_center;
	if ((float)posX <= center)
		direction = -1;
	else
		direction = 1;
	float multiplied_number = 60.0f / 315.0f;
	float angle = posX * multiplied_number;

	if (angle > 30.0f)
	{
		angle = angle - 30.0f;
		return round(angle * 1.0f * 1000.0f) / 1000.0f;
	}
	else
	{
		angle = (30.0f - angle);
		return round(angle * -1.0f * 1000.0f) / 1000.0f;
	}
}

ProcessOutput objectTrackingProcess(SensorData sensorData, int& queueLength, float prevAverage) {
	bool onSignal = false;
	if (sensorData.targetObjectInfo.signature == 1)
		onSignal = true;
	else
		onSignal = false;

	int currentPositionX = sensorData.targetObjectInfo.posX;
	float angle = getAngle(currentPositionX);
	if (angle > 30.0 || angle < -30.0)
		angle = 0;
	float revolvingRatio = angle / 30.0;
	//float spin_radius = radius/2 * sin(radians(radius/2));

	ProcessOutput processOutput;
	float distance = averageFilter(queueLength, prevAverage, getDistance(3000.0f));
	if (distance < 20.0)
	{
		onSignal = false;
		float distance_difference = distance - 10.0;
		if (distance_difference < 0.0)
		{
			processOutput.speed = 0.0;
		}
		else
			processOutput.speed = distance_difference;
	}
	else
	{
		onSignal = true;
		processOutput.speed = distance / 3;
	}

	processOutput.direction = revolvingRatio;
	processOutput.on = onSignal;

	return processOutput;
}

struct ProcessOutput lineTrackingProcess(SensorData sensor_data) {
	bool onSignal = false;
	int direction = 0; // -1 : left, 0 : middle, 1 : right

	if (sensor_data.targetObjectInfo.signature == 1)
		onSignal = false; //target이 있을 경우 라인 추적 off
	else
		onSignal = true;

	if (sensor_data.linesInfo.lineMiddle)
		direction = 0;
	if (sensor_data.linesInfo.lineLeft)
		direction -= 1;
	if (sensor_data.linesInfo.lineRight)
		direction += 1;

	ProcessOutput processOutput;

	float revolving_ratio = (float)direction / 3.0;
	processOutput.direction = revolving_ratio;
	processOutput.on = onSignal;
	processOutput.speed = 10.0;

	return processOutput;
}