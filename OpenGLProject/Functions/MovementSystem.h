#pragma once
#include "../Components/ComponentManager.h"
#include "glm/gtc/matrix_transform.hpp"
class MovementSystem

{
public:
    float timeelapsed =0;
    void Update(float deltaTime,glm::vec3 direction, ComponentManager<PositionComponent>& pos,  ComponentManager<MovementComponent>& speed, std::vector<Entity>& entities)
    {
        timeelapsed += deltaTime;
       
        for (int i = 0; i < entities.size() ;i++) {
            if (speed.HasComponent(entities[i].GetId())) {
            if (timeelapsed > 3)
            {
             
                glm::vec3 Direction = direction - pos.GetComponent(entities[i].GetId()).GetPosition();
                Direction = glm::normalize(Direction);
                speed.GetComponent(entities[i].GetId()).SetVelocity(Direction * speed.GetComponent(entities[i].GetId()).GetSpeed());
                
            }
           
                pos.GetComponent(entities[i].GetId()).SetPosition(
                    pos.GetComponent(entities[i].GetId()).GetPosition() +=
                    speed.GetComponent(entities[i].GetId()).GetVelocity() * deltaTime);
            }
            //MatrixCalc(pos.GetComponent(entities[i].GetId()).GetPosition());

           
        }
        if (timeelapsed > 3)
            timeelapsed = 0;

    }

    glm::mat4 MatrixCalc(glm::vec3 pos)
    {
			
            glm::mat4 Actor = glm::mat4(1.f);
           Actor =  glm::translate(Actor, pos);
			
        return Actor;
    }
};

