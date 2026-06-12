-- Drawing a simple esp box

local box = bbox(0, 0, 0, 0)

register_callback(cb.on_present, function()
	if (engine.is_connected()) then
		for i = 0, 64 do
			local player = entity_list.get_entity(i)

			if (player ~= nil and entity.get_health(player) > 0 and not entity.get_dormant(player)) then
				if math.get_bbox(player, box, bbox_type.box_static) then
					renderer.rect(box.x, box.y, box.w, box.h, color(255, 60, 20, 255))
				end
			end
		end
	end
end)