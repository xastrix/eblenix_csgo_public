#pragma once

#include <cstdint>

struct player_info_t {
	int64_t       u0;
	int           xuid_low;
	int           xuid_high;
	char          m_player_name[256];
	int           m_user_id;
	char          m_steam_id[33];
	unsigned int  m_steam3_id;
	char          m_friends_name[256];
	bool          m_is_fake_player;
	bool          m_is_hltv;
	unsigned int  m_custom_files[4];
	unsigned char m_files_downloaded;
};