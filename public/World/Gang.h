#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "../native/types.h"


namespace gang
{
    
    class Gang
    {
    private:
        static std::vector<Hash> gangs;
        static std::unordered_map<Hash, std::vector<std::string>> models;
    public:
        static const Hash ODRISCOLL = 2297989198;
        static const Hash DEL_LOBO  = 2631665427;
        static const Hash LEMOYNE_RIDER = 555364152;
        static const Hash MURFREE_BROOD = 1078461828;
        static const Hash SKINNER_BROTHERS = 3567255953;  
        
        static bool is_gang(Hash hash_to_search)
        {
            const auto gangs_iterator = std::find(gangs.begin(), gangs.end(), hash_to_search);
            return gangs_iterator != gangs.end();
        }

        static std::string get_gang_name(Hash hash);
        static std::string get_model_for_gang(Hash hash);
    };
    
}

