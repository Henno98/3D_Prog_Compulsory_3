#pragma once
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>


template <typename T>
class SystemManager {
private:
    std::unordered_map<int, std::vector<std::shared_ptr<T>>> SystemArchive;
public:
    void CreateManager() 
    {
        std::cout << "Created Actor successfully\n";
    }
    int assignedPos = 0;
    void AddComponent(const int& EntityID, const T& Component)
    {
        if (EntityID < 0)
        {
            return;

        }
        SystemArchive[EntityID].emplace_back(std::make_shared<T>(Component));

    }
    T& GetComponent(const int& _entityId)
    {
        try
        {
            return *SystemArchive[_entityId].front();
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
        return SystemArchive.find(_entityId) != SystemArchive.end();
    }

    void RemoveComponent(const int& _entityId)
    {
        if (!HasComponent(_entityId))
        {
            throw std::runtime_error("[ERROR]:Component not found for the given entity ID.");
        }
        SystemArchive.erase(_entityId);
    }

    std::unordered_map<int, std::vector<std::shared_ptr<T>>> GetAllComponents()
    {
        return SystemArchive;
    }
};


