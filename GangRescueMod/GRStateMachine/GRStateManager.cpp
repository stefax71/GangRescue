#include "GRStateManager.h"

#include "states/CompletedState.h"
#include "states/InitializingState.h"
#include "states/PendingCompleteState.h"
#include "states/RunningState.h"
#include "states/StartingState.h"
#include "states/WaitingState.h"

string GRStateManager::INITIALIZING =  "INITIALIZING";
string GRStateManager::WAITING  = "WAITING";
string GRStateManager::STARTING = "STARTING";
string GRStateManager::RUNNING  = "RUNNING";
string GRStateManager::PENDING_COMPLETE = "PENDING_COMPLETE";
string GRStateManager::COMPLETED  = "COMPLETED";

std::unordered_map<std::string, std::shared_ptr<GangRescueState>> registered_states = {
    {GRStateManager::INITIALIZING, std::make_shared<InitializingState>()},
    {GRStateManager::WAITING, std::make_shared<WaitingState>()},
    {GRStateManager::STARTING, std::make_shared<StartingState>()},
    {GRStateManager::RUNNING, std::make_shared<RunningState>()},
    {GRStateManager::PENDING_COMPLETE, std::make_shared<PendingCompleteState>()},
    {GRStateManager::COMPLETED, std::make_shared<CompletedState>()}
};

GRStateManager::GRStateManager(game::RdrPlayer& player)
{
    player_ = &player;
    logger_ = RdrLogger::get_logger("GangRescue.log", LogLevel::INFO);
    transition(INITIALIZING);
}

void GRStateManager::transition(std::string new_state)
{
    try
    {
        stringstream log;
        log << "Entering " << new_state << " state";
        logger()->info(log.str().c_str());        
        transition(registered_states.at(new_state).get());
    }
    catch (const std::out_of_range&)
    {
        stringstream error_log;
        error_log << "Status " << new_state << " has not been registered";
        logger()->fatal(error_log.str().c_str());
    }
}

void GRStateManager::transition(GangRescueState* new_state)
{
    if (current_state_class_ != nullptr)
    {
        current_state_class_->exit(*this);
    }
    current_state_class_ = new_state;
    current_state_class_->enter(*this);
}

void GRStateManager::update()
{
    current_state_class_->update(*this);
}

void GRStateManager::reset_state_machine()
{
    hostage_ = nullptr;
    current_area_ = nullptr;
    gang_ = 0;
}

//********************************************************************************************************************
// GETTERS AND SETTERS, useless code
//********************************************************************************************************************

RdrLogger* GRStateManager::logger() const
{
    return logger_;
}

float GRStateManager::event_frequency() const
{
    return event_frequency_;
}

void GRStateManager::set_event_frequency(float event_frequency)
{
    event_frequency_ = event_frequency;
}


int GRStateManager::get_minimum_number_of_enemies() const
{
    return minimum_number_of_enemies;
}

void GRStateManager::set_minimum_number_of_enemies(const int minimum_number_of_enemies)
{
    this->minimum_number_of_enemies = minimum_number_of_enemies;
}

int GRStateManager::get_maximum_number_of_enemies() const
{
    return maximum_number_of_enemies;
}

void GRStateManager::set_maximum_number_of_enemies(const int maximum_number_of_enemies)
{
    this->maximum_number_of_enemies = maximum_number_of_enemies;
}

float GRStateManager::get_distance_to_trigger_event() const
{
    return distance_to_trigger_event;
}

void GRStateManager::set_distance_to_trigger_event(const float distance_to_trigger_event)
{
    this->distance_to_trigger_event = distance_to_trigger_event;
}

game::RdrPlayer* GRStateManager::get_player() const
{
    return player_;
}

MapArea* GRStateManager::get_current_area() const
{
    return current_area_;
}

void GRStateManager::set_current_area(MapArea* const current_area)
{
    current_area_ = current_area;
}

game::RdrPed* GRStateManager::get_hostage() const
{
    return hostage_;
}

void GRStateManager::set_hostage(game::RdrPed* const hostage)
{
    hostage_ = hostage;
}

Hash GRStateManager::get_gang() const
{
    return gang_;
}

void GRStateManager::set_gang(const Hash gang)
{
    gang_ = gang;
}

vector<game::RdrPed*>& GRStateManager::get_enemies()
{
    return enemies;
}

void GRStateManager::add_enemy(game::RdrPed* enemy)
{
    this->enemies.push_back(enemy);
}

bool GRStateManager::is_members_try_to_free_hostage() const
{
    return members_try_to_free_hostage_;
}

void GRStateManager::set_members_try_to_free_hostage(const bool members_try_to_free_hostage)
{
    members_try_to_free_hostage_ = members_try_to_free_hostage;
}

float GRStateManager::get_spawn_point_distance() const
{
    return spawn_point_distance_;
}

void GRStateManager::set_spawn_point_distance(const float spawn_point_distance)
{
    spawn_point_distance_ = spawn_point_distance;
}

int GRStateManager::get_seconds_between_events() const
{
    return seconds_between_events;
}

void GRStateManager::set_seconds_between_events(const int seconds_between_events)
{
    this->seconds_between_events = seconds_between_events;
}
