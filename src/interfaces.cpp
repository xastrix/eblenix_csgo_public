#include "interfaces.h"

#include "signatures.h"
#include "globals.h"
#include "helpers.h"

void CSGO::init()
{
	m_client = Helpers::get_interface<c_base_client>(GLOBAL(module_list[clientDLL]), CLIENT_INTERFACE_VERSION);

	if (!m_client) {
#ifdef _DEBUG
		printf("Failed to create client interface\n");
#endif
		return;
	}

	m_entity_list = Helpers::get_interface<c_entity_list>(GLOBAL(module_list[clientDLL]), CLIENT_ENTITY_INTERFACE_VERSION);

	if (!m_entity_list) {
#ifdef _DEBUG
		printf("Failed to create entity list interface\n");
#endif
		return;
	}

	m_engine = Helpers::get_interface<c_engine_client>(GLOBAL(module_list[engineDLL]), ENGINE_INTERFACE_VERSION);

	if (!m_engine) {
#ifdef _DEBUG
		printf("Failed to create engine interface\n");
#endif
		return;
	}

	m_panel = Helpers::get_interface<c_panel>(GLOBAL(module_list[vgui2DLL]), VGUI_PANEL_INTERFACE_VERSION);

	if (!m_panel) {
#ifdef _DEBUG
		printf("Failed to create vgui panel interface\n");
#endif
		return;
	}

	m_mat_system = Helpers::get_interface<c_material_system>(GLOBAL(module_list[materialsystemDLL]), MATERIAL_SYSTEM_INTERFACE_VERSION);

	if (!m_mat_system) {
#ifdef _DEBUG
		printf("Failed to create material system interface\n");
#endif
		return;
	}

	m_cvar = Helpers::get_interface<c_base_convars>(GLOBAL(module_list[vstdlibDLL]), ENGINE_CVAR_INTERFACE_VERSION);

	if (!m_cvar) {
#ifdef _DEBUG
		printf("Failed to create engine cvar interface\n");
#endif
		return;
	}

	m_trace = Helpers::get_interface<c_trace_ray>(GLOBAL(module_list[engineDLL]), TRACE_INTERFACE_VERSION);

	if (!m_trace) {
#ifdef _DEBUG
		printf("Failed to create engine trace interface\n");
#endif
		return;
	}

	m_model_info = Helpers::get_interface<c_model_info>(GLOBAL(module_list[engineDLL]), MODEL_INFO_INTERFACE_VERSION);

	if (!m_model_info) {
#ifdef _DEBUG
		printf("Failed to create model info interface\n");
#endif
		return;
	}

	m_model_render = Helpers::get_interface<c_model_render>(GLOBAL(module_list[engineDLL]), ENGINE_MODEL_INTERFACE_VERSION);

	if (!m_model_render) {
#ifdef _DEBUG
		printf("Failed to create model render interface\n");
#endif
		return;
	}

	m_render_view = Helpers::get_interface<c_render_view>(GLOBAL(module_list[engineDLL]), ENGINE_RENDER_VIEW_INTERFACE_VERSION);

	if (!m_render_view) {
#ifdef _DEBUG
		printf("Failed to create engine render view interface\n");
#endif
		return;
	}

	m_event_manager = Helpers::get_interface<c_game_event_manager2>(GLOBAL(module_list[engineDLL]), GAME_EVENT_MANAGER_INTERFACE_VERSION);

	if (!m_event_manager) {
#ifdef _DEBUG
		printf("Failed to create game event manager interface\n");
#endif
		return;
	}

	m_device = *Helpers::read<IDirect3DDevice9**>(reinterpret_cast<uintptr_t>(g_sig.s_device));

	if (!m_device) {
#ifdef _DEBUG
		printf("Failed to make device pointer\n");
#endif
		return;
	}

	m_globals = *Helpers::read<c_global_vars**>((*reinterpret_cast<uintptr_t**>(m_client))[11] + 10);

	if (!m_globals) {
#ifdef _DEBUG
		printf("Failed to make global vars pointer\n");
#endif
		return;
	}

	m_client_mode = *Helpers::read<uintptr_t**>((*reinterpret_cast<uintptr_t**>(m_client))[10] + 5);

	if (!m_client_mode) {
#ifdef _DEBUG
		printf("Failed to make client mode pointer\n");
#endif
		return;
	}

	m_input = Helpers::read<i_input*>(reinterpret_cast<uintptr_t>(g_sig.s_input));

	if (!m_input) {
#ifdef _DEBUG
		printf("Failed to make input pointer\n");
#endif
		return;
	}

	m_weapon_system = Helpers::read<c_weapon_system*>(reinterpret_cast<uintptr_t>(g_sig.s_weapon_system));

	if (!m_weapon_system) {
#ifdef _DEBUG
		printf("Failed to make weapon system pointer\n");
#endif
		return;
	}

	m_glow_manager = Helpers::read_ptr<c_glow_manager*>(*reinterpret_cast<uintptr_t*>(g_sig.s_glow_manager));

	if (!m_glow_manager) {
#ifdef _DEBUG
		printf("Failed to make glow manager pointer\n");
#endif
	}
}

void CSGO::init_local(const local_t& local)
{
	m_local = local;
}

c_base_player* CSGO::get_local()
{
	if (!m_local)
		return nullptr;

	return m_local.get();
}