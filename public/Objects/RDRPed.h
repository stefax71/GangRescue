#pragma once

#include <memory>

#include "RDREntity.h"
#include "../RDREnums.h"
#include "../Logger/RdrLogger.h"


class MapArea;

enum gender
{
    MALE,
    FEMALE
};

// enum eMetaPedType
// {
// 	MPT_MALE,
// 	MPT_FEMALE,
// 	MPT_TEEN,
// 	MPT_ANIMAL,
// 	MPT_NONE
// };

namespace game
{
    class RdrPed : public RdrEntity
    {
    private:
        Ped handle_;
        Blip blip_ = 0;
        RdrPed* mount_;
        bool is_fleeing = false;

    public:
        RdrPed(const Ped handle): RdrEntity(handle)
        {
            this->handle_ = handle;
            this-> blip_ = MAP::GET_BLIP_FROM_ENTITY(handle);
        }
        
        static void request_model(Hash model); // Move somewhere else?
        static void release_model(Hash model);

        static RdrPed* create_ped(const char* modelName, Vector3 pos, float heading, bool visible = true,
                                  EBlipStyles blip = EBlipStyles::BLIP_STYLE_NONE);
        static RdrPed* create_ped_new(const char* model, Vector3 position, float heading, Hash group_hash = -1,
                                  int combat_ability = 1, ECombatMovement combat_movement = ECM_OFFENSIVE,
                                  BOOL keep_doing_task = true, BOOL block_temporary_events = false,
                                  const char* weapon_name = nullptr, bool drop_weapon_on_death = true);

        FORCEINLINE Ped get_original_handle() const { return  handle_; }
        FORCEINLINE int get_ped_type() const { return PED::_GET_META_PED_TYPE(*this); }
        void safely_delete_ped();


        bool is_carrying_something() const;
        std::shared_ptr<RdrEntity> get_carried_entity() const;

        // Task related functioons
        void task_go_to_entity(RdrEntity* target, float distance, int duration = -1, float speed = 3.0f) const;
        void task_combat_ped(RdrPed* target) const;

        // -------------- Relationships
        FORCEINLINE Hash get_relationship() const { return PED::GET_PED_RELATIONSHIP_GROUP_HASH(handle_); }
        FORCEINLINE Hash get_default_relationship() const { return PED::GET_PED_RELATIONSHIP_GROUP_DEFAULT_HASH(handle_); }
        FORCEINLINE bool is_injured() const { return PED::IS_PED_INJURED(handle_); }
        FORCEINLINE bool is_fatally_injured() const { return PED::IS_PED_FATALLY_INJURED(handle_); }
        
        void set_relationship(const char* gang) const;
        Hash get_ped_outlaws_gang() const;

        MapArea* get_area_from_position() const;

        // -------------- Mount
        void set_ped_on_mount(const Ped mount, int seat_index = -1);
        RdrPed* get_current_mount();
        FORCEINLINE bool is_on_mount() const { return PED::IS_PED_ON_MOUNT(handle_); }
        FORCEINLINE void get_off_mount() { TASK::TASK_DISMOUNT_ANIMAL(0, 1, 0, 0, 0, 0);}
        FORCEINLINE RdrPed* get_mount_as_rdr_ped() const { return mount_; }

        FORCEINLINE bool is_hogtied() const { return PED::IS_PED_HOGTIED(handle_); }
        
        bool operator==(const RdrPed& other) const
        {
            return handle_ == other.handle_;
        }

        void set_blip_for_ped(EBlipStyles blip);

        operator Ped() const
        {
            return this->handle_;
        }

        bool is_ped_fleeing()
        {
            return is_fleeing;
        }

        void flee_from_location(const Vector3& vector3, float distance);
        void flee_from_ped(RdrPed* ped_to_flee_from, EFleeStyle flee_style = EFleeStyle::MAJOR_THREAT) const;
    };
}
