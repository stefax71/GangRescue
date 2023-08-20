#include "StartingState.h"

#include "RunningState.h"
#include "public/Math/MathUtils.h"
#include "public/Objects/RdrHorse.h"
#include "public/World/Gang.h"
#include "public/World/world.h"
#include "public/UI/UISubTitle.h"

void StartingState::update(GRStateManager& machine)
{
    int members_to_create = random_number_in_range(machine.get_minimum_number_of_enemies(), machine.get_maximum_number_of_enemies());
    stringstream debug;
    debug << "Spawning " << members_to_create << " gang members";
    machine.logger()->info(debug.str().c_str());
    debug.str("");

    spawn_enemies(machine, members_to_create);
    activate_enemies(machine);
    show_subtitle_when_attack_starts(machine);
    
    machine.transition(GRStateManager::RUNNING);
}

void StartingState::spawn_enemies(GRStateManager& machine, int members_to_create)
{
    stringstream debug;
    vector<Vector3> positions = generate_random_positions_at_player_distance(machine.get_player()->get_position(), machine.get_spawn_point_distance(), members_to_create);
    for (int i = 0; i < members_to_create; i++)
    {
        debug.str("");
        debug << "***** GENERATING GANG MEMBER #" << (i + 1) << " *****";
        machine.logger()->info(debug.str().c_str());
        spawn_gang_member(positions[i], &machine);
    }
}

void StartingState::show_subtitle_when_attack_starts(const GRStateManager& machine)
{
    ui::HudDisplay& display = ui::HudDisplay::get_instance();
    stringstream message;
    message << "Members of the ~COLOR_YELLOW~" << gang::Gang::get_gang_name(machine.get_gang()) << "~COLOR_WHITE~ gang are coming to save their partner!";
    display.show_subtitle(message.str().c_str());
}

void StartingState::activate_enemies(GRStateManager& machine)
{
    for (const game::RdrPed* current_enemy : machine.get_enemies())
    {
        stringstream debug;
        debug << "Activating enemy with ID " << current_enemy;
        machine.logger()->info(debug.str().c_str());
        current_enemy->task_combat_ped(machine.get_player()->character.get());
    }

}

std::vector<Vector3> StartingState::generate_random_positions_at_player_distance(Vector3 starting_position, float distance_from_position, int number_of_position_to_generate)
{
    vector<Vector3> positions;

    Vector3 position = random_position_around(starting_position, distance_from_position);
    position = OBJECT::GET_OFFSET_FROM_COORD_AND_HEADING_IN_WORLD_COORDS(position.x, position.y, position.z, CAM::GET_GAMEPLAY_CAM_ROT(2).z, 0, 35.0f * -1, 10);
    position.z = starting_position.z;
    for (int i = 0; i < number_of_position_to_generate; i++)
    {
        Vector3 new_position = random_position_around(position, 8.0f);
        positions.push_back(new_position);
        position = new_position;
    }
    return positions;
}

void StartingState::spawn_gang_member(Vector3 vector3, GRStateManager* machine)
{
    stringstream debug;
    const string model = gang::Gang::get_model_for_gang(machine->get_gang());
    const float heading = machine->get_player()->character->get_heading();

    debug << "Spawn position is " << vector_to_string(vector3);
    machine->logger()->info(debug.str().c_str());
    
    game::RdrPed* generated_enemy = game::RdrPed::create_ped(model.c_str(), vector3, heading, false, game::EBlipStyles::BLIP_STYLE_ENEMY);

    const std::string horse_model = game::RdrHorse::get_random_horse_model();
    const game::RdrHorse* horse = new game::RdrHorse(horse_model, horse::EHorseSaddles::CHARRO_01_STOCK_NEW_SADDLE_000,
                                                     vector3);

    ENTITY::PLACE_ENTITY_ON_GROUND_PROPERLY(*horse, true);
    ENTITY::PLACE_ENTITY_ON_GROUND_PROPERLY(*generated_enemy, true);
    generated_enemy->set_ped_on_mount(*horse);
    debug.str("");
    debug << "Generated enemy ID " << generated_enemy << " (" <<  (*generated_enemy) << ") with horse ID " << horse << " (" << (*horse) << ")";
    machine->logger()->info(debug.str().c_str());
    
    debug.str("");
    debug << "Real position is " << vector_to_string(generated_enemy->get_position());
    machine->logger()->info(debug.str().c_str());
    
    machine->add_enemy(generated_enemy);
}
