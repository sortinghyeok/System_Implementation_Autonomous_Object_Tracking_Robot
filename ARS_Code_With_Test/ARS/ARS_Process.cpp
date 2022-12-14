#include "ARS_Process.h"
#include "ARS_Init.h"

float getDistance(float returnTime)
{
	if (returnTime < 0)
		return -1;

	return ((float)(340.0f * returnTime) / 10000.0f) / 2.0f;
}

void fovDistanceProcess(float* distances, int& currentIdx)
{
	srand((unsigned int)time(NULL));
	if (currentIdx >= queue_size || currentIdx < 0)
		currentIdx = 0;
	for (int i = 0; i < queue_size; i++)
	{
		distances[currentIdx] = getDistance((rand()*100)%60+1);
		currentIdx = (currentIdx + 1) % queue_size;
	}
}

float meanFilter(int& length, float prevAverage, float newValue)
{
	if (length <= 0 || length >= 10)
	{
		length = 0;
		return 0;
	}
		
	if (length == 1) {
		length++;
		return newValue;
	}

	float oldWeight = (length - 1) / (float)length;
	float newWeight = 1 / (float)length;
	float filteredDistance = (prevAverage * oldWeight) + (newValue * newWeight);

	if (length < 10)
		length++;
	return filteredDistance;
}

SensorData sensorDataProcess(float fovDistances[], int& currentIdx)
{
	SensorData sensorData;
	Lines linesInfo = Lines();
	TargetObject targetObjectInfo = TargetObject();
	ProcessOutput processOutput;
	
	sensorData.linesInfo = linesInfo;
	sensorData.targetObjectInfo = targetObjectInfo;
	sensorData.fovDistances = fovDistances;

	fovDistanceProcess(fovDistances, currentIdx);
	return sensorData;
}

float getAngle(int posX) {
	int direction;

	if (posX > 315 || posX < 0)
		return 0;

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
		return round(angle * 1.0f * 1000) / 1000;
	}
	else
	{
		angle = (30.0f - angle);
		return round(angle * -1.0f * 1000) / 1000;
	}
}

ProcessOutput objectTrackingProcess(SensorData sensorData, float distance) {
	bool onSignal = false;

	int currentPositionX = sensorData.targetObjectInfo.posX;
	float angle = getAngle(currentPositionX);
	if (angle > 30.0 || angle < -30.0)
		angle = 0;
	float revolvingRatio = angle / 30.0;
	//float spin_radius = radius/2 * sin(radians(radius/2));

	ProcessOutput processOutput;

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

	if (sensorData.targetObjectInfo.signature == 1)
		onSignal = true;
	else
		onSignal = false;

	processOutput.direction = revolvingRatio;
	processOutput.onSignal = onSignal;

	return processOutput;
}

struct ProcessOutput lineTrackingProcess(SensorData sensor_data) {
	bool onSignal = false;
	int direction = 0; // -1 : left, 0 : middle, 1 : right

	if (sensor_data.targetObjectInfo.signature == 1)
		onSignal = false; //target?? ???? ???? ???? ???? off
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
	processOutput.onSignal = onSignal;
	processOutput.speed = 10.0;

	return processOutput;
}

ProcessOutput hazardPreventionProcess(SensorData sensor_data, float distance) {
	bool onSignal = false;
	float speed = 0.0;
	float revolving_ratio = 0.0;

	if (distance <= 20.0)
	{
		if (distance >= 10.0)
		{
			speed = distance / 3.0;
			onSignal = true;
		}
		else if (distance <= 10.0)
		{
			speed = 0.0;
			onSignal = true;
		}
	}
	else
	{
		onSignal = false;
	}

	int currentPositionX = sensor_data.targetObjectInfo.posX;
	float angle = getAngle(currentPositionX);
	if (angle < -30.0f || angle > 30.0f)
		angle = 0;
	revolving_ratio = angle / 30.0f;

	ProcessOutput process_output;
	process_output.direction = revolving_ratio;
	process_output.onSignal = onSignal;
	process_output.speed = round(speed * 1000.0f) / 1000.0f ;
	
	return process_output;
}

ModifiedData priorityMaker(ProcessOutput outputFromObjectProcess, ProcessOutput outputFromLineProcess, ProcessOutput outputFromHazardProcess) {

	ModifiedData selectedStructure;
	if (outputFromHazardProcess.onSignal == true)
	{
		selectedStructure.speed = outputFromHazardProcess.speed;
		selectedStructure.direction = outputFromHazardProcess.direction;
		selectedStructure.priorityCode = HAZARD_PREVENTION;
	}
	else if (outputFromObjectProcess.onSignal == true)
	{
		selectedStructure.speed = outputFromObjectProcess.speed;
		selectedStructure.direction = outputFromObjectProcess.direction;
		selectedStructure.priorityCode = OBJECT_TRACKING;
	}
	else if (outputFromLineProcess.onSignal == true) {
		selectedStructure.speed = outputFromLineProcess.speed;
		selectedStructure.direction = outputFromLineProcess.direction * 60;
		selectedStructure.priorityCode = LINE_TRACKING;
	}
	else
	{
		selectedStructure.speed = 0;
		selectedStructure.direction = 0;
		selectedStructure.priorityCode = REMOTE_OFF;
	}

	return selectedStructure;
}

ModifiedData setReader(ModifiedData selectedStructure) {
	ModifiedData motorVector;
	if (selectedStructure.speed > 60.0)
	{
		motorVector.speed = TEMPORARY_PREV_DISTANCE;
		motorVector.direction = 0.0;
	}
	else
	{
		motorVector.speed = selectedStructure.speed;
		motorVector.direction = selectedStructure.direction;
	}

	if (selectedStructure.priorityCode > 2 && selectedStructure.priorityCode < 0)
	{
		motorVector.priorityCode = REMOTE_OFF;
	}
	else
	{
		motorVector.priorityCode = selectedStructure.priorityCode;
	}

	return motorVector;
}

AppliedData activeMotorInterface(ModifiedData motorVector)
{
	AppliedData appliedData;
	float speed = motorVector.speed;
	float direction = motorVector.direction;

	if (direction <= 0.1f)
	{
		if (direction >= -0.1f)
		{
			appliedData.leftWheel = REGULAR_ROTATION;
			appliedData.rightWheel = REGULAR_ROTATION;
		}
	}
	else if (direction < 0)
	{
		appliedData.leftWheel = REVERSE_ROTATION;
		appliedData.rightWheel = REGULAR_ROTATION;
	}
	else
	{
		appliedData.leftWheel = REGULAR_ROTATION;
		appliedData.rightWheel = REVERSE_ROTATION;
	}

	appliedData.appliedWheelSpeed = speed * abs(direction);

	return appliedData;
}
