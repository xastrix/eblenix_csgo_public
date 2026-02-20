#pragma once

struct input_t {
	int in_third_person_mode(int slot = -1) {
		using original_fn = int(__thiscall*)(input_t*, int);
		return (*(original_fn**)this)[32](this, slot);
	}

	void to_third_person() {
		using original_fn = void(__thiscall*)(input_t*);
		return (*(original_fn**)this)[35](this);
	}

	void to_first_person() {
		using original_fn = void(__thiscall*)(input_t*);
		return (*(original_fn**)this)[36](this);
	}
};