add_repositories("antlr https://github.com/antlr/antlr4.git")
add_repositories("args https://github.com/Taywee/args.git")
add_repositories("yaml https://github.com/jbeder/yaml-cpp.git")
add_repositories("catch https://github.com/catchorg/Catch2.git")

local repo = "$(configdir)/repositories/"

target("lua-format")
set_kind("binary")
add_cxxflags("-std=c++17", "-fuse-ld=lld", {force = true})
add_files("src/**.cpp", repo .. "antlr/runtime/Cpp/runtime/src/**.cpp",
          repo .. "yaml/src/**.cpp")
add_includedirs("src", "src/lua", repo .. "args", repo .. "yaml/include",
                repo .. "antlr/runtime/Cpp/runtime/src")

target("test-lua-format")
set_kind("binary")
add_cxxflags("-std=c++17", "-fuse-ld=lld", {force = true})
add_files("tests/**.cpp", "src/**.cpp", repo .. "yaml/src/**.cpp",
          repo .. "antlr/runtime/Cpp/runtime/src/**.cpp")
del_files("src/main.cpp")
add_includedirs("src", "src/lua", repo .. "yaml/include",
                repo .. "antlr/runtime/Cpp/runtime/src", repo .. "args",
                repo .. "catch/include")
