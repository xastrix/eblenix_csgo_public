#pragma once

#include <cstdint>

struct player_info_t {
	int64_t       u0;
	int           xuid_low;
	int           xuid_high;
	char          player_name[256];
	int           user_id;
	char          steam_id[33];
	unsigned int  steam3_id;
	char          friends_name[256];
	bool          is_fake_player;
	bool          is_hltv;
	unsigned int  custom_files[4];
	unsigned char files_downloaded;
};