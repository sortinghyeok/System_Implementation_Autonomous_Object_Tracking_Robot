#include "pch.h"
#include "../ARS/ARS_Process.h"
#include <algorithm>

float fovDistances[10];
int currentIdx = 0;
int queueLength = 0;
float prevAverage = 0;

TEST(GetDistanceTest, TC_1) {
	EXPECT_EQ(getDistance(10000.0f), 170.0f);
}

TEST(GetDistanceTest, TC_2) {
	EXPECT_EQ(getDistance(5000.0f), 85.0f);
}

TEST(GetDistanceTest, TC_3) {
	EXPECT_EQ(getDistance(0.0f), 0);
}

TEST(GetDistanceTest, TC_4) {
	EXPECT_EQ(getDistance(-1.0f), -1);
}

TEST(GetDistanceTest, TC_5) {
	EXPECT_EQ(getDistance(1000.0f), 17.0f);
}

TEST(FovDistanceProcessTest, TC_6) {
	float fov_distances[10] = {};
	int currentIdx = 0;
	fovDistanceProcess(fov_distances, currentIdx);
	EXPECT_EQ(currentIdx, 0);
	int validCount = 0;
	for (int i = 0; i < queue_size; i++)
	{
		if (fov_distances[i] >= 0)
			validCount++;
	}
	EXPECT_EQ(validCount, 10);
}

TEST(FovDistanceProcessTest, TC_7) {
	float fov_distances[10] = {1.0f};
	int currentIdx = 1;
	fovDistanceProcess(fov_distances, currentIdx);
	EXPECT_EQ(currentIdx, 1);
	int validCount = 0;
	for (int i = 0; i < queue_size; i++)
	{
		if (fov_distances[i] >= 0)
			validCount++;
	}
	EXPECT_EQ(validCount, 10);
}

TEST(FovDistanceProcessTest, TC_8) {
	float fov_distances[10] = { 1.0f, 2.0f };
	int currentIdx = 2;
	fovDistanceProcess(fov_distances, currentIdx);
	EXPECT_EQ(currentIdx, 2);
	int validCount = 0;
	for (int i = 0; i < queue_size; i++)
	{
		if (fov_distances[i] >= 0)
			validCount++;
	}
	EXPECT_EQ(validCount, 10);
}

TEST(FovDistanceProcessTest, TC_9) {
	float fov_distances[10] = { 1.0f, 2.0f, 3.0f };
	int currentIdx = 3;
	fovDistanceProcess(fov_distances, currentIdx);
	EXPECT_EQ(currentIdx, 3);
	int validCount = 0;
	for (int i = 0; i < queue_size; i++)
	{
		if (fov_distances[i] >= 0)
			validCount++;
	}
	EXPECT_EQ(validCount, 10);
}

TEST(FovDistanceProcessTest, TC_10) {
	float fov_distances[10] = { 1.0f, 1.0f, 1.0f, 1.0f };
	int currentIdx = 4;
	fovDistanceProcess(fov_distances, currentIdx);
	EXPECT_EQ(currentIdx, 4);
	int validCount = 0;
	for (int i = 0; i < queue_size; i++)
	{
		if (fov_distances[i] >= 0)
			validCount++;
	}
	EXPECT_EQ(validCount, 10);
}

TEST(FovDistanceProcessTest, TC_11) {
	float fov_distances[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int currentIdx = 0;
	fovDistanceProcess(fov_distances, currentIdx);
	EXPECT_EQ(currentIdx, 0);
	int validCount = 0;
	for (int i = 0; i < queue_size; i++)
	{
		if (fov_distances[i] >= 0)
			validCount++;
	}
	EXPECT_EQ(validCount, 10);
}

TEST(FovDistanceProcessTest, TC_12) {
	float fov_distances[10] = {};
	int currentIdx = 1;
	fovDistanceProcess(fov_distances, currentIdx);
	EXPECT_EQ(currentIdx, 1);
	int validCount = 0;
	for (int i = 0; i < queue_size; i++)
	{
		if (fov_distances[i] >= 0)
			validCount++;
	}
	EXPECT_EQ(validCount, 10);
}

TEST(FovDistanceProcessTest, TC_13) {
	float fov_distances[10] = {};
	int currentIdx = 10;
	fovDistanceProcess(fov_distances, currentIdx);
	EXPECT_EQ(currentIdx, 0);
	int validCount = 0;
	for (int i = 0; i < queue_size; i++)
	{
		if (fov_distances[i] >= 0)
			validCount++;
	}
	EXPECT_EQ(validCount, 10);
}

TEST(FovDistanceProcessTest, TC_14) {
	float fov_distances[10] = {1.0f};
	int currentIdx = -1;
	fovDistanceProcess(fov_distances, currentIdx);
	EXPECT_EQ(currentIdx, 0);
	int validCount = 0;
	for (int i = 0; i < queue_size; i++)
	{
		if (fov_distances[i] >= 0)
			validCount++;
	}
	EXPECT_EQ(validCount, 10);
}

TEST(FovDistanceProcessTest, TC_15) {
	float fov_distances[10] = {};
	int currentIdx = -1;
	fovDistanceProcess(fov_distances, currentIdx);
	EXPECT_EQ(currentIdx, 0);
	int validCount = 0;
	for (int i = 0; i < queue_size; i++)
	{
		if (fov_distances[i] >= 0)
			validCount++;
	}
	EXPECT_EQ(validCount, 10);
}

TEST(FovDistanceProcessTest, TC_16) {
	float fov_distances[10] = {};
	int currentIdx = 100;
	fovDistanceProcess(fov_distances, currentIdx);
	EXPECT_EQ(currentIdx, 0);
	int validCount = 0;
	for (int i = 0; i < queue_size; i++)
	{
		if (fov_distances[i] >= 0)
			validCount++;
	}
	EXPECT_EQ(validCount, 10);
}
TEST(MeanFilterTest, TC_17)
{
	int queueLength = 1;
	float prevAverage = 0.0f;
	EXPECT_EQ(meanFilter(queueLength, prevAverage, 30.0f), 30);
}

TEST(MeanFilterTest, TC_18)
{
	int queueLength = 0;
	float prevAverage = 0.0f;
	EXPECT_EQ(meanFilter(queueLength, prevAverage, 30.0f), 0);
}

TEST(MeanFilterTest, TC_19)
{
	int queueLength = 10;
	float prevAverage = 60.0f;
	EXPECT_EQ(meanFilter(queueLength, prevAverage, 50.0f), 0);
}

TEST(MeanFilterTest, TC_20)
{
	int queueLength = -1;
	float prevAverage = 60.0f;
	EXPECT_EQ(meanFilter(queueLength, prevAverage, 50.0f), 0);
}

TEST(MeanFilterTest, TC_21)
{
	int queueLength = 9;
	float prevAverage = 45.0f;
	EXPECT_EQ(meanFilter(queueLength, prevAverage, 45.0f), 45.0f);
}

TEST(MeanFilterTest, TC_22)
{
	int queueLength = 10;
	float prevAverage = 45.0f;
	EXPECT_EQ(meanFilter(queueLength, prevAverage, 45.0f), 0);
}

TEST(MeanFilterTest, TC_23)
{
	int queueLength = 5;
	float prevAverage = 45.0f;
	EXPECT_EQ(meanFilter(queueLength, prevAverage, 35.0f), 43);
}

TEST(MeanFilterTest, TC_24)
{
	int queueLength = 2;
	float prevAverage = 50.0f;
	EXPECT_EQ(meanFilter(queueLength, prevAverage, 30.0f), 40);
}

TEST(MeanFilterTest, TC_25)
{
	int queueLength = 3;
	float prevAverage = 50.0f;
	EXPECT_EQ(round(meanFilter(queueLength, prevAverage, 30.0f) * 1000.0f)/1000.0f, 43.333f);
}

TEST(MeanFilterTest, TC_26)
{
	int queueLength = 4;
	float prevAverage = 50.0f;
	EXPECT_EQ(meanFilter(queueLength, prevAverage, 30.0f), 45);
}

TEST(MeanFilterTest, TC_27)
{
	int queueLength = 9;
	float prevAverage = 50.0f;
	EXPECT_EQ(round(meanFilter(queueLength, prevAverage, 30.0f)), 48);
}

TEST(MeanFilterTest, TC_28)
{
	int queueLength = 10;
	float prevAverage = 50.0f;
	EXPECT_EQ(round(meanFilter(queueLength, prevAverage, 30.0f)), 0);
}

TEST(MeanFilterTest, TC_29)
{
	int queueLength = 11;
	float prevAverage = 50.0f;
	EXPECT_EQ(round(meanFilter(queueLength, prevAverage, 30.0f)), 0);
}

TEST(MeanFilterTest, TC_30)
{
	int queueLength = -1;
	float prevAverage = 50.0f;
	EXPECT_EQ(round(meanFilter(queueLength, prevAverage, 30.0f)), 0);
}
TEST(SensorDataTest, TC_31)
{
	int currentIdx = 0;
	float fovDistances[10] = {};
	sensorDataProcess(&fovDistances[currentIdx], currentIdx);
	EXPECT_EQ(currentIdx, 0);
}

TEST(SensorDataTest, TC_32)
{
	int currentIdx = 1;
	float fovDistances[10] = {30.0f};
	sensorDataProcess(fovDistances, currentIdx);
	EXPECT_EQ(currentIdx, 1);
}

TEST(SensorDataTest, TC_33)
{
	int currentIdx = -1;
	float fovDistances[10] = { };
	sensorDataProcess(fovDistances, currentIdx);
	EXPECT_EQ(currentIdx, 0);
}

TEST(SensorDataTest, TC_34)
{
	int currentIdx = 9;
	float fovDistances[10] = { };
	sensorDataProcess(fovDistances, currentIdx);
	EXPECT_EQ(currentIdx, 9);
}

TEST(SensorDataTest, TC_35)
{
	int currentIdx = 8;
	float fovDistances[10] = { };
	sensorDataProcess(fovDistances, currentIdx);
	EXPECT_EQ(currentIdx, 8);
}

TEST(SensorDataTest, TC_36)
{
	int currentIdx = 10;
	float fovDistances[10] = { };
	sensorDataProcess(fovDistances, currentIdx);
	EXPECT_EQ(currentIdx, 0);
}

TEST(SensorDataTest, TC_37)
{
	int currentIdx = 11;
	float fovDistances[10] = { };
	sensorDataProcess(fovDistances, currentIdx);
	EXPECT_EQ(currentIdx, 0);
}

TEST(SensorDataTest, TC_38)
{
	int currentIdx = 5;
	float fovDistances[10] = { };
	sensorDataProcess(fovDistances, currentIdx);
	EXPECT_EQ(currentIdx, 5);
}

TEST(GetAngleTest, TC_39)
{
	EXPECT_EQ(getAngle(316), 0);
}

TEST(GetAngleTest, TC_40)
{
	EXPECT_EQ(getAngle(315), round(30.0f * 1000) / 1000);
}

TEST(GetAngleTest, TC_41)
{
	EXPECT_EQ(getAngle(300), round(27.143f * 1000) / 1000);
}

TEST(GetAngleTest, TC_42)
{
	EXPECT_EQ(getAngle(158), round(0.095f * 1000) / 1000);
}

TEST(GetAngleTest, TC_43)
{
	EXPECT_EQ(getAngle(157), round(-0.095f * 1000) / 1000);
}

TEST(GetAngleTest, TC_44)
{
	EXPECT_EQ(getAngle(100), round(-10.952f * 1000) / 1000);
}

TEST(GetAngleTest, TC_45)
{
	EXPECT_EQ(getAngle(50), round(-20.476f * 1000) / 1000);
}

TEST(GetAngleTest, TC_46)
{
	EXPECT_EQ(getAngle(1), round(-29.81f * 1000) / 1000);
}

TEST(GetAngleTest, TC_47)
{
	EXPECT_EQ(getAngle(0), round(-30.0f * 1000) / 1000);
}

TEST(GetAngleTest, TC_48)
{
	EXPECT_EQ(getAngle(-1), round(0.0f * 1000) / 1000);
}
TEST(ObjectTrackingTest, TC_49)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 315;

	float distance = 30.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 10);
	EXPECT_EQ(processOutput.direction, 1);
}

TEST(ObjectTrackingTest, TC_50)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 0;

	float distance = 30.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 10);
	EXPECT_EQ(processOutput.direction, -1);
}

TEST(ObjectTrackingTest, TC_51)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = -1;

	float distance = 30.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 10);
	EXPECT_EQ(processOutput.direction, 0);
}

TEST(ObjectTrackingTest, TC_52)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 314;

	float distance = 30.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 10);
	EXPECT_EQ(round(processOutput.direction * 100)/100, 0.99f);
}

TEST(ObjectTrackingTest, TC_53)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 1;

	float distance = 30.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 10);
	EXPECT_EQ(round(processOutput.direction * 100) / 100, -0.99f);
}

TEST(ObjectTrackingTest, TC_54)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.targetObjectInfo.posX = 316;

	float distance = 30.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_FALSE(processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 10);
	EXPECT_EQ(processOutput.direction, 0);
}

TEST(ObjectTrackingTest, TC_55)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.targetObjectInfo.posX = 315;

	float distance = 30.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_FALSE(processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 10);
	EXPECT_EQ(processOutput.direction, 1);
}

TEST(ObjectTrackingTest, TC_56)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.targetObjectInfo.posX = 314;

	float distance = 30.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_FALSE(processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 10);
	EXPECT_EQ(round(processOutput.direction*100)/100, 0.99f);
}

TEST(ObjectTrackingTest, TC_57)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.targetObjectInfo.posX = 1;

	float distance = 30.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_FALSE(processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 10);
	EXPECT_EQ(round(processOutput.direction * 100) / 100, -0.99f);
}

TEST(ObjectTrackingTest, TC_58)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.targetObjectInfo.posX = 0;

	float distance = 30.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_FALSE(processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 10);
	EXPECT_EQ(round(processOutput.direction * 100) / 100, -1);
}

TEST(ObjectTrackingTest, TC_59)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.targetObjectInfo.posX = -1;

	float distance = 30.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_FALSE(processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 10);
	EXPECT_EQ(round(processOutput.direction * 100) / 100, 0);
}

TEST(ObjectTrackingTest, TC_60)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 316;

	float distance = 300.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 100);
	EXPECT_EQ(processOutput.direction, 0);
}

TEST(ObjectTrackingTest, TC_61)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 315;

	float distance = 300.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 100);
	EXPECT_EQ(processOutput.direction, 1);
}

TEST(ObjectTrackingTest, TC_62)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 1;

	float distance = 300.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 100);
	EXPECT_EQ(round(processOutput.direction*100)/100, -0.99f);
}

TEST(ObjectTrackingTest, TC_63)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 0;

	float distance = 300.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 100);
	EXPECT_EQ(processOutput.direction, -1);
}

TEST(ObjectTrackingTest, TC_64)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = -1;

	float distance = 300.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 100);
	EXPECT_EQ(processOutput.direction, 0);
}

TEST(ObjectTrackingTest, TC_65)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.targetObjectInfo.posX = 316;

	float distance = 300.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(!processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 100);
	EXPECT_EQ(processOutput.direction, 0);
}

TEST(ObjectTrackingTest, TC_66)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.targetObjectInfo.posX = 315;

	float distance = 300.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(!processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 100);
	EXPECT_EQ(processOutput.direction, 1);
}

TEST(ObjectTrackingTest, TC_67)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.targetObjectInfo.posX = 1;

	float distance = 300.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(!processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 100);
	EXPECT_EQ(round(processOutput.direction * 100) / 100, -0.99f);
}

TEST(ObjectTrackingTest, TC_68)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.targetObjectInfo.posX = 0;

	float distance = 300.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(!processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 100);
	EXPECT_EQ(processOutput.direction, -1);
}

TEST(ObjectTrackingTest, TC_69)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.targetObjectInfo.posX = -1;

	float distance = 300.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(!processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 100);
	EXPECT_EQ(processOutput.direction, 0);
}

TEST(ObjectTrackingTest, TC_70)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 2;
	sensorData.targetObjectInfo.posX = -1;

	float distance = 300.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(!processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 100);
	EXPECT_EQ(processOutput.direction, 0);
}

TEST(ObjectTrackingTest, TC_71)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 2;
	sensorData.targetObjectInfo.posX = 1;

	float distance = 300.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(!processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 100);
	EXPECT_EQ(round(processOutput.direction * 100) / 100, -0.99f);
}

TEST(ObjectTrackingTest, TC_72)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 2;
	sensorData.targetObjectInfo.posX = 0;

	float distance = 300.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(!processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 100);
	EXPECT_EQ(processOutput.direction, -1);
}

TEST(ObjectTrackingTest, TC_73)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 2;
	sensorData.targetObjectInfo.posX = 316;

	float distance = 300.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(!processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 100);
	EXPECT_EQ(processOutput.direction, 0);
}

TEST(ObjectTrackingTest, TC_74)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 2;
	sensorData.targetObjectInfo.posX = 315;

	float distance = 300.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(!processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 100);
	EXPECT_EQ(round(processOutput.direction*100)/100, 1);
}

TEST(ObjectTrackingTest, TC_75)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 2;
	sensorData.targetObjectInfo.posX = 314;

	float distance = 300.0f;
	ProcessOutput processOutput = objectTrackingProcess(sensorData, distance);
	EXPECT_TRUE(!processOutput.onSignal);
	EXPECT_EQ(processOutput.speed, 100);
	EXPECT_EQ(round(processOutput.direction * 100) / 100, 0.99f);
}
TEST(LineTrackingTest, TC_76)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.linesInfo.lineMiddle = 1;
	sensorData.linesInfo.lineLeft = 1;
	sensorData.linesInfo.lineRight = 1;

	ProcessOutput processOutput = lineTrackingProcess(sensorData);

	EXPECT_EQ(processOutput.onSignal, false);
	EXPECT_EQ(processOutput.direction, 0);
	EXPECT_EQ(processOutput.speed, 10);
}

TEST(LineTrackingTest, TC_77)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.linesInfo.lineMiddle = 0;
	sensorData.linesInfo.lineLeft = 1;
	sensorData.linesInfo.lineRight = 1;

	ProcessOutput processOutput = lineTrackingProcess(sensorData);

	EXPECT_EQ(processOutput.onSignal, false);
	EXPECT_EQ(processOutput.direction, 0);
	EXPECT_EQ(processOutput.speed, 10);
}

TEST(LineTrackingTest, TC_78)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.linesInfo.lineMiddle = 0;
	sensorData.linesInfo.lineLeft = 0;
	sensorData.linesInfo.lineRight = 1;

	ProcessOutput processOutput = lineTrackingProcess(sensorData);

	EXPECT_EQ(processOutput.onSignal, false);
	EXPECT_EQ(round(processOutput.direction*1000)/1000, 0.333f);
	EXPECT_EQ(processOutput.speed, 10);
}

TEST(LineTrackingTest, TC_79)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.linesInfo.lineMiddle = 0;
	sensorData.linesInfo.lineLeft = 0;
	sensorData.linesInfo.lineRight = 0;

	ProcessOutput processOutput = lineTrackingProcess(sensorData);

	EXPECT_EQ(processOutput.onSignal, false);
	EXPECT_EQ(processOutput.direction, 0);
	EXPECT_EQ(processOutput.speed, 10);
}

TEST(LineTrackingTest, TC_80)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.linesInfo.lineMiddle = 1;
	sensorData.linesInfo.lineLeft = 0;
	sensorData.linesInfo.lineRight = 0;

	ProcessOutput processOutput = lineTrackingProcess(sensorData);

	EXPECT_EQ(processOutput.onSignal, false);
	EXPECT_EQ(processOutput.direction, 0);
	EXPECT_EQ(processOutput.speed, 10);
}

TEST(LineTrackingTest, TC_81)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.linesInfo.lineMiddle = 1;
	sensorData.linesInfo.lineLeft = 1;
	sensorData.linesInfo.lineRight = 0;

	ProcessOutput processOutput = lineTrackingProcess(sensorData);

	EXPECT_EQ(processOutput.onSignal, false);
	EXPECT_EQ(round(processOutput.direction*1000)/1000, -0.333f);
	EXPECT_EQ(processOutput.speed, 10);
}

TEST(LineTrackingTest, TC_82)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.linesInfo.lineMiddle = 0;
	sensorData.linesInfo.lineLeft = 1;
	sensorData.linesInfo.lineRight = 0;

	ProcessOutput processOutput = lineTrackingProcess(sensorData);

	EXPECT_EQ(processOutput.onSignal, false);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, -0.333f);
	EXPECT_EQ(processOutput.speed, 10);
}

TEST(LineTrackingTest, TC_83)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.linesInfo.lineMiddle = 1;
	sensorData.linesInfo.lineLeft = 0;
	sensorData.linesInfo.lineRight = 1;

	ProcessOutput processOutput = lineTrackingProcess(sensorData);

	EXPECT_EQ(processOutput.onSignal, false);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, 0.333f);
	EXPECT_EQ(processOutput.speed, 10);
}

TEST(LineTrackingTest, TC_84)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.linesInfo.lineMiddle = 0;
	sensorData.linesInfo.lineLeft = 0;
	sensorData.linesInfo.lineRight = 0;

	ProcessOutput processOutput = lineTrackingProcess(sensorData);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(processOutput.direction, 0);
	EXPECT_EQ(processOutput.speed, 10);
}

TEST(LineTrackingTest, TC_85)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.linesInfo.lineMiddle = 0;
	sensorData.linesInfo.lineLeft = 0;
	sensorData.linesInfo.lineRight = 1;

	ProcessOutput processOutput = lineTrackingProcess(sensorData);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, 0.333f);
	EXPECT_EQ(processOutput.speed, 10);
}

TEST(LineTrackingTest, TC_86)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.linesInfo.lineMiddle = 0;
	sensorData.linesInfo.lineLeft = 1;
	sensorData.linesInfo.lineRight = 0;

	ProcessOutput processOutput = lineTrackingProcess(sensorData);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, -0.333f);
	EXPECT_EQ(processOutput.speed, 10);
}

TEST(LineTrackingTest, TC_87)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.linesInfo.lineMiddle = 0;
	sensorData.linesInfo.lineLeft = 1;
	sensorData.linesInfo.lineRight = 1;

	ProcessOutput processOutput = lineTrackingProcess(sensorData);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, 0);
	EXPECT_EQ(processOutput.speed, 10);
}

TEST(LineTrackingTest, TC_88)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.linesInfo.lineMiddle = 1;
	sensorData.linesInfo.lineLeft = 0;
	sensorData.linesInfo.lineRight = 0;

	ProcessOutput processOutput = lineTrackingProcess(sensorData);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(processOutput.direction, 0);
	EXPECT_EQ(processOutput.speed, 10);
}

TEST(LineTrackingTest, TC_89)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.linesInfo.lineMiddle = 1;
	sensorData.linesInfo.lineLeft = 0;
	sensorData.linesInfo.lineRight = 1;

	ProcessOutput processOutput = lineTrackingProcess(sensorData);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, 0.333f);
	EXPECT_EQ(processOutput.speed, 10);
}

TEST(LineTrackingTest, TC_90)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 0;
	sensorData.linesInfo.lineMiddle = 1;
	sensorData.linesInfo.lineLeft = 1;
	sensorData.linesInfo.lineRight = 1;

	ProcessOutput processOutput = lineTrackingProcess(sensorData);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(processOutput.direction, 0);
	EXPECT_EQ(processOutput.speed, 10);
}
TEST(HazardPreventionTest, TC_91)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 315;

	float distance = 30.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);
	
	EXPECT_EQ(processOutput.onSignal, false);
	EXPECT_EQ(processOutput.direction, 1);
	EXPECT_EQ(processOutput.speed, 0);
}

TEST(HazardPreventionTest, TC_92)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 316;

	float distance = 30.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, false);
	EXPECT_EQ(processOutput.direction, 0);
	EXPECT_EQ(processOutput.speed, 0);
}

TEST(HazardPreventionTest, TC_93)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 314;

	float distance = 30.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, false);
	EXPECT_EQ(round(processOutput.direction*1000)/1000, 0.994f);
	EXPECT_EQ(processOutput.speed, 0);
}

TEST(HazardPreventionTest, TC_94)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 1;

	float distance = 30.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, false);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, -0.994f);
	EXPECT_EQ(processOutput.speed, 0);
}

TEST(HazardPreventionTest, TC_95)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 0;

	float distance = 30.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, false);
	EXPECT_EQ(processOutput.direction, -1);
	EXPECT_EQ(processOutput.speed, 0);
}

TEST(HazardPreventionTest, TC_96)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = -1;

	float distance = 30.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, false);
	EXPECT_EQ(processOutput.direction, 0);
	EXPECT_EQ(processOutput.speed, 0);
}

TEST(HazardPreventionTest, TC_97)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 316;

	float distance = 20.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(processOutput.direction, 0);
	EXPECT_EQ(processOutput.speed, 6.667f);
}

TEST(HazardPreventionTest, TC_98)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 315;

	float distance = 20.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(processOutput.direction, 1);
	EXPECT_EQ(processOutput.speed, 6.667f);
}

TEST(HazardPreventionTest, TC_99)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 314;

	float distance = 20.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction*1000)/1000, 0.994f);
	EXPECT_EQ(round(processOutput.speed*1000)/1000, 6.667f);
}

TEST(HazardPreventionTest, TC_100)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 158;

	float distance = 20.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, 0.003f);
	EXPECT_EQ(round(processOutput.speed * 1000) / 1000, 6.667f);
}

TEST(HazardPreventionTest, TC_101)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 157;

	float distance = 20.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, -0.003f);
	EXPECT_EQ(round(processOutput.speed * 1000) / 1000, 6.667f);
}

TEST(HazardPreventionTest, TC_102)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 1;

	float distance = 20.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, -0.994f);
	EXPECT_EQ(round(processOutput.speed * 1000) / 1000, 6.667f);
}

TEST(HazardPreventionTest, TC_103)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 0;

	float distance = 20.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, -1);
	EXPECT_EQ(round(processOutput.speed * 1000) / 1000, 6.667f);
}

TEST(HazardPreventionTest, TC_104)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = -1;

	float distance = 20.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(processOutput.direction, 0);
	EXPECT_EQ(processOutput.speed, 6.667f);
}

TEST(HazardPreventionTest, TC_105)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 316;

	float distance = 10.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, 0);
	EXPECT_EQ(round(processOutput.speed * 1000) / 1000, 3.333f);
}

TEST(HazardPreventionTest, TC_106)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 315;

	float distance = 10.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, 1);
	EXPECT_EQ(round(processOutput.speed * 1000) / 1000, 3.333f);
}

TEST(HazardPreventionTest, TC_107)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 314;

	float distance = 10.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, 0.994f);
	EXPECT_EQ(round(processOutput.speed * 1000) / 1000, 3.333f);
}

TEST(HazardPreventionTest, TC_108)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 158;

	float distance = 10.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, 0.003f);
	EXPECT_EQ(round(processOutput.speed * 1000) / 1000, 3.333f);
}

TEST(HazardPreventionTest, TC_109)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 157;

	float distance = 10.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, -0.003f);
	EXPECT_EQ(round(processOutput.speed * 1000) / 1000, 3.333f);
}

TEST(HazardPreventionTest, TC_110)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 1;

	float distance = 10.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, -0.994f);
	EXPECT_EQ(round(processOutput.speed * 1000) / 1000, 3.333f);
}

TEST(HazardPreventionTest, TC_111)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = 0;

	float distance = 10.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, -1);
	EXPECT_EQ(round(processOutput.speed * 1000) / 1000, 3.333f);
}

TEST(HazardPreventionTest, TC_112)
{
	SensorData sensorData;
	sensorData.targetObjectInfo.signature = 1;
	sensorData.targetObjectInfo.posX = -1;

	float distance = 10.0f;
	ProcessOutput processOutput = hazardPreventionProcess(sensorData, distance);

	EXPECT_EQ(processOutput.onSignal, true);
	EXPECT_EQ(round(processOutput.direction * 1000) / 1000, 0);
	EXPECT_EQ(round(processOutput.speed * 1000) / 1000, 3.333f);
}
TEST(PriorityMaker, TC_113)
{
	SensorData sensorData;
	//float distance = 20.0f;
	ProcessOutput outputFromObjectProcess;
	outputFromObjectProcess.onSignal = true;

	ProcessOutput outputFromLineProcess;
	outputFromLineProcess.onSignal = true;

	ProcessOutput outputFromHazardProcess;
	outputFromHazardProcess.onSignal = true;

	ModifiedData modifiedData = priorityMaker(outputFromObjectProcess, outputFromLineProcess, outputFromHazardProcess);
	EXPECT_EQ(modifiedData.priorityCode, HAZARD_PREVENTION);
}

TEST(PriorityMaker, TC_114)
{
	SensorData sensorData;
	//float distance = 20.0f;
	ProcessOutput outputFromObjectProcess;
	outputFromObjectProcess.onSignal = true;

	ProcessOutput outputFromLineProcess;
	outputFromLineProcess.onSignal = false;

	ProcessOutput outputFromHazardProcess;
	outputFromHazardProcess.onSignal = true;

	ModifiedData modifiedData = priorityMaker(outputFromObjectProcess, outputFromLineProcess, outputFromHazardProcess);
	EXPECT_EQ(modifiedData.priorityCode, HAZARD_PREVENTION);
}

TEST(PriorityMaker, TC_115)
{
	SensorData sensorData;
	//float distance = 20.0f;
	ProcessOutput outputFromObjectProcess;
	outputFromObjectProcess.onSignal = true;

	ProcessOutput outputFromLineProcess;
	outputFromLineProcess.onSignal = true;

	ProcessOutput outputFromHazardProcess;
	outputFromHazardProcess.onSignal = false;

	ModifiedData modifiedData = priorityMaker(outputFromObjectProcess, outputFromLineProcess, outputFromHazardProcess);
	EXPECT_EQ(modifiedData.priorityCode, OBJECT_TRACKING);
}

TEST(PriorityMaker, TC_116)
{
	SensorData sensorData;
	//float distance = 20.0f;
	ProcessOutput outputFromObjectProcess;
	outputFromObjectProcess.onSignal = true;

	ProcessOutput outputFromLineProcess;
	outputFromLineProcess.onSignal = true;

	ProcessOutput outputFromHazardProcess;
	outputFromHazardProcess.onSignal = true;

	ModifiedData modifiedData = priorityMaker(outputFromObjectProcess, outputFromLineProcess, outputFromHazardProcess);
	EXPECT_EQ(modifiedData.priorityCode, HAZARD_PREVENTION);
}

TEST(PriorityMaker, TC_117)
{
	SensorData sensorData;
	//float distance = 20.0f;
	ProcessOutput outputFromObjectProcess;
	outputFromObjectProcess.onSignal = false;

	ProcessOutput outputFromLineProcess;
	outputFromLineProcess.onSignal = true;

	ProcessOutput outputFromHazardProcess;
	outputFromHazardProcess.onSignal = false;

	ModifiedData modifiedData = priorityMaker(outputFromObjectProcess, outputFromLineProcess, outputFromHazardProcess);
	EXPECT_EQ(modifiedData.priorityCode, LINE_TRACKING);
}

TEST(PriorityMaker, TC_118)
{
	SensorData sensorData;

	ProcessOutput outputFromObjectProcess;
	outputFromObjectProcess.onSignal = false;

	ProcessOutput outputFromLineProcess;
	outputFromLineProcess.onSignal = true;

	ProcessOutput outputFromHazardProcess;
	outputFromHazardProcess.onSignal = true;

	ModifiedData modifiedData = priorityMaker(outputFromObjectProcess, outputFromLineProcess, outputFromHazardProcess);
	EXPECT_EQ(modifiedData.priorityCode, HAZARD_PREVENTION);
}

TEST(PriorityMaker, TC_119)
{
	SensorData sensorData;

	ProcessOutput outputFromObjectProcess;
	outputFromObjectProcess.onSignal = false;

	ProcessOutput outputFromLineProcess;
	outputFromLineProcess.onSignal = false;

	ProcessOutput outputFromHazardProcess;
	outputFromHazardProcess.onSignal = true;

	ModifiedData modifiedData = priorityMaker(outputFromObjectProcess, outputFromLineProcess, outputFromHazardProcess);
	EXPECT_EQ(modifiedData.priorityCode, HAZARD_PREVENTION);
}

TEST(PriorityMaker, TC_120)
{
	SensorData sensorData;

	ProcessOutput outputFromObjectProcess;
	outputFromObjectProcess.onSignal = false;

	ProcessOutput outputFromLineProcess;
	outputFromLineProcess.onSignal = false;

	ProcessOutput outputFromHazardProcess;
	outputFromHazardProcess.onSignal = false;

	ModifiedData modifiedData = priorityMaker(outputFromObjectProcess, outputFromLineProcess, outputFromHazardProcess);
	EXPECT_EQ(modifiedData.priorityCode, REMOTE_OFF);
}

TEST(SetReader, TC_121)
{
	ModifiedData selectedStructure;
	selectedStructure.direction = 1;
	selectedStructure.priorityCode = HAZARD_PREVENTION;
	selectedStructure.speed = 61.0f;

	ModifiedData motorVector = setReader(selectedStructure);

	EXPECT_EQ(motorVector.direction, 0);
	EXPECT_EQ(motorVector.priorityCode, HAZARD_PREVENTION);
	EXPECT_EQ(motorVector.speed, TEMPORARY_PREV_DISTANCE);
}

TEST(SetReader, TC_122)
{
	ModifiedData selectedStructure;
	selectedStructure.direction = 1;
	selectedStructure.priorityCode = HAZARD_PREVENTION;
	selectedStructure.speed = 30.0f;

	ModifiedData motorVector = setReader(selectedStructure);

	EXPECT_EQ(motorVector.direction, 1);
	EXPECT_EQ(motorVector.priorityCode, HAZARD_PREVENTION);
	EXPECT_EQ(motorVector.speed, 30.0f);
}

TEST(SetReader, TC_123)
{
	ModifiedData selectedStructure;
	selectedStructure.direction = 0;
	selectedStructure.priorityCode = HAZARD_PREVENTION;
	selectedStructure.speed = 61.0f;

	ModifiedData motorVector = setReader(selectedStructure);

	EXPECT_EQ(motorVector.direction, 0);
	EXPECT_EQ(motorVector.priorityCode, HAZARD_PREVENTION);
	EXPECT_EQ(motorVector.speed, TEMPORARY_PREV_DISTANCE);
}

TEST(SetReader, TC_124)
{
	ModifiedData selectedStructure;
	selectedStructure.direction = -0.5f;
	selectedStructure.priorityCode = HAZARD_PREVENTION;
	selectedStructure.speed = 61.0f;

	ModifiedData motorVector = setReader(selectedStructure);

	EXPECT_EQ(motorVector.direction, 0);
	EXPECT_EQ(motorVector.priorityCode, HAZARD_PREVENTION);
	EXPECT_EQ(motorVector.speed, TEMPORARY_PREV_DISTANCE);
}

TEST(SetReader, TC_125)
{
	ModifiedData selectedStructure;
	selectedStructure.direction = 1.0f;
	selectedStructure.priorityCode = OBJECT_TRACKING;
	selectedStructure.speed = 61.0f;

	ModifiedData motorVector = setReader(selectedStructure);

	EXPECT_EQ(motorVector.direction, 0);
	EXPECT_EQ(motorVector.priorityCode, OBJECT_TRACKING);
	EXPECT_EQ(motorVector.speed, TEMPORARY_PREV_DISTANCE);
}

TEST(SetReader, TC_126)
{
	ModifiedData selectedStructure;
	selectedStructure.direction = 1.0f;
	selectedStructure.priorityCode = OBJECT_TRACKING;
	selectedStructure.speed = 60.0f;

	ModifiedData motorVector = setReader(selectedStructure);

	EXPECT_EQ(motorVector.direction, 1);
	EXPECT_EQ(motorVector.priorityCode, OBJECT_TRACKING);
	EXPECT_EQ(motorVector.speed, 60.0f);
}

TEST(SetReader, TC_127)
{
	ModifiedData selectedStructure;
	selectedStructure.direction = 1.0f;
	selectedStructure.priorityCode = OBJECT_TRACKING;
	selectedStructure.speed = 59.0f;

	ModifiedData motorVector = setReader(selectedStructure);

	EXPECT_EQ(motorVector.direction, 1);
	EXPECT_EQ(motorVector.priorityCode, OBJECT_TRACKING);
	EXPECT_EQ(motorVector.speed, 59.0f);
}

TEST(SetReader, TC_128)
{
	ModifiedData selectedStructure;
	selectedStructure.direction = 1.0f;
	selectedStructure.priorityCode = REMOTE_OFF;
	selectedStructure.speed = 61.0f;

	ModifiedData motorVector = setReader(selectedStructure);

	EXPECT_EQ(motorVector.direction, 0);
	EXPECT_EQ(motorVector.priorityCode, REMOTE_OFF);
	EXPECT_EQ(motorVector.speed, TEMPORARY_PREV_DISTANCE);
}

TEST(SetReader, TC_129)
{
	ModifiedData selectedStructure;
	selectedStructure.direction = 1.0f;
	selectedStructure.priorityCode = REMOTE_OFF;
	selectedStructure.speed = 60.0f;

	ModifiedData motorVector = setReader(selectedStructure);

	EXPECT_EQ(motorVector.direction, 1);
	EXPECT_EQ(motorVector.priorityCode, REMOTE_OFF);
	EXPECT_EQ(motorVector.speed, 60.0f);
}

TEST(SetReader, TC_130)
{
	ModifiedData selectedStructure;
	selectedStructure.direction = 1.0f;
	selectedStructure.priorityCode = REMOTE_OFF;
	selectedStructure.speed = 59.0f;

	ModifiedData motorVector = setReader(selectedStructure);

	EXPECT_EQ(motorVector.direction, 1);
	EXPECT_EQ(motorVector.priorityCode, REMOTE_OFF);
	EXPECT_EQ(motorVector.speed, 59.0f);
}

TEST(SetReader, TC_131)
{
	ModifiedData selectedStructure;
	selectedStructure.direction = 0.0f;
	selectedStructure.priorityCode = REMOTE_OFF;
	selectedStructure.speed = 61.0f;

	ModifiedData motorVector = setReader(selectedStructure);

	EXPECT_EQ(motorVector.direction, 0);
	EXPECT_EQ(motorVector.priorityCode, REMOTE_OFF);
	EXPECT_EQ(motorVector.speed, TEMPORARY_PREV_DISTANCE);
}

TEST(SetReader, TC_132)
{
	ModifiedData selectedStructure;
	selectedStructure.direction = -1.0f;
	selectedStructure.priorityCode = REMOTE_OFF;
	selectedStructure.speed = 60.0f;

	ModifiedData motorVector = setReader(selectedStructure);

	EXPECT_EQ(motorVector.direction, -1);
	EXPECT_EQ(motorVector.priorityCode, REMOTE_OFF);
	EXPECT_EQ(motorVector.speed, 60.0f);
}

TEST(SetReader, TC_133)
{
	ModifiedData selectedStructure;
	selectedStructure.direction = 1.0f;
	selectedStructure.priorityCode = REMOTE_OFF;
	selectedStructure.speed = 59.0f;

	ModifiedData motorVector = setReader(selectedStructure);

	EXPECT_EQ(motorVector.direction, 1);
	EXPECT_EQ(motorVector.priorityCode, REMOTE_OFF);
	EXPECT_EQ(motorVector.speed, 59.0f);
}
TEST(ActiveMotorInterface, test_1)
{
	ModifiedData motorVector;
	motorVector.speed = 100.0f;
	motorVector.direction = 0.1f;
	AppliedData appliedData = activeMotorInterface(motorVector);

	EXPECT_EQ(appliedData.leftWheel, REGULAR_ROTATION);
	EXPECT_EQ(appliedData.rightWheel, REGULAR_ROTATION);
	EXPECT_EQ(appliedData.appliedWheelSpeed, 10);
}
