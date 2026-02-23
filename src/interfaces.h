#pragma once

#include "base_client.h"
#include "entity_list.h"
#include "engine_client.h"
#include "panel.h"
#include "surface.h"
#include "global_vars.h"
#include "material_system.h"
#include "base_convars.h"
#include "trace_ray.h"
#include "model_info.h"
#include "model_render.h"
#include "render_view.h"
#include "game_event_mgr.h"
#include "d3d9.h"
#include "weapon_system.h"
#include "glow_manager.h"

#include "i_input.h"

// ::shared_ptr
#include <memory>

class c_interfaces {
public:
	c_base_client* m_client;
	c_entity_list* m_entity_list;
	c_engine_client* m_engine;
	c_panel* m_panel;
	c_surface_draw_manager* m_surface;
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
	input_t* m_input;
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

class c_csgo : public c_interfaces {
public:
	void init();

	static std::shared_ptr<c_csgo> make_shared() {
		return std::shared_ptr<c_csgo>(new c_csgo());
	}

	void init_local(const local_t& local);
	
	c_base_player* get_local() {
		if (!m_local)
			return nullptr;

		return m_local.get();
	}

private:
	local_t m_local{};
};

inline std::shared_ptr<c_csgo> g_cs = c_csgo::make_shared();