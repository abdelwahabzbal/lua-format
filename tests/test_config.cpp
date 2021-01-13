#include <catch.hpp>

#include "format.h"

TEST_CASE("indent", "config") {
    Config config;
    config.set("indent_width", 2);
    REQUIRE("function W()\n  print(1)\n  print(2)\nend\n" == lua_format("function W() print(1) print(2) end", config));

    config.set("indent_width", 1);
    REQUIRE("function W()\n print(1)\n print(2)\nend\n" == lua_format("function W() print(1) print(2) end", config));
}

TEST_CASE("use_tab", "config") {
    Config config;
    config.set("indent_width", 1);
    config.set("use_tab", true);

    REQUIRE("function W()\n\tprint(1)\n\tprint(2)\nend\n" == lua_format("function W() print(1) print(2) end", config));

    config.set("use_tab", false);
    REQUIRE("function W()\n print(1)\n print(2)\nend\n" == lua_format("function W() print(1) print(2) end", config));
}

TEST_CASE("column_limit", "config") {
    Config config;
    config.set("indent_width", 1);
    config.set("continuation_indent_width", 2);
    config.set("column_limit", 10);
    REQUIRE("function W()\n q(aaaa,\n   bbbb)\n print(\n   aaaa,\n   bbbb)\nend\n" ==
            lua_format("function W() q(aaaa,bbbb) print(aaaa,bbbb) end", config));

    config.set("column_limit", 80);
    REQUIRE("function W()\n q(aaaa, bbbb)\n print(aaaa, bbbb)\nend\n" ==
            lua_format("function W() q(aaaa,bbbb) print(aaaa,bbbb) end", config));
}

TEST_CASE("spaces_before_call", "config") {
    Config config;
    config.set("indent_width", 4);
    config.set("continuation_indent_width", 4);
    config.set("column_limit", 40);
    config.set("spaces_before_call", 0);
    REQUIRE(
        "function t() return function() end end\nlocal a = t{1, 2, 3, 4}\nlocal a = t{1}\nlocal a = t{8}\nlocal a = "
        "t{1, 9}\nlocal a = t(1, 2, 3)\nt{2, 3} {1, 5}\nt{2, 3, 4} {1}\n" ==
        lua_format("function t() return function () end end local a = t{1, 2, 3, 4} local a = t        {1} local a = "
                   "t{8} local a = t  {1,9} local a = t(1, 2, 3) t {2, 3}{1,5} t{2, 3, 4}{1}",
                   config));
}

TEST_CASE("table_sep", "config") {
    Config config;
    config.set("indent_width", 2);
    config.set("table_sep", ",");

    REQUIRE("x = {1, 2, 3}\n" == lua_format("x = {1,2;3}", config));

    config.set("table_sep", ";");
    REQUIRE("x = {1; 2; 3}\n" == lua_format("x = {1,2;3}", config));
}

TEST_CASE("extra_sep_at_table_end", "config") {
    Config config;
    config.set("indent_width", 2);
    config.set("extra_sep_at_table_end", true);

    // do no add sep if table is one line
    REQUIRE("x = {1, 2, 3}\n" == lua_format("x = {1,2;3}", config));

    REQUIRE("x = {\n  1, -- line break\n  2, 3,\n}\n" == lua_format("x = {1,-- line break\n2;3}", config));
    config.set("extra_sep_at_table_end", false);
    REQUIRE("x = {\n  1, -- line break\n  2, 3\n}\n" == lua_format("x = {1,-- line break\n2;3}", config));
}

TEST_CASE("keep_simple_function_one_line", "config") {
    Config config;
    config.set("indent_width", 2);

    REQUIRE("function x() print(1) end\n" == lua_format("function x() print(1) end", config));

    config.set("keep_simple_function_one_line", false);
    REQUIRE("function x()\n  print(1)\nend\n" == lua_format("function x() print(1) end", config));
}

TEST_CASE("args", "config") {
    Config config;
    config.set("indent_width", 2);
    config.set("continuation_indent_width", 4);
    config.set("align_args", true);
    config.set("break_after_functioncall_lp", false);
    config.set("break_before_functioncall_rp", false);
    config.set("column_limit", 10);

    REQUIRE("xxx(xxx,\n    xxxxx)\n" == lua_format("xxx(xxx,xxxxx)", config));

    config.set("break_after_functioncall_lp", true);
    REQUIRE("xxx(\n    xxx,\n    xxxxx)\n" == lua_format("xxx(xxx,xxxxx)", config));
    REQUIRE("x(x, x)\n" == lua_format("x(x,x)", config));

    config.set("break_before_functioncall_rp", true);
    REQUIRE("xxx(\n    xxx,\n    xxxxx\n)\n" == lua_format("xxx(xxx,xxxxx)", config));
    REQUIRE("x(x, x)\n" == lua_format("x(x,x)", config));

    config.set("align_args", false);
    config.set("break_after_functioncall_lp", false);
    config.set("break_before_functioncall_rp", false);
    REQUIRE("x(xxxxxx,\n    xxxxx)\n" == lua_format("x(xxxxxx,xxxxx)", config));
}

TEST_CASE("functiondef", "config") {
    Config config;
    config.set("indent_width", 2);
    config.set("continuation_indent_width", 4);
    config.set("align_parameter", true);
    config.set("break_after_functiondef_lp", false);
    config.set("break_before_functiondef_rp", false);
    config.set("column_limit", 20);

    REQUIRE("function xxx(xxx,\n             xxxxx)\n  print(1)\nend\n" ==
            lua_format("function xxx(xxx,xxxxx)\n  print(1)\nend\n", config));

    config.set("break_after_functiondef_lp", true);
    REQUIRE("function xxx(\n    xxx, xxxxx)\n  print(1)\nend\n" ==
            lua_format("function xxx(xxx,xxxxx)\n  print(1)\nend\n", config));
    REQUIRE("function a(x) end\n" == lua_format("function a(x) end", config));

    config.set("break_before_functiondef_rp", true);
    REQUIRE("function xxx(\n    xxx, xxxxx\n) print(1) end\n" ==
            lua_format("function xxx(xxx,xxxxx)\n  print(1)\nend\n", config));
    REQUIRE("function a(x) end\n" == lua_format("function a(x) end", config));

    config.set("align_parameter", false);
    config.set("break_after_functiondef_lp", false);
    config.set("break_before_functiondef_rp", false);
    REQUIRE("function xxx(xxx,\n    xxxxx) print(1) end\n" ==
            lua_format("function xxx(xxx,xxxxx)\n  print(1)\nend\n", config));

    config.set("chop_down_parameter", true);
    REQUIRE("function xxx(xxx,\n    yyy,\n    zzz) print(1) end\n" ==
            lua_format("function xxx(xxx,yyy,zzz)\n  print(1)\nend\n", config));
}

TEST_CASE("table", "config") {
    Config config;
    config.set("indent_width", 2);
    config.set("continuation_indent_width", 4);
    config.set("align_table_field", true);
    config.set("break_after_table_lb", false);
    config.set("break_before_table_rb", false);
    config.set("column_limit", 15);

    REQUIRE("x = {1, 2, 3,\n     4, 5, 6, 7}\n" == lua_format("x = {1,2,3,4,5,6,7}", config));

    config.set("break_after_table_lb", true);
    REQUIRE("x = {\n  1, 2, 3, 4,\n  5, 6, 7}\n" == lua_format("x = {1,2,3,4,5,6,7}", config));
    REQUIRE("x = {1, 2}\n" == lua_format("x = {1,2}", config));

    config.set("break_before_table_rb", true);
    REQUIRE("x = {\n  1, 2, 3, 4,\n  5, 6, 7\n}\n" == lua_format("x = {1,2,3,4,5,6,7}", config));
    REQUIRE("x = {1, 2}\n" == lua_format("x = {1,2}", config));

    config.set("align_table_field", false);
    config.set("break_after_table_lb", false);
    config.set("break_before_table_rb", false);
    REQUIRE("x = {1, 2, 3,\n  4, 5, 6, 7}\n" == lua_format("x = {1,2,3,4,5,6,7}", config));

    config.set("column_limit", 250);
    config.set("indent_width", 2);
    config.set("tab_width", 2);
    config.set("continuation_indent_width", 2);
    config.set("chop_down_table", true);
    config.set("chop_down_kv_table", true);
    config.set("column_table_limit", 80);

    REQUIRE(
        "test = {\n  image = {1, 2, 3},\n  list = {\n    {\n      ref = {4, 5, 9, 8, 2},\n      tags = {1, 2, 8, 6},\n "
        "     time = 10,\n\n      materials = {{materialId = 123, count = 10}}\n    }\n  }\n}\n" ==
        lua_format("test = {\n  image = {1,2,3},\n  list = {\n    {\n      ref = {4,5,9,8,2},\n      tags = { 1, 2, 8, "
                   "6 },\n      time = 10,\n\n      materials = {\n        {\n          materialId = 123,\n          "
                   "count = 10\n        }\n      },\n    },\n  },\n}",
                   config));
}

TEST_CASE("break_after_operator", "config") {
    Config config;
    config.set("indent_width", 2);
    config.set("continuation_indent_width", 4);
    config.set("column_limit", 10);
    config.set("break_after_operator", true);

    REQUIRE("x = 111 +\n        1111111 +\n        1111111\n" == lua_format("x = 111 + 1111111 + 1111111", config));

    config.set("break_after_operator", false);
    REQUIRE("x = 111\n        + 1111111\n        + 1111111\n" == lua_format("x = 111 + 1111111 + 1111111", config));
}

TEST_CASE("read from file", "config") {
    Config config;
    config.readFromFile(PROJECT_PATH + "/tests/.config/1.lua-format");

    REQUIRE(2 == config.get<int>("indent_width"));
    REQUIRE(";" == config.get<string>("table_sep"));
    REQUIRE(false == config.get<bool>("extra_sep_at_table_end"));

    config.readFromFile(PROJECT_PATH + "/tests/.config/2.lua-format");

    REQUIRE(4 == config.get<int>("indent_width"));
    REQUIRE("," == config.get<string>("table_sep"));
    REQUIRE(false == config.get<bool>("extra_sep_at_table_end"));
}
