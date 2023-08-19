#include "CompletedState.h"

void CompletedState::enter(GRStateManager& machine)
{
    reset_timer_ = std::chrono::steady_clock::now();
}

void CompletedState::update(GRStateManager& machine)
{
    const auto now = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - reset_timer_).count();

    const long long remaining_time = machine.get_seconds_between_events() - elapsed;
    
    if (remaining_time <= 0)
    {
        machine.reset_state_machine();
        machine.transition(GRStateManager::WAITING);
    }
}
