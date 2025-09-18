#pragma once

#include "../render_manager/sprite_manager/sprite_manager.h"

enum _ui_sprite_list {
	UI_SPRITE_NONE,
	UI_SPRITE_LOGO,
	maxUISprites
};

struct ui_sprites {
	sprite_t& get_sprite(const _ui_sprite_list index);
private:
	sprite_t m_sprites[maxUISprites]{};
};