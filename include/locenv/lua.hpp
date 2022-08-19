#ifndef LOCENV_LUA_HPP_INCLUDED
#define LOCENV_LUA_HPP_INCLUDED

#include <cinttypes>
#include <cstddef>
#include <limits>

namespace locenv {
	constexpr bool LUAI_IS32INT = (std::numeric_limits<unsigned int>::max() >> 30) >= 3;
	constexpr int LUAI_MAXSTACK = LUAI_IS32INT ? 1000000 : 15000;
	constexpr int LUA_REGISTRYINDEX = -LUAI_MAXSTACK - 1000;

	constexpr int LUA_TNIL = 0;
	constexpr int LUA_TBOOLEAN = 1;
	constexpr int LUA_TLIGHTUSERDATA = 2;
	constexpr int LUA_TNUMBER = 3;
	constexpr int LUA_TSTRING = 4;
	constexpr int LUA_TTABLE = 5;
	constexpr int LUA_TFUNCTION = 6;
	constexpr int LUA_TUSERDATA = 7;
	constexpr int LUA_TTHREAD = 8;

	typedef struct lua_state *lua;
	typedef int (*lua_function) (lua);
	typedef void * (*lua_alloc) (void *, void *, std::size_t, std::size_t);
	typedef int (*lua_continue) (lua, int, std::intptr_t);
	typedef const char * (*lua_reader) (lua, void *, std::size_t *);
	typedef int (*lua_writer) (lua, const void *, std::size_t, void *);

	struct lua_reg {
		const char *name;
		lua_function func;
	};
}

#endif // LOCENV_LUA_HPP_INCLUDED
