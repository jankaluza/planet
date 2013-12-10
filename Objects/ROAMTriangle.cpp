#include "ROAMTriangle.h"
#include "Object.h"

CROAMTriangle::~CROAMTriangle() {
	for (std::vector<Planet2::Object *>::const_iterator it = objects.begin(); it != objects.end(); ++it) {
		(*it)->tri = NULL;
	}
}
