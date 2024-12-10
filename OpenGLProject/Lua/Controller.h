#pragma once

extern "C"
{
#include "../Dependencies/includes/lua54/include/lua.h"
#include "../Dependencies/includes/lua54/include/lauxlib.h"
#include "../Dependencies/includes/lua54/include/lualib.h"
}
// Link to lua library
#ifdef _WIN32
#pragma comment(lib, "Dependencies/includes/lua54/lua54.lib")
#endif
#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <string>
#include <iostream>
#include <vector>

#include "../Components/ComponentManager.h"

class Controller {
private:
    lua_State* L; // Lua state
    EntityManager& EManager;
    std::vector<Entity>& AllEntities;
    // Component managers (assumed to exist in your game)
    ComponentManager<PositionComponent>& PositionData;
    ComponentManager<MeshComponent>& MeshData;
    ComponentManager<MovementComponent>& MovementData;
    ComponentManager<HealthComponent>& HealthData;
    ComponentManager<DamageComponent>& DamageData;
    ComponentManager<CollisionComponent>& CollisionData;

public:
    Controller(
        EntityManager& entityManager,
        std::vector<Entity>& entities,
        ComponentManager<PositionComponent>& positionData,
        ComponentManager<MeshComponent>& meshData,
        ComponentManager<MovementComponent>& movementData,
        ComponentManager<HealthComponent>& healthData,
        ComponentManager<DamageComponent>& damageData,
        ComponentManager<CollisionComponent>& collisionData
    ) :
        EManager(entityManager),
        AllEntities(entities),
        PositionData(positionData),
        MeshData(meshData),
        MovementData(movementData),
        HealthData(healthData),
        DamageData(damageData),
        CollisionData(collisionData)
    {
        L = luaL_newstate();
        luaL_openlibs(L); // Load Lua standard libraries
        BindLuaFunctions();
    }

    void BindLuaFunctions() {
        lua_register(L, "CreateEntity", Lua_CreateEntity);
    }

    void RunScript(const std::string& scriptPath) {
        if (luaL_dofile(L, scriptPath.c_str()) != LUA_OK) {
            std::cerr << "Error running Lua script: " << lua_tostring(L, -1) << std::endl;
        }
    }

    static int Lua_CreateEntity(lua_State* L) {
        // Retrieve `Controller` instance from Lua's global state
        Controller* controller = static_cast<Controller*>(lua_touserdata(L, lua_upvalueindex(1)));

        // Read entity type from Lua
        std::string entityType = lua_tostring(L, 1);
        int numberofentities = lua_tonumber(L, 2);
        std::cout << "Spawning entity of type: " << entityType << std::endl;
        for (int i = 0; i < numberofentities; i++) {
            // Create entity and configure components
            Entity entity = controller->EManager.CreateEntity();
            controller->AllEntities.emplace_back(entity);

            if (entityType == "Boar") {
                controller->PositionData.AddComponent(entity.GetId(), PositionComponent(glm::vec3(0, 0, 0)));
                controller->MeshData.AddComponent(entity.GetId(), MeshComponent(Sphere, glm::vec3(1.f), glm::vec3(0, 0, 0)));
                controller->MovementData.AddComponent(entity.GetId(), MovementComponent(Tracking, glm::vec3(1, 1, 1), 2.f, 2.f));
                controller->HealthData.AddComponent(entity.GetId(), HealthComponent(10));
                controller->DamageData.AddComponent(entity.GetId(), DamageComponent(2));
                controller->CollisionData.AddComponent(entity.GetId(), CollisionComponent(true, glm::vec3(1, 1, 1)));
            }
            else if (entityType == "Platform") {
                controller->PositionData.AddComponent(entity.GetId(), PositionComponent(glm::vec3(0.f, 0.f, 0.f)));
                controller->MeshData.AddComponent(entity.GetId(), MeshComponent(Cube, glm::vec3(10.f, 1.f, 10.f), glm::vec3(0, 0, 0)));
            }

            lua_pushnumber(L, entity.GetId()); // Return entity ID to Lua
        }
        return numberofentities; // Number of return values
    }

    void RegisterInstance() {
        lua_pushlightuserdata(L, this); // Push `this` pointer
        lua_pushcclosure(L, Lua_CreateEntity, 1); // Bind with upvalue
        lua_setglobal(L, "CreateEntity"); // Make it globally accessible in Lua
    }
};

#endif
