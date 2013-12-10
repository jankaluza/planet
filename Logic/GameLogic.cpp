
#include "GameLogic.h"
#include "../Base/Screen.h"
#include "../Base/PathFollower.h"
#include "../Base/FontManager.h"
#include "../Objects/Hero.h"
#include "../Objects/Text.h"
#include "../Objects/Cat.h"
#include "../pgr.h"

namespace Planet2 {

	static int jump = 0;
	static void *lastObject;

	GameLogic::GameLogic(Screen *screen) : _screen(screen), _rx(0), _ry(0), _t0(0), _t1(0) {
		_three = new Text(_screen->Fonts->GetFont("font.ttf", 128), -1, 100, "3", 128, 128, 128);
		_two = new Text(_screen->Fonts->GetFont("font.ttf", 128), -1, 100, "2", 128, 128, 128);
		_one = new Text(_screen->Fonts->GetFont("font.ttf", 128), -1, 100, "1", 128, 128, 128);
		_go = new Text(_screen->Fonts->GetFont("font.ttf", 128), -1, 100, "GO", 128, 128, 128);
		_gameOver = new Text(_screen->Fonts->GetFont("font.ttf", 100), -1, -1, "GAME OVER", 255, 0, 0);
		_win = new Text(_screen->Fonts->GetFont("font.ttf", 100), -1, -1, "YOU WIN", 0, 255, 0);
		_left = new Text(_screen->Fonts->GetFont("font.ttf", 200), -1, 100, "<", 0, 255, 0);
		_right = new Text(_screen->Fonts->GetFont("font.ttf", 200), -1, 100, ">", 0, 255, 0);
		_press = new Text(_screen->Fonts->GetFont("font.ttf", 30), -1, 50, "Press 'r' to restart", 0, 255, 0);
		_l1 = new Text(_screen->Fonts->GetFont("font.ttf", 150), 20, -500, "❤", 255, 0, 0);
		_l2 = new Text(_screen->Fonts->GetFont("font.ttf", 150), 150, -500, "❤", 255, 0, 0);
		_l3 = new Text(_screen->Fonts->GetFont("font.ttf", 150), 280, -500, "❤", 255, 0, 0);

		reset();
	}

	GameLogic::~GameLogic() {
		delete _three;
		delete _two;
		delete _one;
		delete _go;
		delete _gameOver;
		delete _left;
		delete _right;
		delete _press;
		delete _l1;
		delete _l2;
		delete _l3;
		delete _win;
	}

	void GameLogic::reset() {
		_state = PreIntro;
		_cameraHeight = 2000;
		_cameraDistance = 1000;
		_camFlyAngle = 0;
		_rx = 0;
		_ry = 0;
		_t0 = 0;
		_t1 = 0;

		_follower = new PathFollower(_screen, _screen->path);
		_follower->setSpeed(0.2);

		_t0 = SDL_GetTicks()/1000.0f;
		_t1 = _t0;

		delete _screen->Camera;
		_screen->Camera = new C3DObject();

		CDoubleVector vPos = CDoubleVector(0, 0, 1378);
		_screen->Camera->SetPosition(vPos);

		C3DObject *c = _screen->Camera;
		c->Rotate(c->GetRightAxis(), 0.2);

		_height = _screen->getPlanet()->GetHeight(c->GetPosition()) + _cameraHeight;
		_toHeight = _height;
		_sumT = 0;
		_ticksPerSec = 0;

		CMatrix heroM;
		CVector target(0,0,0);
		CVector pos = CVectorFromSpherical(0.2, 0.2);
		heroM.ModelMatrix(pos, target, c->GetUpAxis());
		// 	heroM.Translate(0, 200, 0);
		*static_cast<C3DObject *>(_screen->Heron) = heroM;

		_screen->Objects->RemoveObject(_three, 4);
		_screen->Objects->RemoveObject(_two, 4);
		_screen->Objects->RemoveObject(_one, 4);
		_screen->Objects->RemoveObject(_go, 4);
		_screen->Objects->RemoveObject(_press, 4);
		_screen->Objects->RemoveObject(_gameOver, 4);
		_screen->Objects->RemoveObject(_left, 4);
		_screen->Objects->RemoveObject(_right, 4);
		_screen->Objects->RemoveObject(_win, 4);

		if (!_l1->managed) _screen->Objects->AddObject(_l1, 4);
		if (!_l2->managed) _screen->Objects->AddObject(_l2, 4);
		if (!_l3->managed) _screen->Objects->AddObject(_l3, 4);

		_screen->Heron->reset();
		lastObject = 0;
	}

	bool GameLogic::detectCollision() {
		ObjectList &_objects = _screen->Planet->getSolidObjects();
		for (ObjectList::const_iterator obj = _objects.begin(); obj != _objects.end(); ++obj) {
			if ((*obj)->visible) {
				if (_screen->Heron->DistanceTo(**obj) < 50 && *obj != lastObject) {
					lastObject = *obj;
					return true;
				}
			}
		}

		return false;
	}

	void GameLogic::handleIntro(float dt) {
		_screen->Heron->Moving = false;
		// Translate camera to be _cameraDistance behind Hero and _cameraHeight above Hero
		CVector center(0,0,0);
		C3DObject *c = _screen->Camera;
		c->SetPosition(_screen->Heron->GetPosition());

		CMatrix m;
		m.ModelMatrix(c->GetPosition(), center, c->GetUpAxis());
		m.Translate(0, -_cameraDistance, _cameraHeight);
		*c = m;

		// Focus camera to Hero
		m.ModelMatrix(c->GetPosition(), _screen->Heron->GetPosition(), c->GetUpAxis());
		*c = m;

		// For intro, change the camera distance and height until we are in the proper
		// distance from the Hero
		if (_cameraDistance > 600) {
			if (_cameraDistance > 900) {
				if (!_three->managed) {
					_screen->Objects->AddObject(_three, 4);
				}
			}
			else if (_cameraDistance > 800) {
				if (!_two->managed) {
					_screen->Objects->RemoveObject(_three, 4);
					_screen->Objects->AddObject(_two, 4);
				}
			}
			else if (_cameraDistance > 700) {
				if (!_one->managed) {
					_screen->Objects->RemoveObject(_two, 4);
					_screen->Objects->AddObject(_one, 4);
				}
			}
			else {
				if (!_go->managed) {
					_screen->Objects->RemoveObject(_one, 4);
					_screen->Objects->AddObject(_go, 4);
				}
			}
			_cameraDistance -= 100 * dt;
		}
		if (_cameraHeight > 400) {
			_cameraHeight -= 500 * dt;
		}

		// Proper distance -> start the game
		if (_cameraDistance <= 600 && _cameraHeight <= 400) 
			//if(true)
		{
			_cameraDistance = 600;
			_cameraHeight = 400;
			_state = Game;
			_screen->Objects->RemoveObject(_three, 4);
			_screen->Objects->RemoveObject(_two, 4);
			_screen->Objects->RemoveObject(_one, 4);
			_screen->Objects->RemoveObject(_go, 4);
		}
	}

	void GameLogic::handleGameOver(float dt)
	{
		_screen->Heron->Moving = false;
		// Move the camera to proper distance behind the Hero		

		CVector pCurrent = _screen->Heron->GetPosition(); //Nova pozice hrdiny
		const float r = 800; //Jaky odstup si ma drzet kamera od hrdiny
		const float angle = (30) * 3.14 / 180; //V jakem uhlu (stupne) je kamera od zeme
		//const float lerpSpeed = .05f; //0-1, 1 = okamzita zmena
		const float maxCamSpeed = 30;

		//Smer - pouzity pro rotacni osu
		CVector mDir = DirectionVector(pCurrent, _last);
		if (mDir != mDir) {
			return;
		}

		CVector rAx = mDir ^ pCurrent;
		rAx.Normalize();
		CQuaternion q = CQuaternion(rAx, angle);		

		mDir = pCurrent ^ rAx; //Kolmy na povrch planety

		rAx = pCurrent;
		rAx.Normalize();
		CQuaternion q2 = CQuaternion(rAx, _camFlyAngle);
		_camFlyAngle += 0.5 * dt;

		CQuaternion q3 = q2 * q;

		mDir = q3.RotateVector(mDir);
		//mDir = q.RotateVector(mDir); //Nyni smeruje od aktualni pozice smerem k obloze v danem uhlu
		mDir.Normalize();
		mDir *= r;

		CVector pCamNew = pCurrent + mDir; 

		//-----------------------------------------------------			
		_screen->Camera->SetPosition(pCamNew);
		_screen->Camera->RotateToVector(pCamNew);

		CMatrix x;
		x.ModelMatrix(pCamNew, pCurrent, _screen->Camera->GetUpAxis());
		*_screen->Camera = x;
	}

	void GameLogic::handleGame(float dt) {
		_screen->Heron->Moving = true;
		int turnPreparation;
		CVector old = _screen->Heron->GetPosition();
		CVector newPos;
		bool finish = _follower->follow(dt, newPos, turnPreparation);
		if (finish) {
			_screen->Objects->RemoveObject(_left, 4);
			_screen->Objects->RemoveObject(_right, 4);
			_screen->Objects->AddObject(_win, 4);
			_screen->Objects->AddObject(_press, 4);
			_state = GameOver;			
			_camFlyAngle = 0;
			return;
		}
		C3DObject *c = _screen->Camera;

		newPos *= old.Magnitude();

		switch (turnPreparation) {
		case 1:
			if (_left->managed) _screen->Objects->RemoveObject(_left, 4);
			if (!_right->managed) _screen->Objects->AddObject(_right, 4);
			break;
		case -1:
			if (!_left->managed) _screen->Objects->AddObject(_left, 4);
			if (_right->managed) _screen->Objects->RemoveObject(_right, 4);
			break;
		default:
			if (_left->managed) _screen->Objects->RemoveObject(_left, 4);
			if (_right->managed) _screen->Objects->RemoveObject(_right, 4);
			break;
		}

		// Rotate hero if the rotation is in path.
		CVector center(0,0,0);
		// 	CMatrix heroM;
		_screen->Heron->SetPosition(newPos);
		_screen->Heron->RotateToVector(newPos);
		// 	heroM.ModelMatrix(newPos, center, c->GetUpAxis());

		_ry = 0;
		if (jump) {
			if (jump == 2) {
				// 			std::cout << "jump\n";
				// We are jumping "up", so keep increasing magnitude until we reach _jumpHeight
				float fHeight = newPos.Magnitude() + dt * 300;
				newPos.Normalize();
				newPos *= fHeight;
				_screen->Heron->SetPosition(newPos);
				// 			_screen->Heron->RotateToVector(newPos);

				// 			heroM.ModelMatrix(newPos, center, c->GetUpAxis());
				// 			heroM.RotateX(M_PI/2);
				// 			*static_cast<C3DObject *>(_screen->Heron) = heroM;

				// If we reached _jumpHeight, start falling
				if (_screen->Heron->GetPosition().Magnitude() > _jumpHeight) {
					jump = 1;
				}
			}
			else {
				float fHeight = newPos.Magnitude() - dt * 300;
				newPos.Normalize();
				newPos *= fHeight;
				_screen->Heron->SetPosition(newPos);
				// 			_screen->Heron->RotateToVector(newPos);
				// 			std::cout << "down\n";
				// We are falling, decrease magnitude until we hit the ground
				// 			heroM.Translate(0, _ry, dt * -300);
				// 			heroM.RotateX(M_PI/2);
				// 			*static_cast<C3DObject *>(_screen->Heron) = heroM;

				// We have hit the ground, so sthe jump has finished
				if (_screen->getPlanet()->GetHeight(_screen->Heron->GetPosition()) + 10 > _screen->Heron->GetPosition().Magnitude()) {
					_follower->setSpeed(0.2);
					jump = 0;
				}
			}
		}
		else {
			// No jump... just move forward
			// 		heroM.Translate(0, _ry, 0);
			// 		*static_cast<C3DObject *>(_screen->Heron) = heroM;
			// 		_screen->Heron->SetPosition(newPos);
		}
		// If we are not jumping, we have to rotate Hero according to the planet surface
		// and compute collisions.
		if (!jump) {
			_screen->Planet->RefreshObject(_screen->Heron);
			// 			_screen->Heron->RotateToVector(newPos);


			if (detectCollision()) {
				bool gameOver = _screen->Heron->removeLife();
				if (_l3->managed) {
					_screen->Objects->RemoveObject(_l3, 4);
				}
				else if (_l2->managed) {
					_screen->Objects->RemoveObject(_l2, 4);
				}
				else if (_l1->managed) {
					_screen->Objects->RemoveObject(_l1, 4);
					_screen->Objects->AddObject(_gameOver, 4);
					_screen->Objects->AddObject(_press, 4);
					_screen->Objects->RemoveObject(_left, 4);
					_screen->Objects->RemoveObject(_right, 4);

					_state = GameOver;
					_camFlyAngle = 0;
				}
			}
			else {
				if (_gameOver->managed) {
					_screen->Objects->RemoveObject(_gameOver, 4);
				}
			}
		}
		newPos = _screen->Heron->GetPosition();
		if ((old - _last).Magnitude() > 0.01)
			_last = old;
		// 		std::cout << newPos.x << " " << newPos.y << " " << newPos.z << "\n";

		{
			// Move the camera to proper distance behind the Hero		

			CVector pCurrent = _screen->Heron->GetPosition(); //Nova pozice hrdiny
			CVector pOld = old; //Stara pozice hrdiny
			const float r = 800; //Jaky odstup si ma drzet kamera od hrdiny
			const float angle = (30) * 3.14 / 180; //V jakem uhlu (stupne) je kamera od zeme
			//const float lerpSpeed = .05f; //0-1, 1 = okamzita zmena
			const float maxCamSpeed = 30;

			//Smer - pouzity pro rotacni osu
			CVector mDir = DirectionVector(pCurrent, pOld);
			if (mDir != mDir) {
				return;
			}
			_screen->Heron->RotateToViewVector(-mDir);

			CVector rAx = mDir ^ pCurrent;
			rAx.Normalize();
			CQuaternion q = CQuaternion(rAx, angle);

			mDir = pCurrent ^ rAx; //Kolmy na povrch planety			
			mDir = q.RotateVector(mDir); //Nyni smeruje od aktualni pozice smerem k obloze v danem uhlu
			mDir.Normalize();
			mDir *= r;

			CVector pCamOld = c->GetPosition();
			CVector pCamNew = pCurrent + mDir; //toto je nova pozice kamery
			//tu ale nepouzijeme, tam bychom meli byt v idealnim pripade
			//misto toho interpolujeme z aktualni pozice podle maximalni rychlosti pohybu kamery
			//pCamNew = CVector::Lerp(pCamOld, pCamNew, lerpSpeed);
			CVector onv = pCamNew - pCamOld;
			float mag = onv.MagnitudeSquared();
			onv.Normalize();
			CVector dst = onv * maxCamSpeed;
			if (dst.MagnitudeSquared() < mag)
				pCamNew = pCamOld + dst;

			// 			_screen->Heron->Rotate(_screen->Heron->GetUpAxis(), _screen->Heron->GetViewAxis().Angle(mDir));


			//-----------------------------------------------------			
			c->SetPosition(pCamNew);
			c->RotateToVector(pCamNew);

			CMatrix x;
			x.ModelMatrix(pCamNew, pCurrent, c->GetUpAxis());
			*c = x;
		}
	}



	void GameLogic::onTick() {	
		_t0 = SDL_GetTicks()/1000.0f;
		_dt  = _t0 - _t1;
		_t1 = _t0;

		_ticksPerSec++;
		_sumT += _dt;
		if (_sumT > 1.0f)
		{
			cout << "FPS: " << _ticksPerSec << endl;

			_sumT -= 1.0f;
			_ticksPerSec = 0;
		}

		switch(_state) {
		case PreIntro:
			// Just initialize all things in single frame
			_screen->Planet->RefreshObject(_screen->Heron);
			handleGame(_dt);
			_state = Intro;
			break;
		case Intro:
			handleIntro(_dt);
			break;
		case Game:
			handleGame(_dt);
			break;
		case GameOver:
			handleGameOver(_dt);
			break;
		}
	}

	void GameLogic::onKeyDown(SDLKey key, Uint16 /*mod*/)
	{
		switch(key) {
		case SDLK_ESCAPE: 
			quit(); 
			break;

		case SDLK_r:
			if (_state == GameOver) {
				reset();
			}
			break;

		default:			
			if (_state == Game) {
				if (jump == 0) {
					jump = 2;
					_follower->setSpeed(0.25);
					_jumpHeight = _screen->Heron->GetPosition().Magnitude() + 100;
				}
			}

			// 			_screen->getShip()->toggle();
			break;
		}
	}

	void GameLogic::onKeyUp(SDLKey /*key*/, Uint16 /*mod*/)
	{
	}

	void GameLogic::onMouseMove(unsigned /*x*/, unsigned /*y*/, int xrel, int yrel, Uint8 buttons)
	{
		// 	if(buttons & SDL_BUTTON_LMASK)
		// 	{
		// 		rx += xrel;
		// 		ry += yrel;
		// 		redraw();
		// 	}
		// 	if(buttons & SDL_BUTTON_RMASK)
		// 	{
		// 		pz += yrel;
		// 		redraw();
		// 	}
	}

	void GameLogic::onMouseDown(Uint8 button, unsigned /*x*/, unsigned /*y*/)
	{

	}

	void GameLogic::onMouseUp(Uint8 /*button*/, unsigned /*x*/, unsigned /*y*/)
	{
	}

}
