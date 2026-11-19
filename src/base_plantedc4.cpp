#include "base_entity.h"

#include "interfaces.h"
#include "helpers.h"

bool c_base_plantedc4::bomb_ticking()
{
	return Helpers::read<bool>(uintptr_t(this) +
		Helpers::get_netvar("DT_PlantedC4::m_bBombTicking"));
}

float c_base_plantedc4::defuse_count_down()
{
	return Helpers::read<float>(uintptr_t(this) +
		Helpers::get_netvar("DT_PlantedC4::m_flDefuseCountDown"));
}

int c_base_plantedc4::bomb_defuser()
{
	return Helpers::read<int>(uintptr_t(this) +
		Helpers::get_netvar("DT_PlantedC4::m_hBombDefuser"));
}

int c_base_plantedc4::bomb_site()
{
	return Helpers::read<int>(uintptr_t(this) +
		Helpers::get_netvar("DT_PlantedC4::m_nBombSite"));
}

bool c_base_plantedc4::bomb_defused()
{
	return Helpers::read<bool>(uintptr_t(this) +
		Helpers::get_netvar("DT_PlantedC4::m_bBombDefused"));
}

float c_base_plantedc4::c4_blow()
{
	return Helpers::read<float>(uintptr_t(this) +
		Helpers::get_netvar("DT_PlantedC4::m_flC4Blow"));
}