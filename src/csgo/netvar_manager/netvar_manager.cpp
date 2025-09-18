#include "netvar_manager.h"

#include "../interfaces/interfaces.h"
#include "../sdk/sdk.h"
#include "../fnv.h"

#include <unordered_map>

netvar_manager g_netvars;

using netvar_key_value_map = std::unordered_map<uint32_t, uintptr_t>;
using netvar_table_map = std::unordered_map<uint32_t, netvar_key_value_map>;

static void add_props_for_table(
	netvar_table_map& table_map,
	const uint32_t table_name_hash,
	const std::string& table_name,
	recv_table* table,
	const size_t child_offset = 0
);
static void initialize_props(netvar_table_map& table_map);

uintptr_t netvar_manager::get_netvar(const std::string& netvar)
{
	static netvar_table_map map{};

	if (map.empty())
		initialize_props(map);

	size_t pos{ netvar.find("::", 0) };
	std::string table{ netvar.substr(0, pos) }, prop{};

	size_t offset_pos{ 0 };
	while ((offset_pos = netvar.find(table, offset_pos)) != std::string::npos) {
		offset_pos += netvar.length();
		size_t end_pos = netvar.find("::", pos);
		prop = netvar.substr(pos + 2, end_pos - offset_pos);
	}

	if (map.find(fnv::hash(table.c_str())) == map.end())
		return 0;

	netvar_key_value_map& table_map = map.at(fnv::hash(table.c_str()));
	if (table_map.find(fnv::hash(prop.c_str())) == table_map.end())
		return 0;

	return table_map.at(fnv::hash(prop.c_str()));
}

static void add_props_for_table(netvar_table_map& table_map,
	const uint32_t table_name_hash,
	const std::string& table_name,
	recv_table* table,
	const size_t child_offset
){
	for (int i = 0; i < table->props_count; i++) {
		auto& prop = table->props[i];

		auto name = std::string{ prop.prop_name };

		if (prop.data_table && prop.elements_count > 0) {
			if (name.substr(0, 1) == "0")
				continue;

			add_props_for_table(table_map, table_name_hash, table_name,
				prop.data_table, prop.offset + child_offset);
		}

		if (name.substr(0, 1) != "m")
			continue;

		const auto name_hash = fnv::hash(prop.prop_name);
		const auto offset = uintptr_t(prop.offset) + child_offset;

		table_map[table_name_hash][name_hash] = offset;
	}
}

static void initialize_props(netvar_table_map& table_map)
{
	for (auto c = g_csgo.m_client->get_client_classes(); c; c = c->next_ptr)
	{
		const auto table = reinterpret_cast<recv_table*>(c->recvtable_ptr);
		const auto table_name = table->table_name;
		const auto table_name_hash = fnv::hash(table_name);

		if (table == nullptr)
			continue;

		add_props_for_table(table_map, table_name_hash, table_name, table);
	}
}