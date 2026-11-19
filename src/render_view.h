#pragma once

#define ENGINE_RENDER_VIEW_INTERFACE_VERSION "VEngineRenderView014"

class c_render_view {
public:
	void set_blend(float blend);
	void modulate_color(float const* blend);
};