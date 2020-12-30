
#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"

#include "snap7.h"

namespace lua_module
{

sol::table open_module(sol::this_state L)
{
    sol::state_view lua(L);
    sol::table module = lua.create_table();

    module.set_function("CliErrorText", [](int err)             { return CliErrorText(err);     });
    module.set_function("SrvErrorText", [](int err)             { return SrvErrorText(err);     });
    module.set_function("ParErrorText", [](int err)             { return ParErrorText(err);     });
    module.set_function("SrvEventText", [](TSrvEvent* Event)    { return SrvEventText(Event);   });


    return module;
}

} // namespace lua_module

#if defined(_MSC_VER)
extern "C" __declspec(dllexport) int luaopen_exprtk(lua_State* L)
#else
extern "C" int luaopen_exprtk(lua_State* L)
#endif
{
    return sol::stack::call_lua(L, 1, lua_module::open_module);
}
