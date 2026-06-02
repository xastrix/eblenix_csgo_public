-- Testing command line

h = register_callback(cb.on_frame_stage_notify, function()
	print("==== *LUA* Testing command line ===============================\n")
	print("cmd-line: %s\n", cmdl.get_cmd_line())
	print("steam parameter: %s\n", tostring(cmdl.has_parm("-steam")))
	print("cmd-count: %i\n", cmdl.get_parm_count())
	print("============================================================\n")

	unregister_callback(cb.on_frame_stage_notify, h)
end)