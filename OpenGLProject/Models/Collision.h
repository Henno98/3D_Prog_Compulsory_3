#pragma once
#include "../Models/Cube.h"
#include "../Models/Sphere.h"

class Collision
{
public:
	Collision();
	void CollideWithBall(Sphere& otheractor, Sphere& sphere);
	void CollideWithWall(Cube& otheractor,Sphere& sphere);
};

