#pragma once
#include <map>
#include <memory>
#include <vector>

#include "MapArea.h"

using namespace std;

class MapAreasManager
{
private:
	map<ETownName, MapArea*> cache;
	MapAreasManager();												// Should be private
	MapAreasManager(const MapAreasManager&) = delete;				// Delete copy constructor
	MapAreasManager(const MapAreasManager&&) = delete;				// Delete move constructor
	MapAreasManager& operator=(const MapAreasManager&) = delete;	// Remove assignment operator
	
public:
	
	static MapAreasManager& get_instance()
	{
		static MapAreasManager instance;  // Garantisce che verrà creato solo una volta
		return instance;
	}
	
	MapArea* get_map_area(ETownName areaId);
	vector<MapArea*> get_map_areas();

	static Hash get_map_zone_at_pos(Vector3 position);
	static Hash get_map_zone_at_pos(Vector3 position, EZoneType zoneType);

	static Hash get_map_zone_at_pos(float x, float y, float z);
	static Hash get_map_zone_at_pos(float x, float y, float z, EZoneType zoneType);
	
	
private:
	MapArea* createBlackwater();
	MapArea* createRhodes();
	MapArea* createValentine();
	MapArea* createStrawberry();
	MapArea* createTumbleweed();
	MapArea* createArmadillo();
	MapArea* createSaintDenis();
};