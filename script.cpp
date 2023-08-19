/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#include "script.h"

#include <sstream>
#include <windows.h>

#include "public/rdr2.h"
// #include "GangRescueMod/GangRescueManager.h"
#include "GangRescueMod/GRStateMachine/GRStateManager.h"
#include "public/Objects/RdrHorse.h"
#include "public/Utility/Utils.h"
#include "public/Debug/RdrConsole.h"
#include "public/Map/MapArea.h"
#include "public/Map/MapAreasManager.h"

std::shared_ptr<game::RdrPlayer> player;
RdrLogger* rdr2_logger;
GRStateManager* manager;

std::unique_ptr<game::RdrPed> temp_ped;
RdrConsole* console = &RdrConsole::get_instance();
unordered_map<Hash, Hash> hashes;


enum class BBType
{
    BOOL = 0,
    FLOAT = 1,
    STRING = 2
};

unordered_map<std::string, BBType> bb_keys = {
    {"Cautious", BBType::FLOAT},
    {"Stealth", BBType::FLOAT},
    {"Bewildered", BBType::FLOAT},
    {"MetalDetectorDetectionValue", BBType::FLOAT},
    {"Sick", BBType::FLOAT},
    {"Stealth", BBType::FLOAT},
    {"Bewildered", BBType::BOOL},
    {"blockHeadTailAdditives", BBType::BOOL},
    {"CarryBagAndersLeft", BBType::BOOL},
    {"CarryBagAndersRight", BBType::BOOL},
    {"CarryFishingpole", BBType::BOOL},
    {"CarryManure", BBType::BOOL},
    {"Cold_Empty_Stamina", BBType::BOOL},
    {"Cold_Low_Stamina", BBType::BOOL},
    {"Cold_Stamina", BBType::BOOL},
    {"Crafting_SatchelUpgrade", BBType::BOOL},
    {"DoHerdSmack", BBType::BOOL},
    {"DoHerdWhip", BBType::BOOL},
    {"DrunkStumbleActive", BBType::BOOL},
    {"FishingRodInLeftHand", BBType::BOOL},
    {"FishingRodInRightHand", BBType::BOOL},
    {"ForceHandsOnBelt", BBType::BOOL},
    {"FreeRoamSpawn", BBType::BOOL},
    {"GRAYOUT_ITEM_DROP", BBType::BOOL},
    {"GRAYOUT_ITEM_USE", BBType::BOOL},
    {"HandsOnBelt", BBType::BOOL},
    {"Handsup", BBType::BOOL},
    {"in_a_dress", BBType::BOOL},
    {"inInspectionMode", BBType::BOOL},
    {"IsCold", BBType::BOOL},
    {"IsColdFast", BBType::BOOL},
    {"isInspectingGround", BBType::BOOL},
    {"isLowDeadEyeCoreActive", BBType::BOOL},
    {"isLowHealthCoreActive", BBType::BOOL},
    {"isLowStaminaCoreActive", BBType::BOOL},
    {"IsSearching", BBType::BOOL},
    {"IsSeductive", BBType::BOOL},
    {"IsWithinHornetsVolume", BBType::BOOL},
    {"LanternHeldHigh", BBType::BOOL},
    {"MP_Style_Casual", BBType::BOOL},
    {"MP_Style_Crazy", BBType::BOOL},
    {"mp_style_drunk", BBType::BOOL},
    {"MP_Style_EasyRider", BBType::BOOL},
    {"MP_Style_Flamboyant", BBType::BOOL},
    {"MP_Style_Greenhorn", BBType::BOOL},
    {"MP_Style_Gunslinger", BBType::BOOL},
    {"mp_style_inquisitive", BBType::BOOL},
    {"MP_Style_Refined", BBType::BOOL},
    {"MP_Style_SilentType", BBType::BOOL},
    {"MP_Style_Veteran", BBType::BOOL},
    {"NarrowLedge", BBType::BOOL},
    {"odr3_basement", BBType::BOOL},
    {"odr3_outside", BBType::BOOL},
    {"OnFootIntimidateForced", BBType::BOOL},
    {"PickUpManure", BBType::BOOL},
    {"PlayEquipGlovesFidget", BBType::BOOL},
    {"PlayEquipHatFidget", BBType::BOOL},
    {"PlayLeadin", BBType::BOOL},
    {"PlayUnequipGlovesFidget", BBType::BOOL},
    {"PlayUnequipHatFidget", BBType::BOOL},
    {"PutDownManure", BBType::BOOL},
    {"SadPassenger", BBType::BOOL},
    {"SpawnMissionIntro", BBType::BOOL},
    {"Urgent_Look_Back", BBType::BOOL},
    {"Urgent_Look_Left", BBType::BOOL},
    {"UrgentGlances", BBType::BOOL},
    {"BodyPartChained", BBType::STRING},
    {"BlockInjuredLoco", BBType::BOOL},
    {"DoAFidget", BBType::BOOL},
    {"DoBulletFlinch", BBType::BOOL},
    {"DoLowStarts", BBType::BOOL},
    {"EagleEye", BBType::BOOL},
    {"FoliageActive", BBType::BOOL},
    {"frontpassenger", BBType::BOOL},
    {"HandsOnBelt", BBType::BOOL},
    {"Handsup", BBType::BOOL},
    {"HorseLeadingActive", BBType::BOOL},
    {"InCombat", BBType::BOOL},
    {"IsAiming", BBType::BOOL},
    {"IsAimingMounted", BBType::BOOL},
    {"IsClimbExit", BBType::BOOL},
    {"IsDoingItemInteraction", BBType::BOOL},
    {"IsDrunk", BBType::BOOL},
    {"IsEnteringCover", BBType::BOOL},
    {"IsFocusTarget", BBType::BOOL},
    {"IsInInterior", BBType::BOOL},
    {"isInspecting", BBType::BOOL},
    {"IsLanding", BBType::BOOL},
    {"isLowStaminaCoreActive", BBType::BOOL},
    {"IsOnFire", BBType::BOOL},
    {"IsPlayer", BBType::BOOL},
    {"OnFootIntimidate", BBType::BOOL},
    {"onStairs", BBType::BOOL},
    {"passenger", BBType::BOOL},
    {"PedIsWearingHat", BBType::BOOL},
    {"Rider", BBType::BOOL},
    {"RiderAiming", BBType::BOOL},
    {"RiderBlockIdleVariations", BBType::BOOL},
    {"RiderDoWhip", BBType::BOOL},
    {"RiderIntensityHigh", BBType::BOOL},
    {"RideSideSaddled", BBType::BOOL},
    {"Spooked", BBType::BOOL},
    {"Tracking", BBType::BOOL},
    {"SurfaceIncline", BBType::FLOAT},
    {"Agitation", BBType::FLOAT},
    {"BulletFlinchDirection", BBType::FLOAT},
    {"cautious", BBType::FLOAT},
    {"ClimbHeight", BBType::FLOAT},
    {"Drunkness", BBType::FLOAT},
    {"Fatigue", BBType::FLOAT},
    {"Fear", BBType::FLOAT},
    {"FireDistance", BBType::FLOAT},
    {"FoliageHeight", BBType::FLOAT},
    {"Injury", BBType::FLOAT},
    {"ShuntAngle", BBType::FLOAT},
    {"ShuntForce", BBType::FLOAT},
    {"Soak", BBType::FLOAT},
    {"Strain", BBType::FLOAT},
    {"Temperature", BBType::FLOAT},
    {"Unruliness", BBType::FLOAT},
    {"WaterDepth", BBType::FLOAT},
    {"WindSpeed", BBType::FLOAT},
    {"StaminaCoreValue", BBType::FLOAT},
    {"HealthCoreValue", BBType::FLOAT},
    {"rain", BBType::FLOAT},
    {"BLOCK_CAMERA_DRUNK_SLUMP_BEHAVIOUR", BBType::BOOL},
    {"BLOCK_COFFEE_DISCARD_PROMPT", BBType::BOOL},
    {"BLOCK_COFFEE_DRINK_PROMPT", BBType::BOOL},
    {"BLOCK_STEW_DROP_PROMPT", BBType::BOOL},
    {"BLOCK_STEW_EAT_PROMPT", BBType::BOOL},
    {"BOOK_BLOCK_PAGE_NEXT", BBType::BOOL},
    {"BOOK_BLOCK_PAGE_PREV", BBType::BOOL},
    {"DoAFidget", BBType::BOOL},
    {"DoLowStarts", BBType::BOOL},
    {"FreeRoamSpawn", BBType::BOOL},
    {"GENERIC_ALCOHOL_ALLOW_CHUG_B", BBType::BOOL},
    {"GENERIC_ALCOHOL_ALLOW_CHUG_C", BBType::BOOL},
    {"GENERIC_ALCOHOL_BLOCK_AWAY_PROMPT", BBType::BOOL},
    {"GENERIC_ALCOHOL_BLOCK_CHEERS_HI_DISCARD", BBType::BOOL},
    {"GENERIC_ALCOHOL_BLOCK_CHEERS_HI_PUTAWAY", BBType::BOOL},
    {"GENERIC_ALCOHOL_BLOCK_CHEERS_LO", BBType::BOOL},
    {"GENERIC_ALCOHOL_BLOCK_CHUG_A", BBType::BOOL},
    {"GENERIC_ALCOHOL_BLOCK_CHUG_DISCARD", BBType::BOOL},
    {"GENERIC_ALCOHOL_BLOCK_CHUG_PUT_AWAY", BBType::BOOL},
    {"GENERIC_ALCOHOL_BLOCK_DISCARD_PROMPT", BBType::BOOL},
    {"GENERIC_ALCOHOL_BLOCK_PUTAWAY_PROMPT", BBType::BOOL},
    {"GENERIC_BOOK_READ_AVAILABLE", BBType::BOOL},
    {"GENERIC_BOOK_RETURN_AVAILABLE", BBType::BOOL},
    {"GENERIC_BOOK_TAKE_AVAILABLE", BBType::BOOL},
    {"GENERIC_DOCUMENT_FLIP_AVAILABLE", BBType::BOOL},
    {"GENERIC_WEAPON_CLEAN_PROMPT_AVAILABLE", BBType::BOOL},
    {"HIDE_STEW_DROP_PROMPT", BBType::BOOL},
    {"HIDE_STEW_EAT_PROMPT", BBType::BOOL},
    {"InCombatCharging", BBType::BOOL},
    {"inInspectionMode", BBType::BOOL},
    {"isInspectingGround", BBType::BOOL},
    {"isLowDeadEyeCoreActive", BBType::BOOL},
    {"isLowHealthCoreActive", BBType::BOOL},
    {"isLowStaminaCoreActive", BBType::BOOL},
    {"JOURNAL_BLOCK_CHAPTER_NEXT", BBType::BOOL},
    {"JOURNAL_BLOCK_CHAPTER_PREV", BBType::BOOL},
    {"JOURNAL_BLOCK_PAGE_NEXT", BBType::BOOL},
    {"JOURNAL_BLOCK_PAGE_PREV", BBType::BOOL},
    {"Loco::UseActionMode", BBType::BOOL},
    {"MP_CATALOGUE_BLOCK_PAGE_NEXT", BBType::BOOL},
    {"MP_CATALOGUE_BLOCK_PAGE_PREV", BBType::BOOL},
    {"PlayEquipGlovesFidget", BBType::BOOL},
    {"PlayEquipHatFidget", BBType::BOOL},
    {"PlayUnequipGlovesFidget", BBType::BOOL},
    {"PlayUnequipHatFidget", BBType::BOOL},
    {"POILookAround", BBType::BOOL},
    {"POILookDown", BBType::BOOL},
    {"POILookInto", BBType::BOOL},
    {"RequestKneeCrouch", BBType::BOOL},
    {"SHOW_COFFEE_KNEEL_DISCARD_PROMPT", BBType::BOOL},
    {"SideSaddleRider", BBType::BOOL},
    {"SpawnMissionIntro", BBType::BOOL},
    {"UseFacing", BBType::BOOL},
    {"MetalDetectorDistanceToDigSite", BBType::FLOAT},
    {"DecelerationCause", BBType::STRING},
    {"EmotionalState", BBType::STRING},
};

game::RdrHorse* ped_horse;
bool is_mounting = false;
bool unogtied = false;
bool task_started = false;
long long last_output = 0;

std::unique_ptr<game::RdrPed> other_ped;

void scripted_task()
{
    if (!task_started)
    {
        rdr2_logger->info("Freeing target");
        Vector3 pedPos = player->character->get_position_in_front_of_entity(8);
        other_ped = std::unique_ptr<game::RdrPed>(game::RdrPed::create_ped("g_m_y_uniexconfeds_01", pedPos, player->character->get_heading() * -1, false));            
        if (!unogtied)
        {
            unogtied = true;
            TASK::_TASK_CUT_FREE_HOGTIED_TARGET_PED(*other_ped, *temp_ped);
        }
        task_started = true;
    }

    // const auto current_time = std::chrono::system_clock::now();
    // const auto milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch()).count();
    // const auto elapsed = milliseconds_since_epoch - last_output;
    // if (last_output == 0 || elapsed >= 1000 && other_ped)
    // {
    //     rdr2_logger->info("***** TASKS");
    //     for (int i = 0; i <= 530; i++)
    //     {
    //         if (TASK::GET_IS_TASK_ACTIVE(*other_ped, i))
    //         {
    //             stringstream debug;
    //             debug << "Task with index " << i << " is active";
    //             rdr2_logger->info(debug.str().c_str());
    //         }
    //     }
    //
    //     if (last_output == 0)
    //     {
    //         last_output = milliseconds_since_epoch;
    //     }
    // }

    
}

void utils_get_relationships_for_ped()
{
    RdrLogger* model_logger = RdrLogger::get_logger("model_logger.log", LogLevel::INFO);
    ui::display_help_text("My sample text", 2);
        
    game::RdrEntity* targetted = player->get_target_entity();
    game::RdrPed* targetted_as_ped = new game::RdrPed(*targetted);
        
    if (targetted_as_ped)
    {
        stringstream debug_rel;

        Hash current_hash = targetted_as_ped->get_relationship();
        auto it = hashes.find(*targetted_as_ped);
        if (it == hashes.end()) {
            debug_rel << "**********************************************************";
            model_logger->info(debug_rel.str().c_str());
            debug_rel.str("");
                
            Hash model_hash = ENTITY::GET_ENTITY_MODEL(*targetted_as_ped);
            debug_rel << "The model hash is " << model_hash;
            model_logger->info(debug_rel.str().c_str());
            debug_rel.str("");
            debug_rel << "The relationship group for this ped " << targetted_as_ped << " is " << targetted_as_ped->get_relationship() << endl;
            // debug_rel << "The default relationship group for this ped " << targetted_as_ped << " is " << targetted_as_ped->get_default_relationship() << endl;
            model_logger->info(debug_rel.str().c_str());
            hashes.insert({*targetted_as_ped, current_hash});
        }            
    }
}

chrono::steady_clock::time_point lastExecutionTime;

void log_active_tasks(Ped ped)
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastExecutionTime).count();

    if (elapsedTime >= 1000)
    {
        stringstream debug;
        debug << "Active tasks are: ";
        for (int i = 0; i <= 530; i++)
        {
            if (TASK::GET_IS_TASK_ACTIVE(ped, i))
            {
                debug << " " << i << " ";
            }
        }
        rdr2_logger->info(debug.str().c_str());
        lastExecutionTime = currentTime;
        
    }
}

void create_test_ped()
{
    if (temp_ped)
    {
        // log_active_tasks(*temp_ped);
/*        AI_ATTITUDE_NEUTRAL = 0,
        AI_ATTITUDE_FRIENDLY,
        AI_ATTITUDE_WARY,
        AI_ATTITUDE_COMBATIVE,
        AI_ATTITUDE_NEVER_MET
        
        int att = PED::_GET_PED_ATTITUDE(*temp_ped, *player);
        switch (att)
        {
            case 0: rdr2_logger->info("Attitude is NEUTRAL"); break;
            case 1: rdr2_logger->info("Attitude is FRIENDLY"); break;
            case 2: rdr2_logger->info("Attitude is WARY"); break;
            case 3: rdr2_logger->info("Attitude is COMBATIVE"); break;
            case 4: rdr2_logger->info("Attitude is NEVER MET"); break;
            default: rdr2_logger->info("Attitude is UNKNOWN"); break;
        } */
    }
    if (temp_ped && !PED::IS_PED_HOGTIED(*temp_ped) && !is_mounting && task_started)
    {
        Hash status = PED::_GET_PED_BLACKBOARD_HASH(*temp_ped, "MoodName");

        stringstream dbg;
        dbg << "Status is " << status;
        
        rdr2_logger->info(dbg.str().c_str());
        rdr2_logger->info("Ped is free");
        
        
        // TASK::CLEAR_PED_TASKS_IMMEDIATELY(*temp_ped, true, true);
        // TASK::CLEAR_PED_SECONDARY_TASK(*temp_ped);
        // TASK::TASK_MOUNT_ANIMAL(*temp_ped, *ped_horse, 20000, 0, 3.0, 0, 0, 0);
        // PED::SET_PED_KEEP_TASK(*temp_ped, true);
        is_mounting = true;

        for (int i = 0; i < 10; i++)
        {
            float mot = PED::_GET_PED_MOTIVATION(*temp_ped, i, *temp_ped);
            stringstream mot_debug;
            mot_debug << "Motivation for " << i << " is " << mot;
            rdr2_logger->info(mot_debug.str().c_str());
        }
    }

    
    if (temp_ped != nullptr && temp_ped->is_dead())
    {
        rdr2_logger->info("Deleting pointer, ped is dead");
        game::RdrPed* raw = temp_ped.release();
        delete raw;
        temp_ped = nullptr;
    }
    if (is_key_just_up(VK_F10, false) && temp_ped && ped_horse && !is_mounting)
    {
        int task_id;
        is_mounting = true;
        TASK::OPEN_SEQUENCE_TASK(&task_id);
        TASK::TASK_MOUNT_ANIMAL(0, *ped_horse, -1, -1, 5.0f, (1 << 0), 0, 0);
        TASK::CLOSE_SEQUENCE_TASK(task_id);
        TASK::TASK_PERFORM_SEQUENCE(*temp_ped, task_id);
        TASK::CLEAR_SEQUENCE_TASK(&task_id);
    }
    if (is_key_just_up(VK_F9, false) && temp_ped)
    {
        // utils_get_relationships_for_ped();
        // TASK::_0x1A7D63CB1B0BB223(*temp_ped);   // <-- look in the air
        // TASK::_0xB2D15D3551FE4FAE(*temp_ped);   // <-- Boh
        // TASK::_0xDF94844D474F31E5(*temp_ped);   // <-- look around

        scripted_task();
    }
    if (is_key_just_up(VK_F12, false))
    {
        player->set_invincible(true);
        // ui::HudDisplay& display = ui::HudDisplay::get_instance();
        // display.show_subtitle(player->get_invincible() ? "Player is invincible" : "Player is NORMAL");
        // log_active_tasks(*temp_ped);
        // MapAreasManager* instance = &MapAreasManager::get_instance();
        //
        // const MapArea* map_area = instance->get_map_area(ETownName::RHODES);
        // MAP::BLIP_ADD_FOR_COORDS(game::EBlipStyles::BLIP_STYLE_OBJECTIVE, map_area->cellCoords->x, map_area->cellCoords->y, map_area->cellCoords->z);
    }
    if (is_key_just_up(VK_F11, false))
    {
        if (temp_ped == nullptr)
        {
            Vector3 position_vector3 = player->get_position();
            Vector3 pedPos = player->character->get_position_in_front_of_entity(3);

            std::stringstream ped_position;
            ped_position << "Ped position " << "X: " << pedPos.x << " Y: " << pedPos.y << " Z: " << pedPos.z;

            std::stringstream player_position;
            player_position << "Player position " << "X: " << position_vector3.x << " Y: " << position_vector3.y <<
                " Z: " << position_vector3.z;

            rdr2_logger->info(ped_position.str().c_str());
            rdr2_logger->info(player_position.str().c_str());

            temp_ped = std::unique_ptr<game::RdrPed>(
                game::RdrPed::create_ped("g_m_y_uniexconfeds_01", pedPos, player->character->get_heading() * -1, false,
                                         game::EBlipStyles::BLIP_STYLE_ENEMY));

            std::string horse_model = game::RdrHorse::get_random_horse_model();
            Vector3 horse_pos;
            horse_pos.x = pedPos.x + 8.0f;
            horse_pos.y = pedPos.y  + 8.0f;
            horse_pos.z = pedPos.z;
            ped_horse = new game::RdrHorse(horse_model, horse::EHorseSaddles::CHARRO_01_STOCK_NEW_SADDLE_000, horse_pos);
            log_active_tasks(*temp_ped);
            // temp_ped->get_ped_on_mount(*ped_horse);

            //            temp_ped->task_go_to_entity(*player->character, 1.0f, -1);
        }
    }
}

void update()
{
    // manager->update();
    manager->update();
    create_test_ped();    
}

void main()
{
    player = std::make_shared<game::RdrPlayer>();
    rdr2_logger = RdrLogger::get_logger("GangRescue.log", LogLevel::INFO);

    rdr2_logger->write_to_log("**************************");
    rdr2_logger->write_to_log("Starting module GangRescue");
    rdr2_logger->write_to_log("**************************");

    manager = new GRStateManager(*player);
    
    while (true)
    {
        update();
        WAIT(0);
    }
}

void ScriptMain()
{
    srand(GetTickCount());
    main();
}


