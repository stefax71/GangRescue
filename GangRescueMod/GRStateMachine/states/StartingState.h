#pragma once
#include "GangRescueState.h"

class StartingState: public GangRescueState
{
    std::vector<Vector3, std::allocator<Vector3>> generate_random_positions_at_player_distance(Vector3 starting_position, float distance_from_position, int number_of_position_to_generate);
    void spawn_gang_member(Vector3 vector3, GRStateManager* machine);
    void spawn_enemies(GRStateManager& machine, int members_to_create);
    void show_subtitle_when_attack_starts(const GRStateManager& machine);
    void activate_enemies(GRStateManager& machine);

public:
    void enter(GRStateManager& machine) override {}
    void exit(GRStateManager& machine) override {}

    void update(GRStateManager& machine) override;
};
