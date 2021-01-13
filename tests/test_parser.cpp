#include <catch.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "format.h"

using namespace std;

#define TEST_FILE(file)                                                                 \
    TEST_CASE("format file " + string(file) + " works well", "format_file") {           \
        string filename(file);                                                          \
        ifstream input;                                                                 \
        input.open(filename);                                                           \
        Config config;                                                                  \
        REQUIRE_THROWS_WITH(lua_format(input, config), "Input contains syntax errors"); \
    }

TEST_FILE(PROJECT_PATH + "/tests/.data/issues/issue-55.lua");
