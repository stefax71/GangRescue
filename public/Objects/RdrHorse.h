#pragma once
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "RDRPed.h"
#include "../RDREnums.h"

namespace game
{
    class RdrHorse
    {
    private:
        std::string model_;
        std::shared_ptr<game::RdrPed> ped_;

    public:
        static std::vector<std::string> models;

        RdrHorse(std::string model, Vector3 position);

        RdrHorse(const std::string& model, horse::EHorseSaddles saddle, Vector3 position): RdrHorse(model, position)
        {
            PED::_APPLY_SHOP_ITEM_TO_PED(*ped_, saddle, true, false, false);
        }

        static std::string get_random_horse_model();

        operator Ped() const
        {
            return *this->ped_;
        }
    };

    
}
