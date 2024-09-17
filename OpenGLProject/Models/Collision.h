#pragma once
#include "../Models/Cube.h"
#include "../Models/Sphere.h"

class Collision
{
public:
	Collision();
	void CollideWithBall(std::vector<Sphere>& collisions, Sphere& sphere);
	void CollideWithWall(Cube& otheractor,Sphere& sphere);
};

