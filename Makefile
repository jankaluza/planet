##################################################
# nastaveni
CFLAGS_COMMON=-pipe -ggdb
CC=gcc
CXX=g++

##################################################
# nastaveni knihoven s OpenGL
ifeq ($(OS), Windows_NT)
	CFLAGS=$(CFLAGS_COMMON) -Wl,--enable-auto-import
	LIBS=-lopengl32 -lglew32 -lSDL
	RM=del
else
	USE_GLEE=1
	CFLAGS=$(CFLAGS_COMMON) `sdl-config --cflags` -I. -DUSE_GLEE
	LIBS=-L/usr/lib `sdl-config --libs` -lGL -lGLU -lSDL_image -lSDL_ttf
	RM=rm -f
endif

CXXFLAGS=$(CFLAGS)

##################################################
# obecne cile

LAB1=planet

	DEPS=pgr.o GLee.o Logic/GameLogic.o Logic/DesignLogic.o \
Logic/DesignLogic.o \
Objects/HeightMap.o \
Objects/PlanetaryMap.o \
Objects/Planet.o \
Objects/ROAMSphere.o \
Objects/ROAMTriangle.o \
Objects/Ship.o \
Objects/SurfaceMap.o \
Objects/SkyBox.o \
Base/GLUtil.o \
Base/MainLoop.o \
Base/Math.o \
Base/Matrix.o \
Base/Noise.o \
Base/PathFollower.o \
Base/PixelBuffer.o \
Base/Screen.o \
Base/Texture.o \
Base/World.o \
Base/ModelManager.o \
Base/TextureManager.o \
Base/MaterialManager.o \
Base/LodManager.o \
Base/ObjectManager.o \
Base/GroupObjectManager.o \
Base/FontManager.o \
Base/PathLoader.o \
Base/MapGenerator.o \
Objects/Plant.o \
Objects/Box.o \
Objects/Cat.o \
Objects/Text.o \
Objects/Plant1.o \
Objects/GroupObjectRenderableBase.o \
Objects/Hero.o


.PHONY: all lab1

all: lab1

lab1: $(LAB1)

clean:
	$(RM) $(LAB1)
	$(RM) $(DEPS)
##################################################
# zavislosti

pgr.o: pgr.cpp pgr.h GLee.c GLee.h \
Logic/DesignLogic.cpp \
Objects/HeightMap.cpp \
Objects/PlanetaryMap.cpp \
Objects/Planet.cpp \
Objects/ROAMSphere.cpp \
Objects/Ship.cpp \
Objects/SurfaceMap.cpp \
Objects/SkyBox.cpp \
Objects/SkyBox.h \
Objects/Plant.h \
Objects/Plant.cpp \
Base/GLUtil.cpp \
Base/MainLoop.cpp \
Base/Math.cpp \
Base/Matrix.cpp \
Base/Noise.cpp \
Base/PathFollower.cpp \
Base/PixelBuffer.cpp \
Base/Screen.cpp \
Base/Texture.cpp \
Base/GLUtil.h \
Base/ListTemplates.h \
Base/MainLoop.h \
Base/Math.h \
Base/Matrix.h \
Base/Noise.h \
Base/PathFollower.h \
Base/PixelBuffer.h \
Base/Screen.h \
Base/Texture.h \
Base/World.h \
Base/World.cpp \
Base/ModelManager.cpp \
Base/ModelManager.h \
Base/TextureManager.h \
Base/TextureManager.cpp \
Base/MaterialManager.h \
Base/LodManager.h \
Base/LodManager.cpp \
Base/ObjectManager.h \
Base/ObjectManager.cpp \
Base/GroupObjectManager.h \
Base/GroupObjectManager.cpp \
Base/FontManager.h \
Base/FontManager.cpp \
Base/PathLoader.h \
Base/PathLoader.cpp \
Base/MaterialManager.cpp \
Base/MapGenerator.h \
Base/MapGenerator.cpp \
Objects/GroupObjectRenderableBase.cpp \
Objects/GroupObjectRenderableBase.h \
Objects/Plant1.cpp \
Objects/Plant1.h \
Objects/Box.cpp \
Objects/Box.h \
Objects/Cat.cpp \
Objects/Cat.h \
Objects/Hero.cpp \
Objects/Hero.h \
Objects/Text.cpp \
Objects/Text.h \
Objects/ROAMTriangle.cpp \
Objects/ROAMTriangle.h


# LAB1

planet: main.cpp $(DEPS)
	$(CXX) -o $@ $^ $(CFLAGS) $(LIBS)

