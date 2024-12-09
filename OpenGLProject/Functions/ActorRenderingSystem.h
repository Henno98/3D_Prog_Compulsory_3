#pragma once
#include "../Components/MeshComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/ComponentManager.h"
class ActorRenderingSystem
{
public:

	void UpdateMesh(MeshComponent& meshtype, PositionComponent& pos)
	{
		meshtype.Matrix = meshtype.MatrixCalc(pos.GetPosition());
		
	};

};