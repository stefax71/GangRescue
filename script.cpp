/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#include "script.h"

#include <sstream>
#include "GangRescueMod/GRStateMachine/GRStateManager.h"

std::shared_ptr<game::RdrPlayer> player;
RdrLogger* rdr2_logger;
GRStateManager* manager;

void update()
{
    manager->update();
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


