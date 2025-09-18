#include "ui_sprites.h"

sprite_t& ui_sprites::get_sprite(const _ui_sprite_list index)
{
	return m_sprites[index];
}