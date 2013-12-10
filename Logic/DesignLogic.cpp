
#include "DesignLogic.h"
#include "../Base/Screen.h"
#include "../Base/PathFollower.h"
#include "../Base/Matrix.h"
#include "../Objects/Ship.h"
#include "../pgr.h"

namespace Planet2 {

	DesignLogic::DesignLogic(Screen *screen) : _screen(screen), _rx(0), _ry(0), _t0(0), _t1(0) {
		_t0 = SDL_GetTicks()/1000.0f;
		_t1 = _t0;

		_fm = FM_None;
		_sm = SM_None;
		_rm = RM_None;

		_sumT = 0;
		_ticksPerSec = 0;

// 		CDoubleVector vPos =  CDoubleVector(0, 30, 100); //CDoubleVector(0, 0, 4000);
// 		_screen->Camera->SetPosition(vPos);
	}

	DesignLogic::~DesignLogic() {

	}

	void DesignLogic::onTick() 
	{
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

		const float moveFac = 500 * _dt;
		const float rotateFac = 1.5 * _dt;
		switch(_fm)
		{
		case FM_Forward:
			_screen->Camera->SetPosition(_screen->Camera->GetPosition() + _screen->Camera->GetViewAxis() * moveFac);	
			break;
		case FM_Backward:
			_screen->Camera->SetPosition(_screen->Camera->GetPosition() + _screen->Camera->GetViewAxis() * -moveFac);	
			break;
		}

		switch(_sm)
		{
		case SM_Right:
			_screen->Camera->SetPosition(_screen->Camera->GetPosition() + _screen->Camera->GetRightAxis() * moveFac);	
			break;
		case SM_Left:
			_screen->Camera->SetPosition(_screen->Camera->GetPosition() + _screen->Camera->GetRightAxis() * -moveFac);	
			break;			
		}

		switch(_rm)
		{
		case RM_CW:
			_screen->Camera->Rotate(_screen->Camera->GetViewAxis(), rotateFac);	
			break;
		case RM_CCW:
			_screen->Camera->Rotate(_screen->Camera->GetViewAxis(), -rotateFac);	
			break;
		}
	}

	void DesignLogic::onKeyDown(SDLKey key, Uint16 /*mod*/)
	{
		switch(key) {
		case SDLK_ESCAPE: quit(); break;
		case SDLK_UP:
		case SDLK_w: 
			_fm = FM_Forward;
			break;
		case SDLK_DOWN:
		case SDLK_s: 
			_fm = FM_Backward;
			break;

		case SDLK_LEFT:
		case SDLK_a: 
			_sm = SM_Left;
			break;
		case SDLK_RIGHT:
		case SDLK_d: 
			_sm = SM_Right;
			break;

		case SDLK_q: 
			_rm = RM_CCW;
			break;
		case SDLK_e: 
			_rm = RM_CW;
			break;
		default:
			// 			_screen->getShip()->toggle();
			break;
		}
	}

	void DesignLogic::onKeyUp(SDLKey key, Uint16 /*mod*/)
	{
		switch(key) 
		{
		case SDLK_UP:
		case SDLK_DOWN:
		case SDLK_w: 
		case SDLK_s: 
			_fm = FM_None;
			break;

		case SDLK_LEFT:
		case SDLK_RIGHT:
		case SDLK_a: 
		case SDLK_d: 
			_sm = SM_None;

		case SDLK_q: 
		case SDLK_e: 		
			_rm = RM_None;
			break;
			break;
		}
	}

	void DesignLogic::onMouseMove(unsigned /*x*/, unsigned /*y*/, int xrel, int yrel, Uint8 buttons)
	{
		if(buttons & SDL_BUTTON_LMASK)
		{
			// 		_screen->getPlanet()->Rotate(CVector(0, 1, 0), xrel * 0.03);
			// 		_screen->getPlanet()->Rotate(CVector(1, 0, 0), yrel * 0.03);
			_screen->Camera->Rotate(_screen->Camera->GetRightAxis(), -yrel* 0.01);
			_screen->Camera->Rotate(_screen->Camera->GetUpAxis(), -xrel* 0.01);
		}
		if(buttons & SDL_BUTTON_RMASK)
		{
			_screen->Camera->SetPosition(_screen->Camera->GetPosition() + _screen->Camera->GetViewAxis() * -yrel * 0.5);	
		}	
	}

	void DesignLogic::onMouseDown(Uint8 button, unsigned /*x*/, unsigned /*y*/)
	{

	}

	void DesignLogic::onMouseUp(Uint8 /*button*/, unsigned /*x*/, unsigned /*y*/)
	{
	}

}
