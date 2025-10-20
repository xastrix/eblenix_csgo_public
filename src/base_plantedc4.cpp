#include "base_entity.h"

#include "interfaces.h"
#include "netvar_manager.h"

bool c_base_plantedc4::bomb_ticking()
{
	return *reinterpret_cast<bool*>(uintptr_t(this) +
		netvar_manager::get_address("DT_PlantedC4::m_bBombTicking"));
}

float c_base_plantedc4::defuse_count_down()
{
	return *reinterpret_cast<float*>(uintptr_t(this) +
		netvar_manager::get_address("DT_PlantedC4::m_flDefuseCountDown"));
}

int c_base_plantedc4::bomb_defuser()
{
	return *reinterpret_cast<int*>(uintptr_t(this) +
		netvar_manager::get_address("DT_PlantedC4::m_hBombDefuser"));
}

int c_base_plantedc4::bomb_site()
{
	return *reinterpret_cast<int*>(uintptr_t(this) +
		netvar_manager::get_address("DT_PlantedC4::m_nBombSite"));
}

bool c_base_plantedc4::bomb_defused()
{
	return *reinterpret_cast<bool*>(uintptr_t(this) +
		netvar_manager::get_address("DT_PlantedC4::m_bBombDefused"));
}

float c_base_plantedc4::c4_blow()
{
	return *reinterpret_cast<float*>(uintptr_t(this) +
		netvar_manager::get_address("DT_PlantedC4::m_flC4Blow"));
}

bool c_base_plantedc4::is_bomb_planted()
{
	return this->get_client_class()->class_id == cplantedc4;
}

float c_base_plantedc4::get_defuse_length()
{
	float ret = defuse_count_down() - g_csgo.m_globals->cur_time;
	return ret < 0 ? 0.0f : ret;
}