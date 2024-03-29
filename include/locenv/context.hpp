#ifndef LOCENV_CONTEXT_HPP_INCLUDED
#define LOCENV_CONTEXT_HPP_INCLUDED

#include "lua.hpp"

#include <cinttypes>
#include <filesystem>
#include <string>

namespace locenv {
	struct bootstrap_context {
		std::uint32_t revision;
		const char *name;
		const void *locenv;
		::locenv::lua lua;
		const char *working_directory;
	};

	class context final {
	public:
		std::string module_name;
		std::filesystem::path working_directory;

	public:
		context(const bootstrap_context &bc);
		context(const context &) = delete;
		~context();

	public:
		context &operator=(const context &) = delete;

	public:
		static const context &get(lua lua, int index);
	};
}

#endif // LOCENV_CONTEXT_HPP_INCLUDED
