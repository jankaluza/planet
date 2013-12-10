#include "../GLee.h"
#include "../pgr.h"
#include "Screen.h"
#include "../Objects/Planet.h"
#include "../Objects/Ship.h"
#include "../Objects/SkyBox.h"
#include "../Objects/Plant.h"
#include "../Objects/Plant1.h"
#include "../Objects/Box.h"
#include "../Objects/Hero.h"
#include "../Objects/Text.h"
#include "TextureManager.h"
#include "Matrix.h"
#include "ObjectManager.h"
#include "FontManager.h"
#include "PathLoader.h"
#include "MapGenerator.h"

#include "GL/glu.h"

namespace Planet2 {

	Screen::Screen(unsigned width, unsigned height, unsigned color, unsigned depth, unsigned stencil)
		: _color(color), _rx(0), _ry(0), _pz(-10) {
			// Set OpenGL attributes
			if(SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, color) < 0) throw SDL_Exception();
			if(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depth) < 0) throw SDL_Exception();
			if(SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, stencil) < 0) throw SDL_Exception();
			if(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) < 0) throw SDL_Exception();

			// Create window
			_screen = SDL_SetVideoMode(width, height, color, SDL_OPENGL | SDL_RESIZABLE);
			if (_screen == NULL) 
				throw SDL_Exception();
			SDL_WM_SetCaption("PGR - One button game", NULL);

			// Call init code
			onInit();

			onWindowResized(width, height);
	}

	Screen::~Screen() {
		SDL_FreeSurface(_screen);
	}


	void Screen::onInit() {
		// Initialize static textures and the vertex array
		CMath::InitStaticMembers();

		GLUtil()->Init();	

		Planet = new CPlanet(this);
		Planet->Init();
		
		CDoubleVector vPos = CDoubleVector(0, 1100, 900);
		Camera->SetPosition(vPos);

		//Planet->update(Camera, 0);
		//Planet->update(Camera, 0);
		//Planet->update(Camera, 0);

		Objects->AddObject(Planet, 0);
		Objects->AddObject(new SkyBox(this), 3);

		/*CVector x = CVector(1, 0, 0);
		x.Normalize();
		CVector y = CVector(-1, 0, 0);
		y.Normalize();
		cout << "Test angle: " << x.Angle(y) << endl;*/
		/*
		for (float lat = -3.14*0.5; lat < 0*3.14; lat += 0.5) {
		for (float lon = -3.14*1; lon < -0.9*3.14; lon += 0.5) {
		m_planet->AddObject(new Plant(this), lat, lon);
		}
		}*/

		CRandom r(666);
		float Pi = 3.14;
		float PiPi = 2 * Pi;	
		//Plants
		{
// 			GroupObjectManager *plants = new GroupObjectManager(this);

// 			for (int i = 0; i < 300; ++i)
// 			{
// 				//for (float lat = -3.14*2; lat < 2*3.14; lat += 0.5) {
// 				//for (float lon = -3.14*2; lon < 2*3.14; lon += 0.5) {
// 				// 	float lat = 0;
// 				// 	float lon = 1.8;
// 				float lat = r.RandomD(-PiPi, PiPi);
// 				float lon = r.RandomD(-PiPi, PiPi);
// 
// 				Plant * plant = new Plant(this);
// 				plant->SetScale(CVector(r.RandomD(1, 10)));
// 				plant->Rotate(plant->GetUpAxis(), r.RandomD(0, PiPi));
// 				// Spherical to Cartesian coordinates conversion
// 				float clat = CMath::cos(lat);
// 				CVector pos = CVector(clat * CMath::sin(lon), CMath::sin(lat), clat * CMath::cos(lon));
// 				plant->SetPosition(pos);
// 
// 				plants->AddObject(plant);
// 				//}
// 			}

#define PLANT(X, Y)			{ \
				float lat = X; \
				float lon = Y; \
 \
				Plant * plant = new Plant(this); \
				plant->SetScale(CVector(10)); \
				float clat = CMath::cos(lat); \
				CVector pos = CVector(clat * CMath::sin(lon), CMath::sin(lat), clat * CMath::cos(lon)); \
				plant->SetPosition(pos); \
 \
				plants->AddObject(plant); \
			}
// 			PLANT(0.1,0.1);
// 			PLANT(0.6,0.5);
// 			PLANT(0.2,0.8);
				//}
// 			Planet->AddObjectGroup(plants);
		}

		//Cacti
		{
			GroupObjectManager *cactis = new GroupObjectManager(this);
			for (int i = 0; i < 40; ++i)
			{
				float lat = r.RandomD(-Pi, Pi);
				float lon = r.RandomD(-Pi, Pi);

				Plant1 * plant1 = new Plant1(this);
				plant1->SetScale(r.RandomD(2, 6));
				plant1->Rotate(plant1->GetUpAxis(), r.RandomD(0, PiPi));
				// Spherical to Cartesian coordinates conversion
				CVector pos = CVectorFromSpherical(lat, lon);
				plant1->SetPosition(pos);

				cactis->AddObject(plant1);
			}
			Planet->AddObjectGroup(cactis);
		}

// 		//Box
// 		{
// 			GroupObjectManager *boxes = new GroupObjectManager(this);
// 
// 			for (int i = 0; i < 100; ++i)
// 			{
// 				float lat = r.RandomD(-PiPi, PiPi);
// 				float lon = r.RandomD(-PiPi, PiPi);
// 
// 				Box *box = new Box(this);	
// 				box->solid = false;
// 				box->rotateWithTriangle = true;
// 				box->Rotate(box->GetUpAxis(), r.RandomD(0, PiPi));
// 				// Spherical to Cartesian coordinates conversion
// 				float clat = CMath::cos(lat);
// 				CVector pos = CVector(clat * CMath::sin(lon), CMath::sin(lat), clat * CMath::cos(lon));
// 				box->SetPosition(pos);
// 
// 				boxes->AddObject(box);
// 			}
// 			Planet->AddObjectGroup(boxes);
// 		}

		Heron = new Hero(this);
		Heron->SetPosition(Camera->GetPosition());
		Heron->rotateWithTriangle = true;
		Planet->AddObject(Heron, 1);
		//Objects->AddObject(Heron, 1);

		Catn = NULL;

		PathLoader pathLoader("./data/maps/map.txt");
		pathLoader.load(this);

		MapGenerator mapGenerator;
		mapGenerator.generate(this);

	}

	void Screen::onWindowRedraw(float dt) {
		//Clear
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Objects->update(Camera, dt);

		glPushMatrix();
		glLoadMatrixf(Camera->GetViewMatrix());

		Objects->draw();

		glPopMatrix();

		SDL_GL_SwapBuffers();
	}

	void Screen::onWindowResized(int w, int h) {
		// This frees previous _screen according to docs
		_screen = SDL_SetVideoMode(w, h, _color, SDL_OPENGL | SDL_RESIZABLE);
		if (_screen == NULL) 
			throw SDL_Exception();

		glViewport(0, 0, w, h);
		_width = w; _height = h;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, (double)w / (double)h, 1, 10000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

}
