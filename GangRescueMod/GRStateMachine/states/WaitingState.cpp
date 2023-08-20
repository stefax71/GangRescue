#include "WaitingState.h"

#include <random>

#include "StartingState.h"
#include "public/Math/MathUtils.h"
#include "public/Map/MapArea.h"
#include "public/World/Gang.h"

void WaitingState::enter(GRStateManager& machine)
{
    RdrLogger* logger_ = RdrLogger::get_logger("GangRescue.log", LogLevel::INFO);
    logger_->info("Entered waiting state");
        
    temporary_gang = 0;
    last_time_calculated_chance_ = 0;
    entered_area_ = false;
    is_in_active_area_ = false;        
}

void WaitingState::update(GRStateManager& machine)
{
    entered_area_ = false;
    
    if (!set_map_area_from_player_position(machine)) return;
    entered_area_ = player_entered_trigger_area(machine);

    // Separate logic -> hostage and gang should be processed separately
    // however, if a hostage does not belong to a gang, should return nullptr
    const game::RdrPed* player_hostage = update_fsm_with_hostage_data(machine);
    if (player_hostage == nullptr) return;

    if (!gang::Gang::is_gang(machine.get_gang()))
    {
        std::stringstream info_log;
        info_log << "Hostage not null, but gang hash is " << machine.get_gang() << " that is NOT a gang";
        machine.logger()->info(info_log.str().c_str());
        return;
    }
    
    if (!entered_area_ && !is_in_active_area_)
    {
        return;
    }
    
    if (!entered_area_ && is_in_active_area_)
    {
        machine.logger()->info("Player exited the area");
        is_in_active_area_ = false;
        return;
    }
    
    is_in_active_area_ = true;

    if (chance_below_threshold(machine))
    {
        machine.transition(GRStateManager::STARTING);
    }

}

game::RdrPed* WaitingState::ped_get_carried_hostage_if_valid(game::RdrPed* ped)
{
    if (ped->is_carrying_something())
    {
        if (const game::RdrEntity* carried_entity = ped->get_carried_entity().get(); carried_entity->is_ped() && !carried_entity->is_dead())
        {
            return new game::RdrPed(*carried_entity);
        }
    }
    return nullptr;
}

game::RdrPed* WaitingState::update_fsm_with_hostage_data(GRStateManager& machine)
{
    machine.set_gang(-1);
    machine.set_hostage(nullptr);
    
    game::RdrPed* hostage = ped_get_carried_hostage_if_valid(machine.get_player()->character.get());
    if (hostage == nullptr && machine.get_player()->horse)
    {
        hostage = ped_get_carried_hostage_if_valid(machine.get_player()->horse.get());;
    }

    if (hostage != nullptr)
    {
        const Hash gang_hash = hostage->get_ped_outlaws_gang();
        if (gang_hash > 0)
        {
            machine.set_gang(gang_hash);
            machine.set_hostage(hostage);
            return hostage;
        }
    }
    return nullptr;
}

bool WaitingState::is_event_ready() const
{
    const auto current_time = std::chrono::system_clock::now();
    const auto milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch()).count();
    const auto elapsed = milliseconds_since_epoch - last_time_calculated_chance_;
    if (elapsed < 120000)
    {
        return false;
    }
    return true;
}


void WaitingState::update_last_time_calculated_chance()
{
    const auto current_time = std::chrono::system_clock::now();
    last_time_calculated_chance_ = chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch()).count();
}

double WaitingState::generate_chance()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

bool WaitingState::chance_below_threshold(GRStateManager& machine)
{
    const auto event_frequency = machine.event_frequency();

    if (last_time_calculated_chance_ > 0 && !is_event_ready())
    {
        return false;
    }
    
    update_last_time_calculated_chance();

    const double randomValue = generate_chance();
    
    stringstream debug;
    debug << "Calculated value is " << randomValue << " vs a frequency of " << event_frequency;
    machine.logger()->info(debug.str().c_str());
    return randomValue <= event_frequency;
}


bool WaitingState::set_map_area_from_player_position(GRStateManager& machine)
{
    MapArea* current_area = machine.get_player()->character->get_area_from_position();
    if (current_area == nullptr)
    {
        machine.set_current_area(nullptr);
        return false;
    }
    machine.set_current_area(current_area);
    return true;
}

float WaitingState::distance_from_sheriff_office(GRStateManager& machine)
{
    return distanceBetween(machine.get_player()->character->get_position(), *machine.get_current_area()->policeDeptCoords);
}

bool WaitingState::player_entered_trigger_area(GRStateManager& machine)
{
    return distance_from_sheriff_office(machine) <= machine.get_distance_to_trigger_event();
}