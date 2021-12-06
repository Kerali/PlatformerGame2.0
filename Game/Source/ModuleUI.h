#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include "SDL\include\SDL_pixels.h"

#define MAX_FONTS 100
#define MAX_FONT_CHARS 256
#define DYNAMIC_TEXT_LEN 10

struct SDL_Texture;

struct Font
{
	// Lookup table. All characters displayed in the same order as the texture
	char table[MAX_FONT_CHARS];

	// The font texture
	SDL_Texture* texture = nullptr;

	// Font setup data
	uint totalLength;
	uint rows, columns;
	uint char_w, char_h;
};


class ModuleUI : public Module
{

public:
	ModuleUI();
	~ModuleUI();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:
	int score;
	int lifes;

	int font = -1;

	const char* fontPath;
	char scoreText[DYNAMIC_TEXT_LEN + 1] = { "0000000000" };
	char lifeText[2] = { "0" };

	SDL_Texture* pointSpritesheetTex;
	SDL_Rect pointsRect;

	// Loads a font file from a texture
	// Returns a font index from the fonts array
	// Param texturePath	- The path to the texture file
	// Param characters		- The lookup table. All characters displayed in the same order as the texture
	// Param rows			- The amount of character rows in the texture
	int Load(const char* texturePath, const char* characters, uint rows = 1);

	// Removes a font by its index
	// Unloads the texture and removes it from the fonts array
	void UnLoad(int fontIndex);

	// Create a surface from text
	void BlitText(int x, int y, int fontIndex, const char* text) const;
	void IntToDynamicString(char* buffer, int k);

	int k = 0;

private:
	// An array to keep track and store all loaded fonts
	Font fonts[MAX_FONTS];
};

#endif 
