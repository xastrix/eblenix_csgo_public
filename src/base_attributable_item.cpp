#include "base_entity.h"

#include "netvar_manager.h"
#include "helpers.h"

short c_base_attributable_item::item_definition_index()
{
	return Helpers::read<short>(uintptr_t(this) +
		netvar_manager::get_address("DT_BaseAttributableItem::m_iItemDefinitionIndex"));
}