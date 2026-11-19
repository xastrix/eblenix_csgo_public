#pragma once

class c_global_vars {
public:
	float real_time;
	int   frame_count;
	float absolute_frametime;
	float absolute_frame_start_time;
	float cur_time;
	float frame_time;
	int   max_clients;
	int   tick_count;
	float interval_per_tick;
};