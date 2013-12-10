
#include "PathLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../Objects/Box.h"
#include "../Objects/Cat.h"
#include "../Objects/Planet.h"
// #include "../Objects/GroupObjectManager.h"

namespace Planet2 {

PathLoader::PathLoader(const std::string &file) : _file(file) {

}

PathLoader::~PathLoader() {
	
}

bool PathLoader::load(World *world) {
	std::ifstream source;
    source.open(_file.c_str(), std::ios_base::in);
	if (!source) {
		return false;
	}

	GroupObjectManager *boxes = new GroupObjectManager(world);

	for(std::string line; std::getline(source, line); ) {
		std::istringstream in(line);

		std::string val;
		in >> val;
		if (val == "n") {
			float lat;
			float lon;
			string dir;
			in >> lat >> lon >> dir;
			std::cout << "adding node " << lat << " " << lon << " " << dir << "\n";
			world->path.push_back(PathNode(lat, lon, dir == "<" ? -1 : dir == ">" ? 1 : 0));
		}
		else if (val == "b" || val == "p") {
			PathNode &p = world->path[world->path.size() - 2];
			PathNode &np = world->path[world->path.size() - 1];
			float x;
			in >> x;

			x = x * 0.1;

			float angle = atan2(np.lon - p.lon, np.lat - p.lat);
			float cosa = cos(angle);
			float sina = sin(angle);
			float lat = p.lat + x * cosa;
			float lon = p.lon + x * sina;
// 			std::cout << p.lat << " " << p.lon << " " << np.lat << " " << np.lon << "\n";
// 			std::cout << lat << " " << lon << " " << x << "\n";

// 			float lond = (np.lon - p.lon) * x;
// 			float latd = (np.lat - p.lat) * x;
// 			std::cout << lond << " " << latd << "\n";
// 			float lat = p.lat + latd;
// 			float lon = p.lon + lond;

			if (val == "b") {
				Box *box = new Box(world);	
				box->solid = true;
				box->rotateWithTriangle = true;
	// 			box->Rotate(box->GetUpAxis(), r.RandomD(0, PiPi));

				// Spherical to Cartesian coordinates conversion
				CVector pos = CVectorFromSpherical(lat, lon);
				box->SetPosition(pos);

				boxes->AddObject(box);
			}
			else {
				CVector from = CVectorFromSpherical(np.lat, np.lon);
				CVector to = CVectorFromSpherical(lat, lon);
				world->Catn = new Cat(world);	
				world->Catn->rotateWithTriangle = true;

				// Spherical to Cartesian coordinates conversion
				CVector pos = CVectorFromSpherical(lat, lon);
				world->Catn->SetPosition(pos);
				world->Planet->AddObject(world->Catn);
				CVector mDir = DirectionVector(from, to);
				world->Catn->RotateToViewVector(mDir);
			}
		}
	}

	world->Planet->AddObjectGroup(boxes);
	return true;
}

}
