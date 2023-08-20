#pragma once
#include <vector>


#include "public/Objects/RDRPed.h"
#include "public/Objects/RDRPlayer.h"

class MapAreasManager;
class GangRescueState;

enum class EGRStatus
{
    INITIALIZING,
    WAITING,
    RUNNING,
    COMPLETED
};


class GRStateManager
{
private:
    GangRescueState* current_state_class_ = nullptr;
    RdrLogger* logger_;
    game::RdrPlayer* player_;

    game::RdrPed* hostage_;
    MapArea* current_area_ = nullptr;
    Hash gang_ = 0;

    vector<game::RdrPed*> enemies;
    // Event specific parameters
    float event_frequency_ = {1.0f};
    int minimum_number_of_enemies = {1};
    int maximum_number_of_enemies = {4};
    float distance_to_trigger_event = 100.0f;
    bool members_try_to_free_hostage_ = false;
    float spawn_point_distance_ = 60.0f;
    int seconds_between_events = {60};

public:
    static string INITIALIZING;
    static string WAITING;
    static string STARTING;
    static string RUNNING;
    static string PENDING_COMPLETE;
    static string COMPLETED;
    
    GRStateManager(game::RdrPlayer &player);

    void transition(std::string new_state);

    // void transition(std::string new_state);

    void transition(GangRescueState* new_state);

    void update();

    void reset_state_machine();

    // Getters and setters

    RdrLogger* logger() const;

    float event_frequency() const;

    void set_event_frequency(float event_frequency);

    int get_minimum_number_of_enemies() const;

    void set_minimum_number_of_enemies(const int minimum_number_of_enemies);

    int get_maximum_number_of_enemies() const;

    void set_maximum_number_of_enemies(const int maximum_number_of_enemies);

    float get_distance_to_trigger_event() const;

    void set_distance_to_trigger_event(const float distance_to_trigger_event);

    [[nodiscard]] game::RdrPlayer* get_player() const;

    [[nodiscard]] MapArea* get_current_area() const;

    void set_current_area(MapArea* const current_area);

    [[nodiscard]] game::RdrPed* get_hostage() const;

    void set_hostage(game::RdrPed* const hostage);

    [[nodiscard]] Hash get_gang() const;

    void set_gang(const Hash gang);

    [[nodiscard]] vector<game::RdrPed*>& get_enemies();

    void add_enemy(game::RdrPed* enemy);

    [[nodiscard]] bool is_members_try_to_free_hostage() const;

    void set_members_try_to_free_hostage(const bool members_try_to_free_hostage);

    [[nodiscard]] float get_spawn_point_distance() const;

    void set_spawn_point_distance(const float spawn_point_distance);

    [[nodiscard]] int get_seconds_between_events() const;

    void set_seconds_between_events(const int seconds_between_events);
};
