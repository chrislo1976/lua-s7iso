
#include "module_includes.h"


namespace lua_module
{

// forward declarations
void register_enums(sol::table& module);
void register_types(sol::table& module);
void register_client(sol::table& module);


sol::table open_module(sol::this_state L)
{
    sol::state_view lua(L);
    sol::table module = lua.create_table();

    register_enums(module);
    register_types(module);
    register_client(module);

    module.set_function("CliErrorText", [](int error)           { return CliErrorText(error); });
    module.set_function("SrvErrorText", [](int error)           { return SrvErrorText(error); });
    module.set_function("ParErrorText", [](int error)           { return ParErrorText(error); });
    module.set_function("SrvEventText", [](TSrvEvent* event)    { return SrvEventText(event); });


    return module;
}

} // namespace lua_module

#if defined(_MSC_VER)
extern "C" __declspec(dllexport) int luaopen_s7iso(lua_State* L)
#else
extern "C" int luaopen_s7iso(lua_State* L)
#endif
{
    return sol::stack::call_lua(L, 1, lua_module::open_module);
}
