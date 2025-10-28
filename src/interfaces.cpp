#include "interfaces.h"

#include "signatures.h"
#include "globals.h"
#include "helpers.h"

void interfaces::init()
{
	if (!create_interfaces() || !make_pointers()) {}
}

bool interfaces::create_interfaces()
{
	m_client = Helpers::get_interface<c_base_client>(g::module_list[clientDLL], CLIENT_INTERFACE_VERSION);

	if (!m_client) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to create client interface");
#endif
		return false;
	}

	m_entity_list = Helpers::get_interface<c_entity_list>(g::module_list[clientDLL], CLIENT_ENTITY_INTERFACE_VERSION);

	if (!m_entity_list) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to create entity list interface");
#endif
		return false;
	}

	m_engine = Helpers::get_interface<c_engine_client>(g::module_list[engineDLL], ENGINE_INTERFACE_VERSION);

	if (!m_engine) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to create engine interface");
#endif
		return false;
	}

	m_panel = Helpers::get_interface<c_panel>(g::module_list[vgui2DLL], VGUI_PANEL_INTERFACE_VERSION);

	if (!m_panel) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to create vgui panel interface");
#endif
		return false;
	}

	m_mat_system = Helpers::get_interface<c_material_system>(g::module_list[materialsystemDLL], MATERIAL_SYSTEM_INTERFACE_VERSION);

	if (!m_mat_system) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to create material system interface");
#endif
		return false;
	}

	m_cvar = Helpers::get_interface<c_base_convars>(g::module_list[vstdlibDLL], ENGINE_CVAR_INTERFACE_VERSION);

	if (!m_cvar) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to create engine cvar interface");
#endif
		return false;
	}

	m_trace = Helpers::get_interface<c_trace_ray>(g::module_list[engineDLL], TRACE_INTERFACE_VERSION);

	if (!m_trace) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to create engine trace interface");
#endif
		return false;
	}

	m_model_info = Helpers::get_interface<c_model_info>(g::module_list[engineDLL], MODEL_INFO_INTERFACE_VERSION);

	if (!m_model_info) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to create model info interface");
#endif
		return false;
	}

	m_model_render = Helpers::get_interface<c_model_render>(g::module_list[engineDLL], ENGINE_MODEL_INTERFACE_VERSION);

	if (!m_model_render) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to create model render interface");
#endif
		return false;
	}

	m_render_view = Helpers::get_interface<c_render_view>(g::module_list[engineDLL], ENGINE_RENDER_VIEW_INTERFACE_VERSION);

	if (!m_render_view) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to create engine render view interface");
#endif
		return false;
	}

	m_event_manager = Helpers::get_interface<c_game_event_manager2>(g::module_list[engineDLL], GAME_EVENT_MANAGER_INTERFACE_VERSION);

	if (!m_event_manager) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to create game event manager interface");
#endif
		return false;
	}

	return true;
}

bool interfaces::make_pointers()
{
	m_device = *Helpers::read<IDirect3DDevice9**>(reinterpret_cast<uintptr_t>(g_sig.s_device));

	if (!m_device) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to make device pointer");
#endif
		return false;
	}

	m_globals = *Helpers::read<c_global_vars**>((*reinterpret_cast<uintptr_t**>(m_client))[11] + 10);

	if (!m_globals) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to make global vars pointer");
#endif
		return false;
	}

	m_client_mode = *Helpers::read<uintptr_t**>((*reinterpret_cast<uintptr_t**>(m_client))[10] + 5);

	if (!m_client_mode) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to make client mode pointer");
#endif
		return false;
	}

	m_input = Helpers::read<i_input*>(reinterpret_cast<uintptr_t>(g_sig.s_input));

	if (!m_input) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to make input pointer");
#endif
		return false;
	}

	m_weapon_system = Helpers::read<c_weapon_system*>(reinterpret_cast<uintptr_t>(g_sig.s_weapon_system));

	if (!m_weapon_system) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to make weapon system pointer");
#endif
		return false;
	}

	m_glow_manager = Helpers::read_ptr<c_glow_manager*>(*reinterpret_cast<uintptr_t*>(g_sig.s_glow_manager));

	if (!m_glow_manager) {
#ifdef _DEBUG
		_DBG_NOTIFY("Failed to make glow manager pointer");
#endif
		return false;
	}

	return true;
}