#pragma once

#include <vector>

using namespace std;

namespace Planet2
{
	//todo: nestacilo by jen jedno cislo? slo by s tim nejak rozumne pracovat?

	//template <class T>
	class Model;
	class LodManager
	{
		class LodItem
		{
		public:
			Model* Item;
			float MinDistance;
			float MaxDistance;
			LodItem(Model* item, float minDistance, float maxDistance) : Item(item), MinDistance(minDistance), MaxDistance(maxDistance) {}
			inline bool IsInRange(float distance) { return distance >= MinDistance && distance <= MaxDistance; }
		};

	public:
		LodManager(void);
		virtual ~LodManager(void);

		void AddModel(Model* item, float minDistance, float maxDistance);
		Model* GetModelAt(float distance);
	private:
		LodItem* _current;	
		vector<LodItem> _data;
	};
}

