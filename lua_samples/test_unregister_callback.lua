-- Testing unregistering callback functions

h = register_callback(cb.on_frame_stage_notify, function()
	print("test_unregister_callback.lua: a function called once\n")
    
	-- Unregister this callback to prevent it from being called again
	unregister_callback(cb.on_frame_stage_notify, h)
end)