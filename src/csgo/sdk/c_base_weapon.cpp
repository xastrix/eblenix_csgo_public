#include "c_base_entity.h"

#include "../interfaces/interfaces.h"
#include "../netvar_manager/netvar_manager.h"

#include <vector>

float c_base_weapon::next_primary_attack()
{
	return *reinterpret_cast<float*>(uintptr_t(this) +
		g_netvars.get_netvar("DT_BaseCombatWeapon::m_flNextPrimaryAttack"));
}

int c_base_weapon::clip1_count()
{
	return *reinterpret_cast<int*>(uintptr_t(this) +
		g_netvars.get_netvar("DT_BaseCombatWeapon::m_iClip1"));
}

int c_base_weapon::primary_reserve_ammo_acount()
{
	return *reinterpret_cast<int*>(uintptr_t(this) +
		g_netvars.get_netvar("DT_BaseCombatWeapon::m_iPrimaryReserveAmmoCount"));
}

short c_base_weapon::item_definition_index()
{
	return *reinterpret_cast<short*>(uintptr_t(this) +
		g_netvars.get_netvar("DT_BaseAttributableItem::m_iItemDefinitionIndex"));
}

weapon_info_t* c_base_weapon::get_weapon_data()
{
	return g_csgo.m_weapon_system->get_weapon_data(item_definition_index());
}