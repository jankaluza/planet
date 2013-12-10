#include "LodManager.h"
#include <vector>

using namespace Planet2;
using namespace std;

//template <class T>
LodManager::LodManager(void) : _current(0)
{
}

//template <class T>
LodManager::~LodManager(void)
{
}

//template <class T>
void LodManager::AddModel(Model* item, float minDistance, float maxDistance)
{
	//vlozit za prvek, ktery ma max <= min
	vector<LodItem>::iterator it = _data.begin();	
	for (it = _data.begin(); it != _data.end() && it->MaxDistance > minDistance; it++);

	it = _data.insert(it, LodItem(item, minDistance, maxDistance));

	_current = &(*it);
}

//template <class T>
Model* LodManager::GetModelAt(float distance)
{	
	if (!_current)
		return 0;

	if (_current->IsInRange(distance))
		return _current->Item;

	//Najít jiný vhodný model	
	vector<LodItem>::iterator it = _data.begin();	
	for (it = _data.begin(); it != _data.end() && !it->IsInRange(distance); it++);

	//Todo: pozor -> pokud se dostaneme mimo hranice, 
	//1. jednak bude hodnota NULL
	//2. pri dalsim vykresleni se bude prohledavat cele pole znova -> lze to nejak optimalizovat?
	if (it == _data.end())
		return 0;

	_current = &(*it);

	return _current->Item;
}
