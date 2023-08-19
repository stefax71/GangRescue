#pragma once
#include "GangRescueState.h"

class CompletedState: public GangRescueState
{
private:
    std::chrono::steady_clock::time_point reset_timer_;

public:
    void enter(GRStateManager& machine) override;
    void exit(GRStateManager& machine) override {}
    void update(GRStateManager& machine) override;
};
