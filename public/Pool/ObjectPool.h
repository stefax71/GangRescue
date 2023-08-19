#pragma once
#include <memory>
#include <unordered_map>
#include <mutex>
#include <sstream>

#include "../Objects/RDREntity.h"
#include "../Logger/RdrLogger.h"
#include "../native/types.h"

template <typename EntityType, typename... Args>
class ObjectPool
{
private:
    std::unordered_map<std::string, std::shared_ptr<EntityType>> elements_map;
    std::mutex pool_mutex_;
    
    ObjectPool() {
        
    }

    ObjectPool(const ObjectPool&) = delete;
    
public:
    static ObjectPool& getInstance() {
        static ObjectPool instance;
        return instance;
    }
    
    template <typename... Args>
    EntityType* acquire_entity(ScrHandle entity, Args&&... args)
    {
        std::lock_guard<std::mutex> lock(pool_mutex_);
        auto* dev_log = RdrLogger::get_logger("developer.txt", LogLevel::INFO);
    
        auto it = elements_map.find(std::to_string(entity));
        if (it == elements_map.end())
        {
            it = elements_map.emplace(std::to_string(entity), std::make_shared<EntityType>(entity, std::forward<Args>(args)...)).first;
        }
    
        auto* en = it->second.get();
        std::stringstream str;
        str << "Restituisco " << en << " per la entity " << entity;
        dev_log->info(str.str().c_str());
        return en;
    }
};

