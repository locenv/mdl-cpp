#include <locenv/api.hpp>
#include <locenv/context.hpp>

#include <cstring>

using std::filesystem::path;

namespace locenv {
	context::context(const bootstrap_context &bc) :
		module_name(bc.name),
		working_directory(bc.working_directory, path::format::native_format)
	{
	}

	context::~context()
	{
	}

	const context &context::get(lua lua, int index)
	{
		// Check if it is a context.
		if (!api->lua_getmetatable(lua, index)) {
			api->aux_error(lua, "expect a module context at #%d", index);
		}

		if (api->lua_getfield(lua, -1, "__name") != LUA_TSTRING) {
			lua_pop(lua, 2);
			api->aux_error(lua, "invalid metatable for the value at #%d", index);
		}

		auto type = api->lua_tolstring(lua, -1, nullptr);

		if (!type || std::strcmp(type, "locenv") == 0 || std::strchr(type, '.')) {
			lua_pop(lua, 2);
			api->aux_error(lua, "expect a module context at #%d", index);
		}

		lua_pop(lua, 2);

		// Get userdata
		return *reinterpret_cast<context *>(api->lua_touserdata(lua, index));
	}
}
