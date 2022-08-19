#include <locenv/api.hpp>

namespace locenv {
	const api_table *api;

	void lua_pop(lua lua, int count)
	{
		api->lua_settop(lua, -count - 1);
	}
}
