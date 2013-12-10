#pragma once

#include <SDL/SDL_ttf.h>
#include <map>
#include <string>

namespace Planet2
{

	class FontManager
	{
	public:
		FontManager(const std::string &path);
		virtual ~FontManager();

		TTF_Font *GetFont(const std::string &name, int size);

	private:
		typedef std::map<int, TTF_Font *> SizeMap;
		typedef std::map<std::string, SizeMap> FontMap;
		std::string _dir;
		FontMap _fonts;
	};
}

