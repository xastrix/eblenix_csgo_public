#include "aimbot.h"

#include "vars.h"

void c_aimbot::run(user_cmd_t* cmd)
{
	if (g_var->get_as<bool>(V_AIMBOT_ENABLED).value())
		auto_pistol_t::get_instance().think(cmd);

	triggerbot_t::get_instance().think(cmd);
}