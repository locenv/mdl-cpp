#ifndef LOCENV_API_HPP_INCLUDED
#define LOCENV_API_HPP_INCLUDED

#include "context.hpp"
#include "lua.hpp"

#include <cinttypes>
#include <cstddef>
#include <sstream>
#include <utility>

namespace locenv {
	struct api_table {
		std::uint32_t revision;
		void (*lua_pushboolean) (lua, int);
		void (*lua_pushcclosure) (lua, lua_function, int);
		const char * (*lua_pushfstring) (lua, const char *, ...);
		void (*lua_pushinteger) (lua, long long);
		void (*lua_pushlightuserdata) (lua, void *);
		const char * (*lua_pushlstring) (lua, const char *, std::size_t);
		void (*lua_pushnil) (lua);
		void (*lua_pushnumber) (lua, double);
		const char * (*lua_pushstring) (lua, const char *);
		int (*lua_pushthread) (lua);
		void (*lua_pushvalue) (lua, int);
		const char * (*lua_pushvfstring) (lua, const char *, void *);
		void (*lua_createtable) (lua, int, int);
		void * (*lua_newuserdatauv) (lua, std::size_t, int);
		void (*lua_settable) (lua, int);
		void (*lua_rawset) (lua, int);
		void (*lua_seti) (lua, int, long long);
		void (*lua_rawseti) (lua, int, long long);
		void (*lua_setfield) (lua, int, const char *);
		void (*lua_rawsetp) (lua, int, const void *);
		int (*lua_setmetatable) (lua, int);
		int (*lua_setiuservalue) (lua, int, int);
		int (*lua_iscfunction) (lua, int);
		int (*lua_isinteger) (lua, int);
		int (*lua_isnumber) (lua, int);
		int (*lua_isstring) (lua, int);
		int (*lua_isuserdata) (lua, int);
		int (*lua_type) (lua, int);
		const char * (*lua_typename) (lua, int);
		int (*lua_getmetatable) (lua, int);
		int (*lua_toboolean) (lua, int);
		lua_function (*lua_tocfunction) (lua, int);
		long long (*lua_tointegerx) (lua, int, int *);
		const char * (*lua_tolstring) (lua, int, std::size_t *);
		double (*lua_tonumberx) (lua, int, int *);
		const void * (*lua_topointer) (lua, int);
		lua (*lua_tothread) (lua, int);
		void * (*lua_touserdata) (lua, int);
		int (*lua_geti) (lua, int, long long);
		int (*lua_rawgeti) (lua, int, long long);
		int (*lua_gettable) (lua, int);
		int (*lua_rawget) (lua, int);
		int (*lua_getfield) (lua, int, const char *);
		int (*lua_rawgetp) (lua, int, const void *);
		int (*lua_next) (lua, int);
		int (*lua_getiuservalue) (lua, int, int);
		int (*lua_getglobal) (lua, const char *);
		void (*lua_setglobal) (lua, const char *);
		int (*lua_gettop) (lua);
		void (*lua_settop) (lua, int);
		void (*lua_callk) (lua, int, int, std::intptr_t, lua_continue);
		int (*lua_pcallk) (lua, int, int, int, std::intptr_t, lua_continue);
		int (*lua_error) (lua);
		void (*lua_warning) (lua, const char *, int);
		int (*lua_checkstack) (lua, int);
		int (*lua_absindex) (lua, int);
		void (*lua_copy) (lua, int, int);
		void (*lua_rotate) (lua, int, int);
		void (*lua_len) (lua, int);
		unsigned long long (*lua_rawlen) (lua, int);
		int (*lua_compare) (lua, int, int, int);
		int (*lua_rawequal) (lua, int, int);
		void (*lua_arith) (lua, int);
		void (*lua_concat) (lua, int);
		int (*lua_load) (lua, lua_reader, void *, const char *, const char *);
		int (*lua_dump) (lua, lua_writer, void *, int);
		void (*lua_toclose) (lua, int);
		void (*lua_closeslot) (lua, int);
		std::size_t (*lua_stringtonumber) (lua, const char *);
		lua_alloc (*lua_getallocf) (lua, void **);
		int (*lua_gc) (lua, int, ...);
		double (*lua_version) (lua);
		void (*aux_checkany) (lua, int);
		long long (*aux_checkinteger) (lua, int);
		const char * (*aux_checklstring) (lua, int, std::size_t *);
		double (*aux_checknumber) (lua, int);
		int (*aux_checkoption) (lua, int, const char *, const char **);
		void * (*aux_checkudata) (lua, int, const char *);
		void * (*aux_testudata) (lua, int, const char *);
		void (*aux_checktype) (lua, int, int);
		int (*aux_typeerror) (lua, int, const char *);
		int (*aux_argerror) (lua, int, const char *);
		long long (*aux_optinteger) (lua, int, long long);
		const char * (*aux_optlstring) (lua, int, const char *, std::size_t *);
		double (*aux_optnumber) (lua, int, double);
		int (*aux_error) (lua, const char *, ...);
		void (*aux_checkstack) (lua, int, const char *);
		const char * (*aux_tolstring) (lua, int, std::size_t *);
		long long (*aux_len) (lua, int);
		int (*aux_getsubtable) (lua, int, const char *);
		int (*aux_ref) (lua, int);
		void (*aux_unref) (lua, int, int);
		int (*aux_newmetatable) (lua, const char *);
		void (*aux_setmetatable) (lua, const char *);
		int (*aux_callmeta) (lua, int, const char *);
		int (*aux_getmetafield) (lua, int, const char *);
		int (*aux_loadstring) (lua, const char *);
		int (*aux_loadfilex) (lua, const char *, const char *);
		int (*aux_loadbufferx) (lua, const char *, size_t, const char *, const char *);
		void (*aux_setfuncs) (lua, const lua_reg *, int);
		void (*aux_where) (lua, int);
		void (*aux_traceback) (lua, lua, const char *, int);
		const char * (*aux_gsub) (lua, const char *, const char *, const char *);
		int (*aux_execresult) (lua, int);
		int (*aux_fileresult) (lua, int, const char *);
		std::uint32_t (*module_configurations_path) (const void *, const char *, char *, std::uint32_t);
	};

	extern const api_table *api;

	void lua_pop(lua lua, int count);

	inline int lua_upvalue(int i)
	{
		return LUA_REGISTRYINDEX - i;
	}

	template<class T, class... Args>
	void lua_new_userdata(lua lua, int context, Args &&...args)
	{
		// Get table name.
		std::stringstream table;

		context = api->lua_absindex(lua, context);

		table << ::locenv::context::get(lua, context).module_name;
		table << '.';
		table << typeid(T).name();

		// Create userdata.
		auto ud = api->lua_newuserdatauv(lua, sizeof(T), 0);

		try {
			new(ud) T(std::forward<Args>(args)...);
		} catch (...) {
			lua_pop(lua, 1);
			throw;
		}

		// Associate the userdata with metatable.
		if (api->aux_newmetatable(lua, table.str().c_str())) {
			api->lua_pushvalue(lua, context);
			api->lua_pushcclosure(lua, [](::locenv::lua l) -> int {
				std::stringstream t;

				t << ::locenv::context::get(l, lua_upvalue(1)).module_name;
				t << '.';
				t << typeid(T).name();

				reinterpret_cast<T *>(api->aux_checkudata(l, 1, t.str().c_str()))->~T();

				return 0;
			}, 1);

			api->lua_setfield(lua, -2, "__gc");
		}

		api->lua_setmetatable(lua, -2);
	}
}

#endif // LOCENV_API_HPP_INCLUDED
