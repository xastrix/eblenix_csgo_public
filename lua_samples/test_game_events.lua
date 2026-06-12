-- Testing game events

register_callback(cb.on_game_events, function(e)
	if e:get_name() == event_list[e_list.player_hurt] then
		print("test_game_events.lua: player hurt!\n")

	elseif e:get_name() == event_list[e_list.player_death] then
		print("test_game_events.lua: player death!\n")

	elseif e:get_name() == event_list[e_list.player_spawn] then
		print("test_game_events.lua: player spawn!\n")

	elseif e:get_name() == event_list[e_list.player_connect] then
		print("test_game_events.lua: player connect!\n")

	elseif e:get_name() == event_list[e_list.player_disconnect] then
		print("test_game_events.lua: player disconnect!\n")

	elseif e:get_name() == event_list[e_list.player_footstep] then
		print("test_game_events.lua: player footstep!\n")

	elseif e:get_name() == event_list[e_list.player_falldamage] then
		print("test_game_events.lua: player fall damage!\n")

	elseif e:get_name() == event_list[e_list.player_given_c4] then
		print("test_game_events.lua: player given c4!\n")

	elseif e:get_name() == event_list[e_list.hostage_hurt] then
		print("test_game_events.lua: hostage hurt!\n")

	elseif e:get_name() == event_list[e_list.hostage_killed] then
		print("test_game_events.lua: hostage killed!\n")

	elseif e:get_name() == event_list[e_list.hostage_rescued] then
		print("test_game_events.lua: hostage rescued!\n")

	elseif e:get_name() == event_list[e_list.item_purchase] then
		print("test_game_events.lua: item purchase!\n")

	elseif e:get_name() == event_list[e_list.round_prestart] then
		print("test_game_events.lua: round prestart!\n")

	elseif e:get_name() == event_list[e_list.round_start] then
		print("test_game_events.lua: round start!\n")

	elseif e:get_name() == event_list[e_list.round_end] then
		print("test_game_events.lua: round end!\n")

	elseif e:get_name() == event_list[e_list.bomb_beginplant] then
		print("test_game_events.lua: bomb begin plant!\n")

	elseif e:get_name() == event_list[e_list.bomb_planted] then
		print("test_game_events.lua: bomb planted!\n")

	elseif e:get_name() == event_list[e_list.bomb_exploded] then
		print("test_game_events.lua: bomb exploded!\n")

	elseif e:get_name() == event_list[e_list.bomb_begindefuse] then
		print("test_game_events.lua: bomb begin defuse!\n")

	elseif e:get_name() == event_list[e_list.bomb_abortdefuse] then
		print("test_game_events.lua: bomb abort defuse!\n")

	elseif e:get_name() == event_list[e_list.bomb_defused] then
		print("test_game_events.lua: bomb defused!\n")

	elseif e:get_name() == event_list[e_list.bomb_dropped] then
		print("test_game_events.lua: bomb dropped!\n")

	elseif e:get_name() == event_list[e_list.bomb_beep] then
		print("test_game_events.lua: bomb beep!\n")
	end
end)