#pragma once
#include "MovementComponent.h"
#include "PositionComponent.h"
#include "iostream"
#include "vector"
#include "unordered_map"
#include "EntityManager.h"
#include "HealthComponent.h"
#include "DamageComponent.h"
#include "CollisionComponent.h"
#include "SizeComponent.h"

struct BaseClass
{
	virtual void CreateManager()
	{
	
    
        std::cout << "[LOG]:ComponentArchive Created\n";
    
   
	};
	virtual ~BaseClass() {};


};

template <typename T>
class ComponentManager : BaseClass
{

private:
	std::unordered_map<int, std::vector<std::shared_ptr<T>>> ComponentArchive;

public:
	void CreateManager() override
	{
		std::cout << "Created Actor successfully\n";
	}
	void AddComponent(const int& EntityID, const T& Component )
	{
		if(EntityID < 0)
		{
			return;

		}
		ComponentArchive[EntityID].emplace_back(std::make_shared<T>(Component));

	}
    T& GetComponent(const int& _entityId)
    {
        try
        {
            return *ComponentArchive[_entityId].front();
        }
        catch (const std::exception& e) {
            // Catch any exception thrown during the execution
            // of divide function
            std::cerr << "Exception caught: " << e.what() << std::endl;
            //return T();
        }
    }

    bool HasComponent(const int& _entityId)
    {
        return ComponentArchive.find(_entityId) != ComponentArchive.end();
    }

    void RemoveComponent(const int& _entityId)
    {
        if (!HasComponent(_entityId))
        {
            throw std::runtime_error("[ERROR]:Component not found for the given entity ID.");
        }
        ComponentArchive.erase(_entityId);
    }

    std::unordered_map<int, std::vector<std::shared_ptr<T>>> GetAllComponents()
    {
        return ComponentArchive;
    }
};



