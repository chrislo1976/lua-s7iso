
#include "module_includes.h"


namespace lua_module
{

void register_client(sol::table& module)
{
    module.new_usertype<TS7Client>(
        "TS7Client",            sol::constructors<TS7Client()>()
        ,"setConnectionType",   &TS7Client::SetConnectionType
        ,"connectTo",           &TS7Client::ConnectTo
        ,"isConnected",         &TS7Client::Connected
        ,"disconnect",          &TS7Client::Disconnect
        ,"plcStatus",           &TS7Client::PlcStatus
    );
}

} // namespace lua_module
