-- Testing unregistering callback functions

h = register_callback(cb.on_present, function()
	print("test_unregister_callback.lua: a function called once\n")
    
	-- Unregister this callback to prevent it from being called again
	unregister_callback(cb.on_present, h)
end)