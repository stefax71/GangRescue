#pragma once
#include "GangRescueState.h"
#include "../GRStateManager.h"
#include "../../../public/Objects/RDRPed.h"

class WaitingState: public GangRescueState
{
private:
    Hash temporary_gang = 0;
    long long last_time_calculated_chance_ = 0;
    bool entered_area_ = false;
    bool is_in_active_area_ = false;

    game::RdrPed* ped_get_carried_hostage_if_valid(game::RdrPed* ped);
    game::RdrPed* update_fsm_with_hostage_data(GRStateManager& machine);
    bool is_event_ready() const;
    void update_last_time_calculated_chance();
    [[nodiscard]] double generate_chance();
    [[nodiscard]] bool chance_below_threshold(GRStateManager& machine);
    bool set_map_area_from_player_position(GRStateManager& machine);
    static float distance_from_sheriff_office(GRStateManager& machine);
    bool player_entered_trigger_area(GRStateManager& machine);

public:
    void enter(GRStateManager& machine) override;
    void exit(GRStateManager& machine) override {}
    
    void update(GRStateManager& machine) override;
    
};
