-- Testing memory

-- Offsets
local dwEntityList    = 0x4E051DC
local dwRadarBase     = 0x523BCB4
local m_iHealth       = 0x100
-- Protocol version 13881 [1575/2000258]
-- Exe version 1.38.8.1 (csgo)

local client          = mem.get_module("client.dll")

h = register_callback(cb.on_frame_stage_notify, function()
	print("==== *LUA* Player list (Testing memory) ====================\n")

	local radar_base = mem.r32(client.addr + dwRadarBase)

	for i = 0, 64 do
		local player = mem.r32(client.addr + dwEntityList + (i * 0x10))

		if player ~= 0 then
			local name   = ""
			local health = mem.r32(player + m_iHealth)

			if radar_base ~= 0 then
				local radar_hud = mem.r32(radar_base + 0x78)

				if radar_hud ~= 0 then
					name = mem.rs(radar_hud + 0x174 * (i + 2) + 0x18)
					
					if name ~= "" and health > 0 then
						print("[player_list] \"%s\" has %ihp\n", name, health)
					end
				end
			end
		end
	end

	print("============================================================\n")

	unregister_callback(cb.on_frame_stage_notify, h)
end)