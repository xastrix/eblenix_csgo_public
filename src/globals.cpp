#include "globals.h"

#include "ui.h"
#include "helpers.h"

static long long get_elapsed_time(const std::chrono::steady_clock::time_point start_time)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count();
}

void g::handle_playing_time(const std::chrono::steady_clock::time_point start_time, int interval)
{
	static int last_min  = 0;
	static int last_hour = 0;

	int total_minutes    = 0;
	int total_hours      = 0;

	std::this_thread::sleep_for(std::chrono::milliseconds(interval));

	total_minutes        = get_elapsed_time(start_time) / 60000;
	total_hours          = total_minutes / 60;

	if (total_hours != last_hour) {
		last_hour = total_hours;

		i_flags[IF_HOURS_IN_GAME] = 0;

		i_flags[IF_MINUTES_IN_GAME] = 0;
		i_flags[IF_SECONDS_IN_GAME] = 0;

		last_min = 0;
	}

	if (total_minutes != last_min) {
		last_min = total_minutes;
		i_flags[IF_SECONDS_IN_GAME] = 0;
	}
	else {
		i_flags[IF_SECONDS_IN_GAME] = (get_elapsed_time(start_time) % 60000) / 1000;
	}

	i_flags[IF_MINUTES_IN_GAME] = total_minutes % 60;
	i_flags[IF_HOURS_IN_GAME] = total_hours;
}

void g::unload()
{
	if (g_ui->get_menu_state())
		g_ui->set_menu_state(false);

	lib_state.set_state(state_t::SL_SHUTDOWN);
}