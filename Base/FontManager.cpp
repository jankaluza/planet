#include "FontManager.h"
#include "../pgr.h"

using namespace Planet2;
using namespace std;

FontManager::FontManager(const std::string &path) : _dir(getFilePath(path)) {

}

FontManager::~FontManager() {
}

TTF_Font *FontManager::GetFont(const std::string &name, int size) {
	FontMap::iterator it = _fonts.find(name);
	if (it != _fonts.end()) {
		SizeMap::iterator is = it->second.find(size);
		if (is != it->second.end()) {
			return is->second;
		}
	}

	TTF_Font *font = TTF_OpenFont(std::string(_dir + name).c_str(), size);
	_fonts[name][size] = font;
	return font;
}
