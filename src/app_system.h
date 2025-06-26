#pragma once

class c_app_system {
public:
	virtual bool        connect(void* factory) = 0;
	virtual void        disconnect() = 0;
	virtual void*       query_interface(const char* interface_name) = 0;
	virtual int         init() = 0;
	virtual void        shutdown() = 0;
	virtual const void* get_dependencies() = 0;
	virtual int         get_tier() = 0;
	virtual void        reconnect(void* factory, const char* interface_name) = 0;
	virtual void        unknown() = 0;
};