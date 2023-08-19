#pragma once

#include "../native/types.h"
#include "../native/natives.h"
#include "../Pool/ObjectPool.h"
#include <memory>
#include "RDRPed.h"

namespace game
{
    class RdrPlayer
    {
        Player handle_ = 0;
        RdrEntity* current_target;
    
    public:
        std::shared_ptr<RdrPed> character;
        std::shared_ptr<RdrPed> horse;

        RdrPlayer()
        {
            this->handle_ = PLAYER::PLAYER_ID();
            this->character = std::make_shared<RdrPed>(get_ped());
            Ped horse_for_player = get_horse();
            if (horse_for_player > 0)
            {
                this->horse = std::make_shared<RdrPed>(horse_for_player);
            }
        }

        FORCEINLINE unsigned int get_id()
        {
            return this->handle_ > 0 ? handle_ : this->handle_ = PLAYER::PLAYER_ID();
        }

        FORCEINLINE Ped get_ped() const
        {
            return PLAYER::GET_PLAYER_PED(handle_);
        }

        FORCEINLINE Ped get_horse() const
        {
            return PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(handle_);
        }

        
        FORCEINLINE void set_invincible(const bool flag) const
        {
            PLAYER::SET_PLAYER_INVINCIBLE(handle_, flag);
        }

        FORCEINLINE bool get_invincible() const
        {
            return PLAYER::GET_PLAYER_INVINCIBLE(handle_);
        }

        RdrEntity* get_entity_is_free_aiming_at() const;

        RdrEntity* get_target_entity();

        FORCEINLINE bool is_targeting_anything() const
        {
            return PLAYER::IS_PLAYER_TARGETTING_ANYTHING(handle_);
        }

        Vector3 get_position() const;
    
        operator Player() const
        {
            return handle_;
        }
    };
}
