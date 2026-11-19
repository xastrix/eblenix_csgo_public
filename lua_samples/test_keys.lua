-- In the game, press and hold the C key to check for UP/DOWN key presses

local wm_keydown = 0x0100
local wm_keyup = 0x0101

local vk_up = 0x26
local vk_down = 0x28
local vk_c = 0x43

local interval = 100
local last_time = 0

math.randomseed(os.time())
register_callback(cb.on_wndproc, function(m, w, l)
    if m == wm_keydown then
        if w == vk_c then
            local current_time = os.clock() * 1000
            if current_time - last_time > interval then
				local r = math.random(0, 1)
                local vk = r == 0 and vk_up or vk_down

				-- function for simulating key presses
                process_keybd_msg(wm_keydown, vk)
                process_keybd_msg(wm_keyup, vk)

                last_time = current_time
            end
        end
    end
end)