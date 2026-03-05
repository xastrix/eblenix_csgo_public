#include "interfaces.h"

#include "signatures.h"
#include "globals.h"
#include "helpers.h"

void c_csgo::init()
{
	m_client = Helpers::get_interface<c_base_client>(GLOBAL(module_list[clientDLL]), CLIENT_INTERFACE_VERSION);
	m_entity_list = Helpers::get_interface<c_entity_list>(GLOBAL(module_list[clientDLL]), CLIENT_ENTITY_INTERFACE_VERSION);
	m_engine = Helpers::get_interface<c_engine_client>(GLOBAL(module_list[engineDLL]), ENGINE_INTERFACE_VERSION);
	m_panel = Helpers::get_interface<c_panel>(GLOBAL(module_list[vgui2DLL]), VGUI_PANEL_INTERFACE_VERSION);
	m_surface = Helpers::get_interface<c_surface_draw_manager>(GLOBAL(module_list[vguiMatSurfaceDLL]), VGUI_SURFACE_INTERFACE_VERSION);
	m_mat_system = Helpers::get_interface<c_material_system>(GLOBAL(module_list[materialsystemDLL]), MATERIAL_SYSTEM_INTERFACE_VERSION);
	m_cvar = Helpers::get_interface<c_base_convars>(GLOBAL(module_list[vstdlibDLL]), ENGINE_CVAR_INTERFACE_VERSION);
	m_trace = Helpers::get_interface<c_engine_trace>(GLOBAL(module_list[engineDLL]), ENGINE_TRACE_INTERFACE_VERSION);
	m_model_info = Helpers::get_interface<c_model_info>(GLOBAL(module_list[engineDLL]), MODEL_INFO_INTERFACE_VERSION);
	m_model_render = Helpers::get_interface<c_model_render>(GLOBAL(module_list[engineDLL]), ENGINE_MODEL_INTERFACE_VERSION);
	m_render_view = Helpers::get_interface<c_render_view>(GLOBAL(module_list[engineDLL]), ENGINE_RENDER_VIEW_INTERFACE_VERSION);
	m_event_manager = Helpers::get_interface<c_game_event_manager2>(GLOBAL(module_list[engineDLL]), GAME_EVENT_MANAGER_INTERFACE_VERSION);
	
	m_device = *Helpers::read<IDirect3DDevice9**>(reinterpret_cast<uintptr_t>(SIG(S_DEVICE)));
	m_globals = *Helpers::read<c_global_vars**>((*reinterpret_cast<uintptr_t**>(m_client))[11] + 10);
	m_client_mode = *Helpers::read<uintptr_t**>((*reinterpret_cast<uintptr_t**>(m_client))[10] + 5);
	m_input = Helpers::read<input_t*>(reinterpret_cast<uintptr_t>(SIG(S_INPUT)));
	m_weapon_system = Helpers::read<c_weapon_system*>(reinterpret_cast<uintptr_t>(SIG(S_WEAPON_SYSTEM)));
	m_glow_manager = Helpers::read_ptr<c_glow_manager*>(*reinterpret_cast<uintptr_t*>(SIG(S_GLOW_MANAGER)));
}

void c_csgo::init_local(const local_t& local)
{
	m_local = local;
}