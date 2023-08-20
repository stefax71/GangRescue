#include "RunningState.h"

#include "CompletedState.h"
#include "public/Map/MapArea.h"
#include "public/Math/MathUtils.h"
#include "public/World/Gang.h"
#include "public/UI/UISubTitle.h"

void RunningState::enter(GRStateManager& machine)
{
    reset_variables_to_default();
    
    Hash gang_hash = machine.get_gang();
    if (gang_hash > 0)
    {
        int relationship_lawman_gang = PED::GET_RELATIONSHIP_BETWEEN_GROUPS(static_cast<unsigned>(game::ERelationship::LAWMAN), gang_hash);
        saved_relationship_between_lawmen_and_gang = relationship_lawman_gang;
        log_relationship(machine.logger(), "Lawman", gang::Gang::get_gang_name(gang_hash).c_str(), saved_relationship_between_lawmen_and_gang);
        int relationship_gang_lawmen = PED::GET_RELATIONSHIP_BETWEEN_GROUPS(gang_hash, static_cast<unsigned>(game::ERelationship::LAWMAN));
        saved_relationship_between_gang_and_lawmen_ = relationship_gang_lawmen;
        log_relationship(machine.logger(), gang::Gang::get_gang_name(gang_hash).c_str(), "Lawman", saved_relationship_between_lawmen_and_gang);

        PED::SET_RELATIONSHIP_BETWEEN_GROUPS(static_cast<int>(game::ERelationshipType::ACQUAINTANCE_TYPE_PED_HATE), static_cast<unsigned>(game::ERelationship::LAWMAN), gang_hash);
        PED::SET_RELATIONSHIP_BETWEEN_GROUPS(static_cast<int>(game::ERelationshipType::ACQUAINTANCE_TYPE_PED_HATE), gang_hash, static_cast<unsigned>(game::ERelationship::LAWMAN));
        machine.logger()->info("Set relationship with LAWMAN to hate");
        
        PED::SET_PED_RELATIONSHIP_GROUP_HASH(*machine.get_hostage(), static_cast<unsigned>(game::ERelationship::CRIMINAL));
        machine.logger()->info("Changed relationship group of hostage to avoid being killed by cops");
    }
}

void RunningState::reset_variables_to_default()
{
    rescuer_ = nullptr;
    rescuer_mount_ = NULL;
    is_mounting_ = false;
    saved_relationship_between_lawmen_and_gang = 0;
    saved_relationship_between_gang_and_lawmen_ = 0;
}

void RunningState::log_relationship(RdrLogger* logger, const char* group_one, const char* group_two, int saved_relationship_between_lawmen_and_gang)
{
    stringstream debug;
    debug << "Relationship between " << group_one << " and " << group_two << " with value of " << saved_relationship_between_lawmen_and_gang << " saved";
    logger->info(debug.str().c_str());
}

void RunningState::update(GRStateManager& machine)
{
    remove_dead_enemies(machine);
    if (machine.get_enemies().empty())
    {
        machine.transition(GRStateManager::COMPLETED);
    }
    // Check if player is still alive

    make_all_injured_enemies_flee(machine);

    remove_all_fled_enemies(machine);

    if (machine.is_members_try_to_free_hostage())
    {
        // Check if player is further than 50 from hostage, if so the closest enemy to the hostage tries to free hostage
        free_hostage_if_possible(machine);    
    }

    hostage_check_if_free(machine);

    if (hostage_is_in_jail(machine))
    {
        machine.get_hostage()->set_blip_for_ped(game::BLIP_STYLE_NONE);
        machine.transition(GRStateManager::PENDING_COMPLETE);
    }

    if (machine.get_hostage()->is_dead())
    {
        ui::HudDisplay& display = ui::HudDisplay::get_instance();
        stringstream message;
        message << "The hostage is  ~COLOR_RED~dead";
        display.show_subtitle(message.str().c_str());            
        machine.transition(GRStateManager::PENDING_COMPLETE);
    }

    if (machine.get_player()->character->is_dead())
    {
        Ped hostage = *machine.get_hostage();
        PED::DELETE_PED(&hostage);
        machine.logger()->info("Deleting hostage");
        delete machine.get_hostage();
        force_destroy_all_peds(machine);
        machine.transition(GRStateManager::COMPLETED);
    }
}

bool RunningState::hostage_is_in_jail(GRStateManager& machine)
{
    const Vector3 hostage_position = machine.get_hostage()->get_position();
    const Vector3* jail_position = machine.get_current_area()->cellCoords;
    const float distance_from_cell = distanceBetween(hostage_position, *jail_position);
    
    return distance_from_cell <= 2.0f;
}

void RunningState::force_destroy_all_peds(GRStateManager& machine)
{
    for (auto it = machine.get_enemies().begin(); it != machine.get_enemies().end(); )
    {
        Ped current_ped = **it;
        Ped current_horse = *(*it)->get_current_mount();
        ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&current_ped);
        ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&current_horse);

        stringstream info;
        info << "Deleting enemy " << *it << " and horse " << (*it)->get_current_mount();
        machine.logger()->info(info.str().c_str());
        
        delete (*it)->get_current_mount();
        delete *it;
        machine.get_enemies().erase(it);
    }
}

void RunningState::remove_dead_enemies(GRStateManager& machine)
{
    {
        std::lock_guard<std::mutex> lock(running_mutex_);
        for (auto it = machine.get_enemies().begin(); it != machine.get_enemies().end(); /* NO INCREMENT */)
        {
            if ((*it)->is_dead())
            {
                if ( game::RdrPed* mount = (*it)->get_mount_as_rdr_ped(); mount != nullptr)
                {
                    Ped target = *(*it);
                    Ped mount_hash = *mount;
                    delete mount;
                    TASK::TASK_SMART_FLEE_PED(mount_hash, target, 10000, 20000, 0, 3.0f, 0);
                }
                delete *it;
                it = machine.get_enemies().erase(it);
                stringstream debug;
                debug << "Removing dead enemy with ID " << (*it);
                machine.logger()->info(debug.str().c_str());
            } else
            {
                
                ++it;    
            }
        }
    }
}

void RunningState::make_all_injured_enemies_flee(GRStateManager& machine)
{
    {
        std::lock_guard<mutex> lock(running_mutex_);
        for (auto it = machine.get_enemies().begin(); it != machine.get_enemies().end(); ++it )
        {
            if ((*it)->is_injured())
            {
                stringstream debug;
                debug << "Enemy with ID " << (*it) << " is injured, fleeing";
                machine.logger()->info(debug.str().c_str());
                (*it)->flee_from_ped(machine.get_player()->character.get());
            }
        }
    }
}

void RunningState::remove_all_fled_enemies(GRStateManager& machine)
{
    {
        lock_guard<mutex> lock(running_mutex_);

        for (auto it = machine.get_enemies().begin(); it != machine.get_enemies().end(); )
        {
            const float distance_from_player = distanceBetween((*it)->get_position(), machine.get_player()->get_position());
            if ( distance_from_player > 600.0f)
            {
                (*it)->safely_delete_ped();
                delete *it;
                it = machine.get_enemies().erase(it);
                stringstream debug;
                debug << "Removing enemy that went too far " << (*it);
                machine.logger()->info(debug.str().c_str());
            } else
            {
                ++it;
            }
        }
    }
}

void RunningState::free_hostage_if_possible(GRStateManager& machine)
{
    if (rescuer_ != nullptr) return;
    
    auto hostage_location = machine.get_hostage()->get_position();
    auto player_location = machine.get_player()->get_position();

    if (distanceBetween(hostage_location, player_location) > 10.0f)
    {
        float min_distance = 100.0f;
        // Find closest enemy to hostage
        for (auto it = machine.get_enemies().begin(); it != machine.get_enemies().end(); ++it)
        {
            float current_mob_distance = distanceBetween((*it)->get_position(), hostage_location);
            if (current_mob_distance < min_distance)
            {
                rescuer_ = (*it);
                rescuer_mount_ = *rescuer_->get_current_mount();
            }
        }
        if (rescuer_ != nullptr)
        {
            int handle = *rescuer_;
            stringstream debug;
            debug << "Gang member " << rescuer_ << " is freeing hostage";
            machine.logger()->info(debug.str().c_str());
            TASK::CLEAR_PED_TASKS_IMMEDIATELY(handle, true, true);
            perform_free_hostage_task_sequence(machine, handle);

            ui::HudDisplay& display = ui::HudDisplay::get_instance();
            stringstream message;
            message << "One of the ~COLOR_YELLOW~" << gang::Gang::get_gang_name(machine.get_gang()) << "~COLOR_WHITE~ is ~COLOR_RED~freeing your hostage!";
            display.show_subtitle(message.str().c_str());            
        }
    }
}

void RunningState::hostage_check_if_free(GRStateManager& machine)
{
    if (machine.get_hostage()->is_hogtied())
    {
        return;
    }

    if (rescuer_mount_ != NULL && !is_mounting_)
    {
        is_mounting_ = true;
        TASK::TASK_MOUNT_ANIMAL(*rescuer_, rescuer_mount_, 20000, -1, 5.0f, (1 << 0), 0, 0);
        TASK::CLEAR_PED_TASKS_IMMEDIATELY(*machine.get_hostage(), true, true);
        TASK::TASK_MOUNT_ANIMAL(*machine.get_hostage(), rescuer_mount_, -1, 0, 5.0f, (1 << 0), 0, 0);
        PED::SET_PED_KEEP_TASK(*machine.get_hostage(), true);
    }
}

void RunningState::perform_free_hostage_task_sequence(GRStateManager& machine, int handle)
{
    int task_id;
    TASK::OPEN_SEQUENCE_TASK(&task_id);
    TASK::TASK_GO_TO_ENTITY(0, *machine.get_hostage(), 10000, 8.0f, 3.0f, 0, 0);
            
    if (PED::IS_PED_ON_MOUNT(0))
    {
        TASK::TASK_DISMOUNT_ANIMAL(0, 1, 0, 0, 0, 0);
    }
    // TASK::TASK_GO_TO_ENTITY(0, *machine.get_hostage(), 10000, 1.0f, 3.0f, 0, 0);            
    TASK::_TASK_CUT_FREE_HOGTIED_TARGET_PED(0, *machine.get_hostage());
    TASK::CLOSE_SEQUENCE_TASK(task_id);

    TASK::TASK_PERFORM_SEQUENCE(handle, task_id);
    TASK::CLEAR_SEQUENCE_TASK(&task_id);
}

void RunningState::exit(GRStateManager& machine)
{
    Hash gang_hash = machine.get_gang();
    if (gang_hash > 0)
    {
        PED::SET_RELATIONSHIP_BETWEEN_GROUPS(saved_relationship_between_lawmen_and_gang, static_cast<unsigned>(game::ERelationship::LAWMAN), gang_hash);
        PED::SET_RELATIONSHIP_BETWEEN_GROUPS(saved_relationship_between_gang_and_lawmen_, gang_hash, static_cast<unsigned>(game::ERelationship::LAWMAN));
        machine.logger()->info("Relationship restored");
    }
}