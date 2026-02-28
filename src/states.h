#pragma once

// lambda
#include <functional>

// ::shared_ptr
#include <memory>

enum state_list {
	SL_INIT_BASE,
	SL_INIT_VARS,
	SL_INIT_HOOKS,
	SL_WAITING_FOR_SHUTDOWN,
	SL_SHUTDOWN,
};

// Type alias for state index
using state_t = int;

class c_state_mgr {
public:
	using s_fn = std::function<void(state_t&)>;

	void call_state(state_t state, s_fn fn) {
		if (m_state == state) fn(m_state);
	}

	static std::shared_ptr<c_state_mgr> make_shared() {
		return std::shared_ptr<c_state_mgr>(new c_state_mgr());
	}

	void set_current_state(state_t state) {
		m_state = state;
	}

	state_t get_current_state() {
		return m_state;
	}

private:
	state_t m_state{};
};

inline std::shared_ptr<c_state_mgr> g_state = c_state_mgr::make_shared();