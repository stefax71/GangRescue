#include "PendingCompleteState.h"

#include "public/Map/MapArea.h"
#include "public/Math/MathUtils.h"

void PendingCompleteState::enter(GRStateManager& machine)
{
    {
        lock_guard<mutex> lock(pending_complete_mutex);

        sheriff_station = machine.get_current_area()->policeDeptCoords;
        machine.logger()->info("Making all the alive gang members flee");
        
        for (auto it = machine.get_enemies().begin(); it != machine.get_enemies().end(); ++it)
        {
            const Ped handle = **it;
            TASK::TASK_SMART_FLEE_COORD(handle, sheriff_station->x, sheriff_station->y, sheriff_station->z, 10000, 20000, 0, 3.0f);
        }
    }
}

void PendingCompleteState::update(GRStateManager& machine)
{
    {
        lock_guard<mutex> lock(pending_complete_mutex);

        for (auto it = machine.get_enemies().begin(); it != machine.get_enemies().end(); )
        {
            Vector3 ped_position = (*it)->get_position(); 
            float distance =  distanceBetween(ped_position, *sheriff_station);
            if (distance > 80 || (*it)->is_dead())
            {
                stringstream log;
                log << "Enemy " << (*it) << " is further than 300 from sheriff office, removing";
                machine.logger()->info(log.str().c_str());
                (*it)->safely_delete_ped();
                delete *it;
                machine.get_enemies().erase(it);
                
            } else
            {
                ++it;
            }
        }
    }
    if (machine.get_enemies().empty())
    {
        machine.transition(GRStateManager::COMPLETED);    
    }
    
}
