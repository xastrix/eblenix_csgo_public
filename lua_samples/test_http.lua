-- Test GET requests from the example.com website

local target = "https://example.com/"

h = register_callback(cb.on_frame_stage_notify, function()

	http.get(target, function(response)
		print("==== *LUA* Testing http requests ===============================\n")
		print("url: %s\n", target)
		
		if response and response.code ~= -1 then
			print("code: %d\n", response.code)
			print("resp: \n%s\n", response.resp)
		else
			print("resp: http request failed (check internet connection or url)\n")
		end
		
		print("================================================================\n")
	end)
	
	unregister_callback(cb.on_frame_stage_notify, h)
end)