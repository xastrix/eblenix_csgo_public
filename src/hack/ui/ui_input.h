#pragma once

#include "ui_adders.h"

#include "../globals.h"
#include "../var_manager/var_manager.h"
#include "../input_manager/input_manager.h"

struct ui_input : public ui_adders {
	void handle_toggle_keys(unsigned int k);
	void handle_input(unsigned int k);
	bool get_menu_state();
};