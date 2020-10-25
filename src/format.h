#pragma once

#include <iostream>
#include <string>

#include "config.h"

using namespace std;

std::string get_current_dir();
#define PROJECT_PATH get_current_dir()

string lua_format(const string& input, const Config& config);
string lua_format(istream& stream, const Config& config);