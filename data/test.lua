local function test_lua_engine()
    print("Testing Lua Engine...")

    -- Basic arithmetic
    local add = 5 + 3
    print("Addition: 5 + 3 = " .. add)

    local subtract = 10 - 4
    print("Subtraction: 10 - 4 = " .. subtract)

    local multiply = 6 * 7
    print("Multiplication: 6 * 7 = " .. multiply)

    local divide = 20 / 4
    print("Division: 20 / 4 = " .. divide)

    local modulo = 17 % 5
    print("Modulo: 17 % 5 = " .. modulo)

    local power = 2 ^ 8
    print("Power: 2 ^ 8 = " .. power)

    -- String operations
    local concat = "Hello" .. " " .. "Lua"
    print("String concat: " .. concat)

    -- Table operations
    local t = {1, 2, 3}
    print("Table length: " .. #t)

    print("[OK] Lua Engine is working!")
end

-- Run the test
test_lua_engine()

-- Test table operations
print("\n--- Table Tests ---")
local person = {name = "Alice", age = 30, city = "New York"}
print("Table: name=" .. person.name .. ", age=" .. person.age)

local mixed_table = {1, "two", 3.14, true, {nested = "table"}}
for i, v in ipairs(mixed_table) do
    print("Index " .. i .. ": " .. tostring(v))
end

-- Test string methods
print("\n--- String Tests ---")
local str = "Lua Programming"
print("String length: " .. string.len(str))
print("Uppercase: " .. string.upper(str))
print("Substring: " .. string.sub(str, 1, 3))
print("Find 'Pro': " .. tostring(string.find(str, "Pro")))
