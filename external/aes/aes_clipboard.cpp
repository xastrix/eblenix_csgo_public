#include "aes_clipboard.hpp"
#include "aes.hpp"

#include <windows.h>

static void aes_encrypt(const std::string& key, std::string& data)
{
	size_t pad = 16 - (data.size() % 16);
	data.append(pad, static_cast<char>(pad));

	AES_ctx ctx;
	AES_init_ctx(&ctx, (const uint8_t*)key.c_str());

	for (int i = 0; i < data.size(); i += 16)
	{
		AES_ECB_encrypt(&ctx, (uint8_t*)&data[i]);
	}
}

static void aes_decrypt(const std::string& key, std::string& data)
{
	AES_ctx ctx;
	AES_init_ctx(&ctx, (const uint8_t*)key.c_str());

	for (int i = 0; i < data.size(); i += 16)
	{
		AES_ECB_decrypt(&ctx, (uint8_t*)&data[i]);
	}

	if (!data.empty())
	{
		uint8_t pad = static_cast<uint8_t>(data.back());
		if (pad > 0 && pad <= 16)
		{
			data.resize(data.size() - pad);
		}
	}
}

void aes_export_encrypted_to_clipboard(UINT format, const std::string& key, const std::string& plain_text)
{
	std::string data = plain_text;
	aes_encrypt(key, data);

	if (!OpenClipboard(nullptr))
		return;

	EmptyClipboard();

	size_t size = data.size();

	HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE, size + sizeof(size_t));
	if (!h) {
		CloseClipboard();
		return;
	}

	void* p = GlobalLock(h);
	memcpy(p, &size, sizeof(size_t));
	memcpy(static_cast<char*>(p) + sizeof(size_t), data.data(), size);
	GlobalUnlock(h);

	SetClipboardData(format, h);
	CloseClipboard();
}

std::string aes_import_decrypted_from_clipboard(uint32_t format, const std::string& key)
{
	if (!OpenClipboard(nullptr))
		return "";

	auto h = GetClipboardData(format);
	if (!h) {
		CloseClipboard();
		return "";
	}

	void* ptr = GlobalLock(h);
	if (!ptr) {
		CloseClipboard();
		return "";
	}

	size_t actual_size = 0;
	memcpy(&actual_size, ptr, sizeof(size_t));

	if (actual_size == 0) {
		GlobalUnlock(h);
		CloseClipboard();
		return "";
	}

	std::string encrypted(static_cast<char*>(ptr) + sizeof(size_t), actual_size);

	GlobalUnlock(h);
	CloseClipboard();

	aes_decrypt(key, encrypted);

	return encrypted;
}