#pragma once
#include "Object.h"

namespace Planet2
{
	class World;
	class Model;

	class Hero : public Object
	{
	private:
		class LinearInterpolator
		{
		private:
			float _current;
			float _min;
			float _max;
			bool _sub;
			float _speed;
		public:
			float Get() { return _current; }
			void Update(float dt)
			{
				float xt = _speed * dt * (_sub ? -1 : 1);
				_current += xt;

				if ((_sub && _current <= _min) || (!_sub && _current >= _max))
					_sub = !_sub;
			}

			LinearInterpolator(float min, float max, float speed, float initial) : _current(initial), _min(min), _max(max), _speed(speed), _sub(true) {}
		};

	public:
		Hero(World *world);

		void beginDraw();
		void endDraw();
		void update(C3DObject *camera, float dt);
		void draw();


		bool removeLife() {
			if (_life > 0) {
				_life--;
			}
			return _life == 0;
		}

		void addLife() {
			if (_life != 3) {
				_life++;
			}
		}

		void reset() {
			_life = 3;
			Moving = false;
		}

		bool Moving;

	private:
		void drawModel(Model *model, CVector translate, CVector axis, float angle);
		void GetMatrix(CMatrix &m, CVector translate, CVector axis, float angle);

		Model* _torso;
		Model* _tail;
		LinearInterpolator _tail_li;
		LinearInterpolator _feet_li;

		Model* _leftFront;
		Model* _rightFront;
		Model* _leftRear;
		Model* _rightRear;
		//CMatrix _m;

		C3DObject *_camera;
		int _life;
	};
}

