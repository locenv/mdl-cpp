#ifndef LOCENV_CONTEXT_HPP_INCLUDED
#define LOCENV_CONTEXT_HPP_INCLUDED

#include "lua.hpp"

#include <cinttypes>
#include <filesystem>
#include <string>

namespace locenv {
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
	};

	struct bootstrap_context {
		std::uint32_t revision;
		const char *name;
		const void *locenv;
		::locenv::lua lua;
		const char *working_directory;
	};
}

#endif // LOCENV_CONTEXT_HPP_INCLUDED
