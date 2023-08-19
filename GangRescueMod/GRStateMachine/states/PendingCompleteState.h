#pragma once
#include "GangRescueState.h"

class PendingCompleteState: public GangRescueState
{
    mutex pending_complete_mutex;
    Vector3* sheriff_station;
    vector<Ped> peds;

public:
    void enter(GRStateManager& machine) override;
    void exit(GRStateManager& machine) override
    {
        
    }
    void update(GRStateManager& machine) override;
};
