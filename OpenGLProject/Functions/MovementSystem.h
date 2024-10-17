#pragma once
#include "../Components/ComponentManager.h"
#include "glm/gtc/matrix_transform.hpp"
class MovementSystem

{
public:
    void Update(float deltaTime, ComponentManager<PositionComponent>& pos,  ComponentManager<MovementComponent>& speed,const std::vector<Entity>& entities)
    {
       
        for (int i = 0; i <entities.size() ;i++) {

            
            pos.GetComponent(entities[i].EntityID).SetPosition(pos.GetComponent(entities[i].EntityID).GetPosition() +=
                speed.GetComponent(entities[i].EntityID).Speed*deltaTime);

            MatrixCalc(pos.GetComponent(entities[i].EntityID).Position);


        }
    };
    glm::mat4 MatrixCalc(glm::vec3 pos)
    {
			
            glm::mat4 Actor = glm::mat4(1.f);
            glm::translate(Actor, pos);
			
        return Actor;
    }
};

