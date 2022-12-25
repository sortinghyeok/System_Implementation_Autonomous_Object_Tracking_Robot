#include "pch.h"
#include "../ARS/ARS_Process.h"
#include <algorithm>

float fovDistances[10];
int currentIdx = 0;
int queueLength = 0;
float prevAverage = 0;
TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}
TEST(GetDistanceTest, plusInput) {
	EXPECT_EQ(getDistance(10000.0f), 170.0f);
}
TEST(StackDistanceTest, plusInput)
{
	float sum = 0;
	int validCount = 0;
	stackDistance(&fovDistances[currentIdx], currentIdx);
	for (int i = 0; i < queue_size; i++)
	{
		if (fovDistances[i])
		{
			validCount++;
			sum+=fovDistances[i];
		}
	}
	prevAverage = sum / 10.0f;
	EXPECT_EQ(validCount, 10);
}

TEST(AverageFilterTest, size_1)
{
	float newDistance = getDistance(3000.0f);
	queueLength++;
	EXPECT_EQ(averageFilter(queueLength, prevAverage, newDistance), 51);
}

TEST(AverageFilterTest, fullSize_1)
{
	stackDistance(&fovDistances[currentIdx], currentIdx);
	float newDistance = getDistance(500.0f); //return 8.5
	int testLength = 10;
	float fovDistances[10] = { 25.0f, 25.0f, 25.0f, 25.0f ,25.0f ,25.0f ,25.0f, 25.0f, 25.0f, 25.0f };
	EXPECT_EQ(averageFilter(testLength, 25.0f, newDistance), (float)23.35);
}

TEST(SensorDataTest, idx_0)
{
	int currentIdx = 0;
	sensorDataProcess(&fovDistances[currentIdx], currentIdx);
	EXPECT_EQ(currentIdx, 0);
}

TEST(SensorDataTest, idx_3)
{
	int currentIdx = 3;
	sensorDataProcess(&fovDistances[currentIdx], currentIdx);
	EXPECT_EQ(currentIdx, 3);
}

TEST(GetAngleTest, x_315)
{
	EXPECT_EQ(getAngle(315), 30.0f);
}

TEST(GetAngleTest, x_300)
{
	EXPECT_EQ(getAngle(300), 27.143f);
}

TEST(ObjectTrackingProcessTest, size_1)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 315;
	prevAverage = 15.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, queueLength, prevAverage);
	EXPECT_TRUE(processOutput.on);
	EXPECT_EQ(processOutput.direction, 1);
}

TEST(LineTrackingProcessTest, test_1)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.linesInfo.lineMiddle = 1;
	sensorData.linesInfo.lineLeft = 1;
	sensorData.linesInfo.lineRight = 1;

	ProcessOutput processOutput = lineTrackingProcess(sensorData);

	EXPECT_EQ(processOutput.on, false);
	EXPECT_EQ(processOutput.direction, 0);
	EXPECT_EQ(processOutput.speed, 10);
}