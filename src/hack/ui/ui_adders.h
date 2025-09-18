#pragma once

#include "ui_globals.h"

struct ui_adders : public ui_globals {
	void add(const std::wstring& name, bool space = false);
	void add_bool(const std::wstring& name, const std::string& var, bool space = false);
	void add_int(const std::wstring& name, const std::string& var, int min, int max, int step, const  bool space = false);
	void add_float(const std::wstring& name, const std::string& var, float min, float max, float step, bool space = false);
	void add_item(const std::wstring& name, const std::string& var, std::vector<std::wstring> items, bool space = false);
	void add_key(const std::wstring& name, const std::string& var, bool space = false);
	void add_function(const std::wstring& name, std::function<void(void)> fn, bool space = false);
	void add_tab(const std::wstring& name, bool space = false);
	
	void add_sub_bool(const std::wstring& name, const std::string& var, bool space = false);
	void add_sub_int(const std::wstring& name, const std::string& var, int min, int max, int step, bool space = false);
	void add_sub_float(const std::wstring& name, const std::string& var, float min, float max, float step, bool space = false);
	void add_sub_item(const std::wstring& name, const std::string& var, std::vector<std::wstring> items, bool space = false);
	void add_sub_key(const std::wstring& name, const std::string& var, bool space = false);
	void add_sub_function(const std::wstring& name, std::function<void(void)> fn, bool space = false);
	void add_sub_tab(const std::wstring& name, bool space = false);
	
	void add_sub_sub_bool(const std::wstring& name, const std::string& var, bool space = false);
	void add_sub_sub_int(const std::wstring& name, const std::string& var, int min, int max, int step, bool space = false);
	void add_sub_sub_float(const std::wstring& name, const std::string& var, float min, float max, float step, bool space = false);
	void add_sub_sub_item(const std::wstring& name, const std::string& var, std::vector<std::wstring> items, bool space = false);
	void add_sub_sub_key(const std::wstring& name, const std::string& var, bool space = false);
	void add_sub_sub_function(const std::wstring& name, std::function<void(void)> fn, bool space = false);
};