#pragma once
#pragma once

#include "../native/types.h"
#include "../native/natives.h"

namespace game
{
    class RdrEntity
    {
    private:
        Entity handle_;

    protected:

    public:
        RdrEntity(Entity h)
        {
            this->handle_ = h;
        }

        Vector3 get_position() const;
    
        Vector3 get_position_in_front_of_entity(float distance);

        FORCEINLINE int get_entity() { return this->handle_; }
        FORCEINLINE bool is_ped() const { return ENTITY::IS_ENTITY_A_PED(handle_) > 0; }
        FORCEINLINE bool is_dead() const { return ENTITY::IS_ENTITY_DEAD(handle_) > 0; }
        FORCEINLINE bool is_animal() const { return ENTITY::GET_IS_ANIMAL(handle_) > 0; }
        FORCEINLINE bool is_vehicle() const { return ENTITY::IS_ENTITY_A_VEHICLE(handle_) > 0; }
        FORCEINLINE float get_heading() const { return ENTITY::GET_ENTITY_HEADING(handle_); }
        static RdrEntity* create_entity_from_handle(Entity entity_handle);

        bool operator==(const RdrEntity& other) const
        {
            return handle_ == other.handle_;
        }
    
        operator Entity() const
        {
            return this->handle_;
        }
    
    
    };
    
}
