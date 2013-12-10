
#include "MapGenerator.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../Objects/Box.h"
#include "../Objects/Plant.h"
#include "../Objects/Plant1.h"
#include "../Objects/Planet.h"
// #include "../Objects/GroupObjectManager.h"

namespace Planet2 {

MapGenerator::MapGenerator() {

}

MapGenerator::~MapGenerator() {
	
}

bool MapGenerator::isValidPosition(World *world, float la, float lo) {
	if (points.empty()) {
		for (Path::iterator it = world->path.begin(); it != world->path.end(); ++it) {
			PathNode &p = *it;
			if ((it + 1) == world->path.end()) {
				break;
			}
			PathNode &np = *(it + 1);
			float lat = p.lat;
			float lon = p.lon;
			while(true) {
				float angle = atan2(np.lon - lon, np.lat - lat);
				float cosa = cos(angle);
				float sina = sin(angle);
				lat += 0.1 * cosa;
				lon += 0.1 * sina;

				if ((cosa > 0 && lat > np.lat) || (cosa < 0 && lat < np.lat)) {
					lat = np.lat;
				}
				if ((sina > 0 && lon > np.lon) || (sina < 0 && lon < np.lon)) {
					lon = np.lon;
				}

				points.push_back(CVectorFromSpherical(lat, lon));

				if (lat == np.lat && lon == np.lon) {
					break;
				}
			}
		}
	}

	CVector t = CVectorFromSpherical(la, lo);

	for (std::vector<CVector>::iterator it = points.begin(); it != points.end(); ++it) {
		CVector &v = *it;

		if (v.Distance(t) < 0.09) {
			return false;
		}
	}

	return true;
}

void MapGenerator::generatePlants(World *world) {
	CRandom r(666);
	float Pi = 3.14;
	float PiPi = 2 * Pi;

	GroupObjectManager *plants = new GroupObjectManager(world);

	
	for (int i = 0; i < 300; )
	{
		float lat = r.RandomD(-Pi,Pi);
		float lon = r.RandomD(0, 2*Pi);

		if (!isValidPosition(world, lat, lon)) {
			continue;
		}
		++i;

		Plant * plant = new Plant(world);
		plant->SetScale(CVector(r.RandomD(1, 10)));
		plant->Rotate(plant->GetUpAxis(), r.RandomD(0, PiPi));
		// Spherical to Cartesian coordinates conversion
		CVector pos = CVector(cos(lat) * sin(lon), sin(lat) * sin(lon), cos(lon));
		plant->SetPosition(pos);

		plants->AddObject(plant);
	}

	world->Planet->AddObjectGroup(plants);
}

void MapGenerator::generateCactis(World *world) {
	CRandom r(777);
	float Pi = 3.14;
	float PiPi = 2 * Pi;

	GroupObjectManager *cactis = new GroupObjectManager(world);
	for (int i = 0; i < 300; )
	{
		float lat = r.RandomD(-Pi, Pi);
		float lon = r.RandomD(0, 2*Pi);

		if (!isValidPosition(world, lat, lon)) {
			continue;
		}
		++i;

		Plant1 * plant1 = new Plant1(world);
		plant1->SetScale(r.RandomD(2, 6));
		plant1->Rotate(plant1->GetUpAxis(), r.RandomD(0, PiPi));
		// Spherical to Cartesian coordinates conversion
		CVector pos = CVectorFromSpherical(lat, lon);
		plant1->SetPosition(pos);

		cactis->AddObject(plant1);
	}
	world->Planet->AddObjectGroup(cactis);
}

void MapGenerator::generate(World *world) {
	Path &path = world->path;

	generatePlants(world);
	generateCactis(world);
}

}
