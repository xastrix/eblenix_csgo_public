#pragma once

#define GAME_EVENT_MANAGER_INTERFACE_VERSION "GAMEEVENTSMANAGER002"

class c_game_event {
public:
	const char* get_name() {
		using original_fn = const char*(__thiscall*)(c_game_event*);
		return (*(original_fn**)this)[1](this);
	}

	int get_int(const char* name = nullptr, int def = 0) {
		using original_fn = int(__thiscall*)(c_game_event*, const char*, int);
		return (*(original_fn**)this)[6](this, name, def);
	}

	const char* get_string(const char* name = nullptr, const char* def = "") {
		using original_fn = const char*(__thiscall*)(c_game_event*, const char*, const char*);
		return (*(original_fn**)this)[9](this, name, def);
	}
};

class c_game_event_listener2 {
public:
	virtual      ~c_game_event_listener2(void) {}
	virtual void fire_game_event(c_game_event* event) = 0;
	virtual int  get_event_debug_id(void) { return 42; };
};

class c_game_event_manager2 {
public:
	bool add_listener(c_game_event_listener2* listener, const char* name, bool server_side = false);
	int remove_listener(c_game_event_listener2* listener);
};