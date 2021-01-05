
#include "module_includes.h"

#include <regex>


namespace lua_module
{

bool fromS7Address(std::string adrStr, S7Address& adrInfo)
{
    std::locale loc;
    for (std::string::size_type i=0;i<adrStr.length();++i)
        adrStr[i] = std::toupper(adrStr[i],loc);

    // check for single bits (EAM/IQF)
    {
        std::regex re("^([EIAQMF])(\\d+)\\.(\\d+)$", std::regex_constants::ECMAScript);
        std::smatch match;
        if (std::regex_search(adrStr, match, re) && match.size() >= 4)
        {
            std::string ar = match[1]; // => capture 0 is the whole string!
            std::string by = match[2];
            std::string bi = match[3];

            if (ar == "M" || ar == "F")
                adrInfo.area = S7AreaMK;
            else if (ar == "E" || ar == "I")
                adrInfo.area = S7AreaPE;
            else if (ar == "A" || ar == "Q")
                adrInfo.area = S7AreaPA;

            adrInfo.db      = 0;
            adrInfo.start   = (std::stoi(by) * 8) + std::stoi(bi); // => start is the bit-Index here
            adrInfo.amount  = 1;
            adrInfo.wordLen = S7WLBit;

            return true;
        }
    }

    // check for byte/word/dword (EAM/IQF)
    {
        std::regex re("^([EIAQMF])([BWD])(\\d+)$", std::regex_constants::ECMAScript);
        std::smatch match;
        if (std::regex_search(adrStr, match, re) && match.size() >= 4)
        {
            std::string ar = match[1]; // => capture 0 is the whole string!
            std::string wi = match[2];
            std::string by = match[3];

            if (ar == "M" || ar == "F")
                adrInfo.area = S7AreaMK;
            else if (ar == "E" || ar == "I")
                adrInfo.area = S7AreaPE;
            else if (ar == "A" || ar == "Q")
                adrInfo.area = S7AreaPA;

            adrInfo.db      = 0;
            adrInfo.start   = std::stoi(by);
            adrInfo.amount  = 1;

            if (wi == "B")
                adrInfo.wordLen = S7WLByte;
            else if (wi == "W")
                adrInfo.wordLen = S7WLWord;
            else if (wi == "D")
                adrInfo.wordLen = S7WLDWord;


            return true;
        }
    }


//    std::regex re_db_bits("\\ADB(\\d+)[.]DBX(\\d+)[.](\\d+)$");
//    if (std::regex_match(adr, re_db_bits))
//    {

//    }

//    std::regex re_db_bytes("\\ADB(\\d+)[.]DB([BWD])(\\d+)$");
//    if (std::regex_match(adr, re_db_bytes))
//    {

//    }

    return false;
}

void register_client(sol::table& module)
{
    module.new_usertype<TS7Client>(
        "TS7Client",            sol::constructors<TS7Client()>()

        ,"setConnectionType",   &TS7Client::SetConnectionType

        ,"connectTo",           [](TS7Client& client, const char* address, int rack, int slot)
                                {
                                    int ret = client.ConnectTo(address, rack, slot);
                                    return returnWithCliError(ret);
                                }

        ,"disconnect",          [](TS7Client& client)
                                {
                                    int ret = client.Disconnect();
                                    return returnWithCliError(ret);
                                }

        ,"isConnected",         &TS7Client::Connected
        ,"plcStatus",           &TS7Client::PlcStatus


        ,"read",                [](TS7Client& client, std::string address, sol::optional<S7FormatHint> formatHint, sol::this_state L)
                                {
                                    sol::variadic_results values;


                                    S7Address adrInfo;
                                    bool valid = fromS7Address(address, adrInfo);
                                    if (!valid)
                                    {
                                        values.push_back({ L, sol::in_place, sol::lua_nil });
                                        values.push_back({ L, sol::in_place, "Invalid address string!" });
                                        return values;
                                    }

                                    S7FormatHint hint = S7FormatHint::Unsigned;
                                    if (formatHint)
                                    {
                                        hint = formatHint.value();
                                    }


                                    uint8_t buf[0xFFFF];
                                    int ret = client.ReadArea(adrInfo.area, adrInfo.db, adrInfo.start, adrInfo.amount, adrInfo.wordLen, &buf);
                                    if (ret != 0)
                                    {
                                        values.push_back({ L, sol::in_place, sol::lua_nil });
                                        values.push_back({ L, sol::in_place, CliErrorText(ret) });

                                        return values;
                                    }


                                    if (adrInfo.wordLen == S7WLBit)
                                    {
                                        values.push_back({ L, sol::in_place_type<bool>, buf[0] > 0 });
                                        values.push_back({ L, sol::in_place, "OK" });
                                    }
                                    else if (adrInfo.wordLen == S7WLByte)
                                    {
                                        if (hint == S7FormatHint::Signed)
                                            values.push_back({ L, sol::in_place_type<int8_t>, buf[0] });
                                        else
                                            values.push_back({ L, sol::in_place_type<uint8_t>, buf[0] });

                                        values.push_back({ L, sol::in_place, "OK" });
                                    }
                                    else if (adrInfo.wordLen == S7WLWord)
                                    {
                                        if (hint == S7FormatHint::Signed)
                                        {
                                            int16_t v = (*(int16_t*)&buf[0]);
                                            SwapEndian(v);
                                            values.push_back({ L, sol::in_place_type<int16_t>, v });
                                        }
                                        else
                                        {
                                            uint16_t v = (*(uint16_t*)&buf[0]);
                                            SwapEndian(v);
                                            values.push_back({ L, sol::in_place_type<uint16_t>, v });
                                        }

                                        values.push_back({ L, sol::in_place, "OK" });
                                    }
                                    else if (adrInfo.wordLen == S7WLDWord)
                                    {
                                        if (hint == S7FormatHint::Float)
                                        {
                                            float v = (*(float*)&buf[0]);
                                            SwapEndian(v);
                                            values.push_back({ L, sol::in_place_type<float>, v });
                                        }
                                        else if (hint == S7FormatHint::Signed)
                                        {
                                            int32_t v = (*(int32_t*)&buf[0]);
                                            SwapEndian(v);
                                            values.push_back({ L, sol::in_place_type<int32_t>, v });
                                        }
                                        else
                                        {
                                            uint32_t v = (*(uint32_t*)&buf[0]);
                                            SwapEndian(v);
                                            values.push_back({ L, sol::in_place_type<uint32_t>, v });
                                        }

                                        values.push_back({ L, sol::in_place, "OK" });
                                    }
                                    else
                                    {
                                        values.push_back({ L, sol::in_place, sol::lua_nil });
                                        values.push_back({ L, sol::in_place, "Invalid data!" });
                                    }

                                    return values;
                                }
    );
}

} // namespace lua_module
