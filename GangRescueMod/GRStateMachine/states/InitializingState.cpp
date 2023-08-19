#include "InitializingState.h"
#include <iostream>
#include <sstream>
#include <filesystem>

#include "WaitingState.h"


void InitializingState::read_frequency_from_ini_file(GRStateManager& machine)
{
    auto logger = machine.logger();
    stringstream debug_config;
    const int val = GetPrivateProfileInt(TEXT("General"), TEXT("event_chance"),30, TEXT(".\\GangRescue.ini"));
    machine.set_event_frequency(static_cast<float>(val) / 100.0f);
    debug_config << "Frequency set to " << val << "%, which translates into " << machine.event_frequency();
    logger->info(debug_config.str().c_str());
}

void InitializingState::set_minimum_numer_of_enemies_from_ini_file(GRStateManager& machine)
{
    auto logger = machine.logger();
    stringstream debug_config;
    machine.set_minimum_number_of_enemies(GetPrivateProfileInt(TEXT("General"),TEXT("minimum_enemies"),2,TEXT(".\\GangRescue.ini")));
    debug_config << "Minimum number of gang members spawning set to  " << machine.get_minimum_number_of_enemies();
    logger->info(debug_config.str().c_str());
}

void InitializingState::set_maximum_number_of_enemies_from_ini_file(GRStateManager& machine)
{
    auto logger = machine.logger();
    stringstream debug_config;
    machine.set_maximum_number_of_enemies(GetPrivateProfileInt(TEXT("General"),TEXT("maximum_enemies"),6,TEXT(".\\GangRescue.ini")));
    debug_config << "Maximum number of gang members spawning set to  " << machine.get_maximum_number_of_enemies();
    logger->info(debug_config.str().c_str());
}

void InitializingState::set_distance_from_sheriff_office_from_ini_file(GRStateManager& machine)
{
    auto logger = machine.logger();
    stringstream debug_config;
    debug_config.str("");
    TCHAR buffer[256];
    GetPrivateProfileString(TEXT("General"),TEXT("distance_from_sheriff_office"), TEXT("100.0"), buffer, sizeof(buffer), TEXT(".\\GangRescue.ini"));
    try {
        machine.set_distance_to_trigger_event(std::stof(buffer));
    } catch (const std::exception& ex) {
        debug_config << "Error parsing " << buffer << " to a valid value, default 100";
        machine.set_distance_to_trigger_event(100.0f);
    } 
    debug_config << "Distance from sheriff office to trigger event set to  " << machine.get_distance_to_trigger_event();
    logger->info(debug_config.str().c_str());
}

void InitializingState::set_member_try_to_free_hostage_from_ini_file(GRStateManager& machine)
{
    auto logger = machine.logger();
    stringstream debug_config;
    int val = GetPrivateProfileInt(TEXT("General"),TEXT("members_try_to_free_hostage"),0,TEXT(".\\GangRescue.ini"));
    bool member_try_to_free_hostage = val == 1 ? true : false;
    machine.set_members_try_to_free_hostage(member_try_to_free_hostage);
    debug_config << "Experimental feature of members freeing hostage is  " << (member_try_to_free_hostage ? " enabled" : "disabled");
    logger->info(debug_config.str().c_str());
}

void InitializingState::set_spawn_point_distance(GRStateManager& machine)
{
    stringstream debug_config;
    debug_config.str("");
    auto logger = machine.logger();
    
    TCHAR buffer[256];
    GetPrivateProfileString(TEXT("General"),TEXT("spawn_point_distance_from_player"), TEXT("150.0"), buffer, sizeof(buffer), TEXT(".\\GangRescue.ini"));
    try {
        machine.set_spawn_point_distance(std::stof(buffer));
    } catch (const std::exception& ex) {
        debug_config << "Error parsing " << buffer << " to a valid value, default 150.5";
        machine.set_spawn_point_distance(100.0f);
    }
    debug_config << "Spawn point distance is set to  " << machine.get_spawn_point_distance();
    logger->info(debug_config.str().c_str());
}

void InitializingState::update(GRStateManager& machine)
{
    read_frequency_from_ini_file(machine);
    set_minimum_numer_of_enemies_from_ini_file(machine);
    set_maximum_number_of_enemies_from_ini_file(machine);
    set_distance_from_sheriff_office_from_ini_file(machine);
    set_spawn_point_distance(machine);
    set_member_try_to_free_hostage_from_ini_file(machine);
    machine.transition(GRStateManager::WAITING);
}
