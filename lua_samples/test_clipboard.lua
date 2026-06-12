-- Testing clipboard

-- Register our custom clipboard format
local format = clip.register_format("1337clipboardformat")

-- Define our 32 byte (256) key for AES-256
local key    = "YOU-NEED-TO-CREATE-A-32-CHAR-KEY"

-- Encrypt the text using the custom format and the key
clip.aes_crypt(format, key, "Encrypted message!")

h = register_callback(cb.on_frame_stage_notify, function()
	print("==== *LUA* Testing clipboard ===============================\n")

	-- Print the decrypted text to the console
	print("aes decrypted data: %s\n", clip.aes_decrypt(format, key))

	-- Populate the clipboard with standard text data (1 = CF_TEXT)
	clip.set_data(1, "Hello from clipboard!")

	-- Print the newly added CF_TEXT data to the console
	print("CF_TEXT data: %s\n", clip.get_data(1))
	print("============================================================\n")

	-- Clear the clipboard contents
	clip.set_empty()

	-- Unregister the callback to prevent infinite loops
	unregister_callback(cb.on_frame_stage_notify, h)
end)