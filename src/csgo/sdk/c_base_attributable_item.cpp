#include "c_base_entity.h"

#include "../netvar_manager/netvar_manager.h"

short c_base_attributable_item::item_definition_index()
{
	return *reinterpret_cast<short*>(uintptr_t(this) +
		g_netvars.get_netvar("DT_BaseAttributableItem::m_iItemDefinitionIndex"));
}