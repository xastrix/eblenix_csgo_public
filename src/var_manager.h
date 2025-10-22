#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <optional>

using var_type = std::variant<bool, int, float>;
using vars_t = std::vector<std::pair<std::string, var_type>>;

struct var_manager {
	void init();
	
	void reset() {
		init();
	}

	void set(const std::string& key, const var_type& value) {
		for (auto& var : vars)
		{
			if (var.first == key)
			{
				if (var.second != value)
					var.second = std::move(value);

				return;
			}
		}

		vars.emplace_back(std::move(key), std::move(value));
	}

	template <typename T>
	std::optional<T> get_as(const std::string& key)
	{
		for (const auto& v : vars)
		{
			if (v.first == key)
			{
				std::optional<var_type> opt{ v.second };

				if (opt.has_value()) {
					if (const auto val = std::get_if<T>(&opt.value())) {
						return *val;
					}
				}
			}
		}

		return std::nullopt;
	}

	vars_t get_vars() {
		return vars;
	}

	void undo();
private:
	vars_t vars{};
};

inline var_manager g_vars;