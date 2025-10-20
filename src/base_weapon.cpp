#include "base_entity.h"

#include "interfaces.h"
#include "netvar_manager.h"

float c_base_weapon::next_primary_attack()
{
	return *reinterpret_cast<float*>(uintptr_t(this) +
		netvar_manager::get_address("DT_BaseCombatWeapon::m_flNextPrimaryAttack"));
}

int c_base_weapon::clip1_count()
{
	return *reinterpret_cast<int*>(uintptr_t(this) +
		netvar_manager::get_address("DT_BaseCombatWeapon::m_iClip1"));
}

int c_base_weapon::primary_reserve_ammo_acount()
{
	return *reinterpret_cast<int*>(uintptr_t(this) +
		netvar_manager::get_address("DT_BaseCombatWeapon::m_iPrimaryReserveAmmoCount"));
}

short c_base_weapon::item_definition_index()
{
	return *reinterpret_cast<short*>(uintptr_t(this) +
		netvar_manager::get_address("DT_BaseAttributableItem::m_iItemDefinitionIndex"));
}

weapon_info_t* c_base_weapon::get_weapon_data()
{
	return g_csgo.m_weapon_system->get_weapon_data(item_definition_index());
}