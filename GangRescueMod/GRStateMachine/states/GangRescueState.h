#pragma once
#include "../GRStateManager.h"

class GangRescueState
{
public:
    virtual ~GangRescueState() = default;
    
    virtual void enter(GRStateManager& machine) {}
    virtual void exit(GRStateManager& machine) {}
    virtual void update(GRStateManager& machine) = 0;
};
