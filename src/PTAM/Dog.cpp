#include "Dog.h"


Dog::Dog()
{
	hungry.push_back(10); //virtual
	hungry.push_back(8);  //food
	hungry.push_back(6);   //jump

	position[0]=0.3;
	position[1]=0.3;
	position[2]=0;

	direction[0]=0;
	direction[1]=-1;
	direction[2]=0;

}

Dog::~Dog()
{
	//RB_StopCameras();
	//RB_ShutdownRigidBody();
}
