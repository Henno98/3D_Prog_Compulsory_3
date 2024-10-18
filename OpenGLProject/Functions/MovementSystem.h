#pragma once
#include "../Components/ComponentManager.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../Dependencies/includes/GLFW/glfw3.h"
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
	void RecieveInput(ComponentManager<MovementComponent>& speed,ComponentManager<InputComponent>& state, std::vector<Entity>& entities, float deltatime, GLFWwindow* window )
    {

        //for (int i = 0; i < entities.size(); i++) {
        //    if (state.HasComponent(entities[i].GetId()) == true)
        //    {
        //        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //left
        //        {
        //            speed.GetComponent(entities[i].GetId()).SetVelocity(glm::vec3(0, 0, 15));

        //        }
        //        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //right
        //        {

        //            speed.GetComponent(entities[i].GetId()).SetVelocity(glm::vec3(0,0, -15));

        //        }
        //        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //back
        //        {

        //            speed.GetComponent(entities[i].GetId()).SetVelocity(glm::vec3(-15, 0, 0));

        //        }
        //        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //forward
        //        {

        //            speed.GetComponent(entities[i].GetId()).SetVelocity(glm::vec3(15, 0, 0) );

        //        }
        //        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) //back
        //        {

        //            speed.GetComponent(entities[i].GetId()).SetVelocity(glm::vec3(0, -15, 0));

        //        }
        //        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) //forward
        //        {

        //            speed.GetComponent(entities[i].GetId()).SetVelocity(glm::vec3(0, 15, 0));

        //        }
        //    }
        //}


    }
};

