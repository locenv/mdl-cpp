#ifndef LOCENV_LOADER_HPP_INCLUDED
#define LOCENV_LOADER_HPP_INCLUDED

#include "api.hpp"
#include "context.hpp"
#include "lua.hpp"

#include <exception>

#define LOCENV_MODULE_LOADER(loader) \
	extern "C" LOCENV_EXPORT int bootstrap(const locenv::bootstrap_context *context, const locenv::api_table *api) { \
		auto lua = context->lua; \
		if (!locenv::api) locenv::api = api; \
		api->lua_pushcclosure(lua, (loader), 0); \
		auto data = api->lua_newuserdatauv(lua, sizeof(locenv::context), 0); \
		try { \
			new(data) locenv::context(*context); \
		} catch (std::exception &e) { \
			locenv::lua_pop(lua, 2); \
			api->lua_pushstring(lua, e.what()); \
			return 1; \
		} \
		if (!api->aux_newmetatable(lua, context->name)) { \
			locenv::lua_pop(lua, 3); \
			api->lua_pushfstring(lua, "someone already created a metatable named '%s'", context->name); \
			return 1; \
		} \
		api->lua_pushstring(lua, context->name); \
		api->lua_pushcclosure(lua, [](locenv::lua l) -> int { \
			auto table = locenv::api->aux_checklstring(l, locenv::lua_upvalue(1), nullptr); \
			auto data = reinterpret_cast<locenv::context *>(locenv::api->aux_checkudata(l, 1, table)); \
			data->~context(); \
			return 0; \
		}, 1); \
		api->lua_setfield(lua, -2, "__gc"); \
		api->lua_setmetatable(lua, -2); \
		return 2; \
	}

#ifdef _MSC_VER
#define LOCENV_EXPORT __declspec(dllexport)
#else
#define LOCENV_EXPORT __attribute__((visibility("default")))
#endif

#endif // LOCENV_LOADER_HPP_INCLUDED
