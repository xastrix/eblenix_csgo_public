#ifndef _AES_CLIPBOARD_HPP_
#define _AES_CLIPBOARD_HPP_

#include <string>

void aes_export_encrypted_to_clipboard(uint32_t format, const std::string& key, const std::string& plain_text);
std::string aes_import_decrypted_from_clipboard(uint32_t format, const std::string& key);

#endif // _AES_CLIPBOARD_HPP_