#include "base_entity.h"

#include "interfaces.h"
#include "netvar_manager.h"
#include "helpers.h"

float c_base_weapon::next_primary_attack()
{
	return Helpers::read<float>(uintptr_t(this) +
		netvar_manager::get_address("DT_BaseCombatWeapon::m_flNextPrimaryAttack"));
}

int c_base_weapon::clip1_count()
{
	return Helpers::read<int>(uintptr_t(this) +
		netvar_manager::get_address("DT_BaseCombatWeapon::m_iClip1"));
}

int c_base_weapon::primary_reserve_ammo_acount()
{
	return Helpers::read<int>(uintptr_t(this) +
		netvar_manager::get_address("DT_BaseCombatWeapon::m_iPrimaryReserveAmmoCount"));
}

short c_base_weapon::item_definition_index()
{
	return Helpers::read<short>(uintptr_t(this) +
		netvar_manager::get_address("DT_BaseCombatWeapon::m_iItemDefinitionIndex"));
}

weapon_info_t* c_base_weapon::get_weapon_data()
{
	return g_cs->m_weapon_system->get_weapon_data(item_definition_index());
}