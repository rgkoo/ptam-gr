#pragma once

#ifndef _DOG
#define _DOG

#include <vector>
#include "Marker.h"
using namespace std;

#define PI 3.1415927

enum GoType
{
	By_Walk,
	By_Run
};

enum ActionType
{
	Action_Bored,
	Action_Eating,
	Action_Jumping,
};

class Dog
{
private:
	vector<int> hungry;  //0:food 1:jump

public:
	Dog();
	~Dog();

	float direction[3];	//朝向参数
	float position[3];	//位置参数

	float idlemidposp[3]; //idle时小狗运动中心位置
	float idledir[3]; //idle之前小狗的运动方向
	Marker targetmarker; //targetmarker;

};

#endif