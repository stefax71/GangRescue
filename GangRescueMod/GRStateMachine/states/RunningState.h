#pragma once
#include "GangRescueState.h"

class RunningState: public GangRescueState
{
private:
    mutex running_mutex_;
    game::RdrPed* rescuer_ = nullptr;
    Ped rescuer_mount_ = NULL;
    bool is_mounting_ = false;
    int saved_relationship_between_lawmen_and_gang = 0;
    int saved_relationship_between_gang_and_lawmen_ = 0;
    void remove_dead_enemies(GRStateManager& machine);
    void make_all_injured_enemies_flee(GRStateManager& machine);
    void remove_all_fled_enemies(GRStateManager& machine);
    void perform_free_hostage_task_sequence(GRStateManager& machine, int handle);
    void free_hostage_if_possible(GRStateManager& machine);
    void hostage_check_if_free(GRStateManager& machine);
    
    void reset_variables_to_default();
    void log_relationship(RdrLogger* logger, const char* group_one, const char* group_two, int saved_relationship_between_lawmen_and_gang);
public:
    void enter(GRStateManager& machine) override;
    bool hostage_is_in_jail(GRStateManager& machine);
    void force_destroy_all_peds(GRStateManager& machine);
    void update(GRStateManager& machine) override;
    void exit(GRStateManager& machine) override;



};

