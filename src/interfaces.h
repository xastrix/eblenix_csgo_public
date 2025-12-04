#pragma once

#include <string>
#include <d3dx9.h>

#include "sdk.h"

struct interfaces {
	c_base_client* m_client;
	c_entity_list* m_entity_list;
	c_engine_client* m_engine;
	c_panel* m_panel;
	c_global_vars* m_globals;
	c_material_system* m_mat_system;
	c_base_convars* m_cvar;
	c_trace_ray* m_trace;
	c_model_info* m_model_info;
	c_model_render* m_model_render;
	c_render_view* m_render_view;
	c_game_event_manager2* m_event_manager;
	IDirect3DDevice9* m_device;
	uintptr_t* m_client_mode;
	i_input* m_input;
	c_weapon_system* m_weapon_system;
	c_glow_manager* m_glow_manager;
};

struct local_t {
	local_t() = default;
	local_t(c_base_player* ptr) : m_ptr(ptr) {};

	c_base_player* get() const {
		return m_ptr;
	}

	bool operator!() const {
		return (m_ptr == nullptr);
	}
private:
	c_base_player* m_ptr{};
};

struct CSGO : public interfaces {
	void init();

	void init_local(const local_t& local);
	c_base_player* get_local();

private:
	local_t m_local{};
};

inline CSGO g_csgo;