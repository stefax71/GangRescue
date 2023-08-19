#pragma once
#include <set>
#include <string>

#include "../../public/native/types.h"

enum class EZoneName
{
    AMBARINO = 0xF2D2E4A4,
    GUARMA = 0x7356B8ED,
    LEMOYNE = 0x00A55D60,
    LOWER_WEST_ELIZABETH = 0x8A606F74,
    NEW_AUSTIN = 0x79E6A26B,
    NEW_HANOVER = 0xB32957A3,
    NUEVO_PARAISO = 0x2496DFE4,
    UPPER_WEST_ELIZABETH = 0xB5AA074D,
    WEST_ELIZABETH = 0x4A4BFEE7,
};

enum class ETownName
{
    AGUASDULCES_FARM = 0x62A26859,
    AGUASDULCES_RUINS = 0x0BFD6F0A,
    AGUASDULCES_VILLA = 0xB80C99B7,
    ANNESBURG = 0x00704B67,
    ARMADILLO = 0xD39FE932,
    BEECHERS_HOPE = 0x9A2C155A,
    BLACKWATER = 0x3EC4B1F5,
    BRAITHWAITE = 0x6A07DAD2,
    BUTCHER = 0x8BECCBA3,
    CALIGA = 0x6F0248BE,
    CORNWALL = 0x91A7512E,
    EMERALD = 0xE3CDCF62,
    LAGRAS = 0x183CA5FA,
    MANICATO = 0x4D704A4B,
    MANZANITA = 0x57350B23,
    RHODES = 0x79FF6291,
    SISKA = 0x7FFE0593,
    ST_DENIS = 0xD25EC74F,
    STRAWBERRY = 0x197DEE02,
    TUMBLEWEED = 0xA51AF855,
    VALENTINE = 0x1B6880B3,
    VAN_HORN = 0x7EBD16BD,
    WALLACE = 0xCBFCD82E,
    WAPITI = 0x632572AF,
};

// EZoneType.STATE does NOT work with the provided native
enum class EZoneType: int
{
    STATE,
    TOWN,
    LAKE,
    RIVER,
    OIL_SPILL,
    SWAMP,
    OCEAN,
    CREEK,
    POND,
    GLACIER,
    DISTRICT,
    TEXT_PRINTED,
    TEXT_WRITTEN
};

enum class MapAreas
{
    Blackwater = 1,
    Rhodes,
    Valentine,
    SaintDenis,
    Strawberry,
    Annesburg,
    Tumbleweed,
    Armadillo
};

class MapArea
{
private:
    std::set<int> areaMissionIds;

public:
    const char* name;
    EZoneName zone_hash_;
    ETownName town_hash_;
    
    Vector3* policeDeptCoords;
    Vector3* bountyPostersCoords;
    Vector3* cellCoords;
    Vector3* moneyCoords;
    int cellnumber;

    MapArea(const char* name, EZoneName zone_name, ETownName town_name, Vector3 policeDeptCoords, Vector3 bountyPostersCoords, Vector3 moneyCoords,
            Vector3 cellCoords, int cellnumber, float bountyPosterHeading = 0);
};
