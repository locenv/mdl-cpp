#include <locenv/api.hpp>
#include <locenv/lua.hpp>

namespace locenv {
	void lua_pop(lua lua, int count)
	{
		api->lua_settop(lua, -count - 1);
	}
}
