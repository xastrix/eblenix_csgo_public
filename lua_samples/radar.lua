-- A function that allows viewing other players on the radar

register_callback(cb.on_move, function()
	if (engine.is_connected()) then
		for i = 2, 64 do
			local player = entity_list.get_entity(i)

			if (player ~= nil and entity.get_health(player) > 0 and not entity.get_dormant(player)) then
				player:set_prop_bool("DT_BaseEntity::m_bSpotted", true)
			end
		end
	end
end)