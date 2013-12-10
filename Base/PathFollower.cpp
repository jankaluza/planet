
#include "PathFollower.h"
#include "../Objects/Planet.h"

using namespace Planet2;

PathFollower::PathFollower(World *world, const Path &path) : _world(world), _path(path) {
	reset();
}

PathFollower::~PathFollower() {

}

void PathFollower::reset() {
	_currentPoint = 0;
	_lat = _path[0].lat;
	_lon = _path[0].lon;
}

bool PathFollower::follow(float dt, CVector &pos, int &turnPreparation) {
	//PathNode &np0 = _path[(_currentPoint + 0) % _path.size()];
	PathNode &np1 = _path[(_currentPoint + 1) % _path.size()];
	//PathNode &np2 = _path[(_currentPoint + 2) % _path.size()];
	float speed = _speed * dt;
	
	float angle = atan2(np1.lon - _lon, np1.lat - _lat);
	float cosa = cos(angle);
	float sina = sin(angle);
	_lat += speed * cosa;
	_lon += speed * sina;

	if ((cosa > 0 && _lat > np1.lat) || (cosa < 0 && _lat < np1.lat)) {
		_lat = np1.lat;
	}
	if ((sina > 0 && _lon > np1.lon) || (sina < 0 && _lon < np1.lon)) {
		_lon = np1.lon;
	}


	//doprava = 1
	//doleva = -1

	

	turnPreparation = 0;
	if (((cosa > 0 && _lat + 0.05 > np1.lat) || (cosa < 0 && _lat - 0.05 < np1.lat)) && ((sina > 0 && _lon + 0.05 > np1.lon) || (sina < 0 && _lon - 0.05 < np1.lon))) {
		////Calc distances
		//float a1 = PathNode::Distance(np0, np1, _world->Planet->m_mapHeight.GetRadius());
		//float b1 = PathNode::Distance(np1, np2, _world->Planet->m_mapHeight.GetRadius());
		//float c1 = PathNode::Distance(np0, np2, _world->Planet->m_mapHeight.GetRadius());
		////Calc angle
		//float an = CMath::acos((a1 * a1 + b1 * b1 - c1 * c1)/(2 * a1 * b1));

		////------------------------------------------------------------------

		//CVector a = np0.ToVector();
		//float fHeight = _world->Planet->m_mapHeight.GetHeight(a);
		//a.Normalize();
		//a *= fHeight;

		//CVector b = np1.ToVector();
		//fHeight = _world->Planet->m_mapHeight.GetHeight(b);
		//b.Normalize();
		//b *= fHeight;

		//CVector c = np2.ToVector();
		//fHeight = _world->Planet->m_mapHeight.GetHeight(c);
		//c.Normalize();
		//c *= fHeight;
		//		
		//CVector ba = DirectionVector(b, a);
		//CVector bc = DirectionVector(b, c);

		//float an1 = bc.Angle(ba);

		////netusim jak ziskat znamenko ....
		////float an2 = TVector<float>::Angle(a, b, c);

		//cout << "Angle : " << an  * 180.0 / 3.14 << endl;
		//cout << "Angle1: " << an1 * 180.0 / 3.14 << endl;
		////cout << "Angle2: " << an2 * 180.0 / 3.14 << endl;

		turnPreparation = np1.dir;
	}

	bool ret = false;
	if (_lat == np1.lat && _lon == np1.lon) {
		_currentPoint = (_currentPoint + 1);
		if (_currentPoint == _path.size() - 1) {
			ret = true;
		}
	}

	pos = CVectorFromSpherical(_lat, _lon);
	return ret;
}

