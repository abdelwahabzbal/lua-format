#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <catch.hpp>
#include "format.h"

// We haven't checked which filesystem to include yet
#ifndef INCLUDE_STD_FILESYSTEM_EXPERIMENTAL

// Check for feature test macro for <filesystem>
#if defined(__cpp_lib_filesystem)
#define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 0

// Check for feature test macro for <experimental/filesystem>
#elif defined(__cpp_lib_experimental_filesystem)
#define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1

// We can't check if headers exist...
// Let's assume experimental to be safe
#elif !defined(__has_include)
#define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1

// Check if the header "<filesystem>" exists
#elif __has_include(<filesystem>)

// If we're compiling on Visual Studio and are not compiling with C++17, we need to use experimental
#ifdef _MSC_VER

// Check and include header that defines "_HAS_CXX17"
#if __has_include(<yvals_core.h>)
#include <yvals_core.h>

// Check for enabled C++17 support
#if defined(_HAS_CXX17) && _HAS_CXX17
// We're using C++17, so let's use the normal version
#define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 0
#endif
#endif

// If the marco isn't defined yet, that means any of the other VS specific checks failed, so we need to use experimental
#ifndef INCLUDE_STD_FILESYSTEM_EXPERIMENTAL
#define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1
#endif

// Not on Visual Studio. Let's use the normal version
#else // #ifdef _MSC_VER
#define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 0
#endif

// Check if the header "<filesystem>" exists
#elif __has_include(<experimental/filesystem>)
#define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1

// Fail if neither header is available with a nice error message
#else
#error Could not find system header "<filesystem>" or "<experimental/filesystem>"
#endif

// We priously determined that we need the exprimental version
#if INCLUDE_STD_FILESYSTEM_EXPERIMENTAL
// Include it
#include <experimental/filesystem>

// We need the alias from std::experimental::filesystem to std::filesystem
namespace std {
    namespace filesystem = experimental::filesystem;
}

// We have a decent compiler and can use the normal version
#else
// Include it
#include <filesystem>
#endif

#endif // #ifndef INCLUDE_STD_FILESYSTEM_EXPERIMENTAL

#define TEST_FILE(file)                                                                                     \
    TEST_CASE("format file " + string(file) + " works well", "format_file") {                               \
        string filename(file);                                                                              \
        ifstream input;                                                                                     \
        input.open(filename);                                                                               \
        Config config;                                                                                      \
        int idx = filename.find_last_of('/');                                                               \
        string expectFileName = filename.substr(0, idx) + "/_" + filename.substr(idx + 1, filename.size()); \
        idx = filename.find_last_of('.');                                                                   \
        string configFileName = filename.substr(0, idx) + ".config";                                        \
        if (std::filesystem::exists(configFileName)) {                                                                   \
            std::cout << configFileName << " exist" << endl;                                                \
            config.readFromFile(configFileName);                                                            \
            std::cout << config.get<bool>("chop_down_parameter") << " dd" << endl;                          \
        }                                                                                                   \
        string actual = lua_format(input, config);                                                          \
        ifstream expectFile(expectFileName);                                                                \
        stringstream ss;                                                                                    \
        ss << expectFile.rdbuf();                                                                           \
        string expect = ss.str();                                                                           \
        REQUIRE(expect == actual);                                                                          \
        string formatTwice = lua_format(actual, config);                                                    \
        REQUIRE(expect == formatTwice);                                                                     \
    }

TEST_FILE(PROJECT_PATH + "/test/testdata/linebreak/args_length.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/linebreak/block.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/linebreak/chained_call_args.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/linebreak/disable_align_in_function.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/linebreak/functioncall.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/linebreak/functiondef.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/linebreak/indent_in_explist.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/linebreak/long_var.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/linebreak/nested_method_call.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/linebreak/operators.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/linebreak/table.lua");

TEST_FILE(PROJECT_PATH + "/test/testdata/comment/function.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/comment/space.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/comment/table.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/comment/varlist.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/comment/attrib.lua");

TEST_FILE(PROJECT_PATH + "/test/testdata/statement/function_call.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/statement/function.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/statement/operator.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/statement/semi.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/statement/shebang.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/statement/statements.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/statement/table.lua");

TEST_FILE(PROJECT_PATH + "/test/testdata/literals/doublequote.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/literals/singlequote.lua");

TEST_FILE(PROJECT_PATH + "/test/testdata/syntax/lua54.lua");

TEST_FILE(PROJECT_PATH + "/test/testdata/expression/function_1.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/expression/function_2.lua");

TEST_FILE(PROJECT_PATH + "/test/testdata/disable_format/simple.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/disable_format/with_indent.lua");

TEST_FILE(PROJECT_PATH + "/test/testdata/issues/issue-1.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/issues/issue-18.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/issues/issue-19.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/issues/issue-36.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/issues/issue-62_1.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/issues/issue-62_2.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/issues/issue-62_3.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/issues/issue-70.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/issues/issue-98.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/issues/issue-98_1.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/issues/issue-80.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/issues/PR-100.lua");

TEST_FILE(PROJECT_PATH + "/test/testdata/issues/PR-108.lua");

TEST_FILE(PROJECT_PATH + "/test/testdata/keep_simple_block_one_line/default.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/keep_simple_block_one_line/keep_simple_function_one_line_false.lua");
TEST_FILE(PROJECT_PATH + "/test/testdata/keep_simple_block_one_line/keep_simple_control_block_one_line_false.lua");
