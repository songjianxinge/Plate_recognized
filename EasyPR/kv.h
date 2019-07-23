#pragma once
#include <map>
#include <string>
#include <iostream>
namespace easypr {
	class Kv
	{
	public:
		Kv();

		void load(const std::string &file);

		std::string get(const std::string &key);

		void add(const std::string &key, const std::string &value);

		void remove(const std::string &key);

		void clear();

	private:
		std::map<std::string, std::string> data_;
	};

}