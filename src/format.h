#pragma once

#include <filesystem>
#include <iostream>
#include <string>

#include "config.h"

using namespace std;

#define PROJECT_PATH string("/home/awh6al/code/cpp/lua-format")

string lua_format(const string& input, const Config& config);
string lua_format(istream& stream, const Config& config);