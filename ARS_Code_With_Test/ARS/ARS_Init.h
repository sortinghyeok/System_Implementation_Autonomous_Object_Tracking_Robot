#pragma once

#define TEMPORARY_PREV_DISTANCE 30

struct Lines {
	int lineLeft;
	int lineMiddle;
	int lineRight;
	Lines() {
		lineLeft = 0;
		lineMiddle = 0;
		lineRight = 0;
	}
};

struct TargetObject {
	int signature;
	int posX;
	int posY;
	int width;
	int height;
	TargetObject() {
		signature = 1;
		posX = 20;
		posY = 20;
		width = 30;
		height = 30;
	}
	TargetObject(int pos_x, int pos_y) {
		signature = 1;
		posX = pos_x;
		posY = pos_y;
		width = 30;
		height = 30;
	}
};

struct SensorData {
	Lines linesInfo;
	TargetObject targetObjectInfo;
	float* fovDistances;
};

struct ProcessOutput {
	float speed;
	float direction;
	bool onSignal;
};

struct ModifiedData {
	float speed;
	float direction;
	int priorityCode;
};

struct AppliedData {
	int leftWheel;
	int rightWheel;
	float appliedWheelSpeed;
};

