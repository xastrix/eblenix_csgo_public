#include "base_client.h"

c_client_class* c_base_client::get_client_classes()
{
	using original_fn = c_client_class*(__thiscall*)(c_base_client*);
	return (*(original_fn**)this)[8](this);
}

bool c_base_client::dispatch_user_message(int msg_type, unsigned int arg1, unsigned int length, const void* data)
{
	using original_fn = bool(__thiscall*)(c_base_client*, int, unsigned int, unsigned int, const void*);
	return (*(original_fn**)this)[38](this, msg_type, arg1, length, data);
}

bool c_base_client::is_chat_raised()
{
	using original_fn = bool(__thiscall*)(c_base_client*);
	return (*(original_fn**)this)[90](this);
}