# lua-s7iso

[![License](http://img.shields.io/badge/License-MIT-brightgreen.svg)](LICENSE)

A lua module for communication to Siemens S7-PLCs

## Basic usage
```lua
local s7 = require('lua-s7iso')

local ip, rack, slot = '192.168.0.75', 0, 1

-- create TS7Client
local plc = s7.TS7Client.new()

-- connect to plc, print error in case of error
local ret,err = plc:connectTo(ip, rack, slot)
if not plc:isConnected() then
    print(ret, err)
    return
end

-- read a byte from MB1000 (unsigned per default)
value,err = plc:read('MB1000')
if value == nil then
    print(err)
end
print(value)

-- read a byte from MB1000, signed
value,err = plc:read('MB1000', s7.FormatHint.SIGNED)
if value == nil then
    print(err)
end
print(value)

-- read a unsigned int (32bit) from MD1000
value,err = plc:read('MD1000', s7.FormatHint.UNSIGNED)
if value == nil then
    print(err)
end
print(value)

-- read a float (32bit) from MD1000
value,err = plc:read('MD1000', s7.FormatHint.FLOAT)
if value == nil then
    print(err)
end
print(value)

-- write -120 to byte at MB1000
-- FormatHint isn't necessary here, because this info is taken
-- directly from the lua data type
ret,err = plc:write('MB1000', -120)
if not ret then
    print(ret,err)
end

-- disconnect from plc
plc:disconnect()


```

## Remarks

- lua-s7iso was compiled and tested with macOS 10.14, Windows 10 x64 and Ubuntu 20.04 x64.
- lua-s7iso is also available via LuaRocks.
- This module was inspired by [lua-snap7](https://github.com/srdgame/lua-snap7), but was written from scratch.
- lua-s7iso uses Snap7 (see below) for communication to PLCs, but does not use/provide each method from it. Rather, lua-s7iso aims to provide a easy-to-use interface.

## External dependencies (included)
- [Sol3 (sol2 v3.0) - a C++ <-> Lua API wrapper with advanced features and top notch performance]( https://github.com/ThePhD/sol2 )
- [Snap7 - Step7 Open Source Ethernet Communication Suite](http://snap7.sourceforge.net)
__