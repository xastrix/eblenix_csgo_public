-- Testing game events

register_callback(cb.on_game_events, function(e)
	if e:get_name() == "player_hurt" then
		convar.console_printf("*LUA* game event: player hurt!\n")

	elseif e:get_name() == "player_death" then
		convar.console_printf("*LUA* game event: player death!\n")

	elseif e:get_name() == "item_purchase" then
		convar.console_printf("*LUA* game event: item purchase!\n")

	elseif e:get_name() == "player_disconnect" then
		convar.console_printf("*LUA* game event: player disconnect!\n")

	elseif e:get_name() == "round_prestart" then
		convar.console_printf("*LUA* game event: round pre-start!\n")

	elseif e:get_name() == "round_start" then
		convar.console_printf("*LUA* game event: round start!\n")

	elseif e:get_name() == "bomb_planted" then
		convar.console_printf("*LUA* game event: bomb planted!\n")

	elseif e:get_name() == "bomb_defused" then
		convar.console_printf("*LUA* game event: bomb defused!\n")

	elseif e:get_name() == "bomb_exploded" then
		convar.console_printf("*LUA* game event: bomb exploded!\n")
	end
end)