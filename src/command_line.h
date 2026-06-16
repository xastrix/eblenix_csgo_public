#pragma once

class c_command_line {
public:
	const char* get_cmd_line() {
		using original_fn = const char*(__thiscall*)(c_command_line*);
		return (*(original_fn**)this)[2](this);
	}

	bool has_parm(const char* parm) {
		using original_fn = bool(__thiscall*)(c_command_line*, const char*);
		return (*(original_fn**)this)[4](this, parm);
	}

	int parm_count() {
		using original_fn = int(__thiscall*)(c_command_line*);
		return (*(original_fn**)this)[10](this);
	}
};

using command_line_t = c_command_line*(__stdcall*)();