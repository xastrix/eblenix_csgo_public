#include "visuals.h"

#include "interfaces.h"

void c_visuals::run()
{
	if (!g_cs->m_engine->is_connected())
		return;

	if (!g_cs->get_local())
		return;

	player_esp_t::get_instance().think();
	world_esp_t::get_instance().think();
}