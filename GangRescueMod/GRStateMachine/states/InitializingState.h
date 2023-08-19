#pragma once
#include "GangRescueState.h"

class InitializingState: public GangRescueState
{
private:
    void read_frequency_from_ini_file(GRStateManager& machine);
    void set_minimum_numer_of_enemies_from_ini_file(GRStateManager& machine);
    void set_maximum_number_of_enemies_from_ini_file(GRStateManager& machine);
    void set_distance_from_sheriff_office_from_ini_file(GRStateManager& machine);
    void set_member_try_to_free_hostage_from_ini_file(GRStateManager& machine);
    void set_spawn_point_distance(GRStateManager& machine);
public:
    void enter(GRStateManager& machine) override {}
    void exit(GRStateManager& machine) override {}
    void update(GRStateManager& machine) override;
    
    
    
};
