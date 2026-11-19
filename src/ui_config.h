#pragma once

#define MAX_MENU_ENTRIES                        12
#define MAX_SUB_ENTRIES                         16

/* Menu box width in pixels */
#define MENU_BOX_WIDTH                          178

/* Menu box height in pixels */
#define MENU_BOX_HEIGHT                         22

/* Display "+" signs on the right side of the menu box */
#define MENU_DRAW_PLUS_SIGNS

/* Enable/disable menu animations (open/close ui, checkbox, etc) */
#define MENU_ANIMATIONS

enum _ui_states {
	UI_NONE_STATE,
	UI_BOOL_STATE,
	UI_INT_STATE,
	UI_FLOAT_STATE,
	UI_FUNCTION_STATE,
	UI_ITEM_STATE,
	UI_KEY_STATE,
	UI_TAB_STATE,
};

enum _ui_subs {
	UI_SUB_POS,
	UI_SUB_SUB_POS,
	UI_SUB_SUB_SUB_POS,
	maxUISubs,
};

enum _ui_colors {
	UI_TEXT_COL,
	UI_MAIN_COL,
	UI_PRIMARY_COL,
	UI_SHADOW_COL,
	UI_OUTLINE_COL,
	maxUIColors,
};

enum _ui_sprites {
	UI_SPRITE_LOGO,
	maxUISprites,
};