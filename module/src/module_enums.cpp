
#include "module_includes.h"


namespace lua_module
{

void register_enums(sol::table& module)
{
    module.new_enum(
        "ConnectionType"
        ,"PG",    CONNTYPE_PG
        ,"OP",    CONNTYPE_OP
        ,"BASIC", CONNTYPE_BASIC
    );

    module.new_enum(
        "S7IsoErrors"
        ,"CONNECT",            errIsoConnect
        ,"DISCONNECT",         errIsoDisconnect
        ,"INVALID_PDU",        errIsoInvalidPDU
        ,"INVALID_DATASIZE",   errIsoInvalidDataSize
        ,"NULL_POINTER",       errIsoNullPointer
        ,"SHORT_PACKET",       errIsoShortPacket
        ,"TOO_MANY_FRAGMENTS", errIsoTooManyFragments
        ,"PDU_OVERFLOW",       errIsoPduOverflow
        ,"SEND_PACKET",        errIsoSendPacket
        ,"RECV_PACKET",        errIsoRecvPacket
        ,"INVALID_PARAMS",     errIsoInvalidParams
    );

    module.new_enum(
        "S7CpuStatus"
        ,"UNKNOWN", S7CpuStatusUnknown
        ,"RUN",     S7CpuStatusRun
        ,"STOP",    S7CpuStatusStop
    );
}

} // namespace lua_module
