#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Music.h"

DECLARE_MUSIC(template1);

UINT8 collision_tiles[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

IMPORT_MAP(map);

void START() {
	scroll_target = SpriteManagerAdd(SpritePlayer, 50, 50);
	InitScroll(BANK(map), &map, collision_tiles, 0);
	SpriteManagerAdd(SpriteCoal, 70, 50);
	//PlayMusic(template1, 1);
}

void UPDATE() {
}
