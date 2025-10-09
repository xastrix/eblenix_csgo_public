#pragma once

struct hooks {
	void init();
	void undo();
};

extern hooks g_hooks;