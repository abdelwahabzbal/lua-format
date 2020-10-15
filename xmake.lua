add_repositories("antlr https://github.com/antlr/antlr4.git")
add_repositories("args https://github.com/Taywee/args.git")
add_repositories("yaml https://github.com/jbeder/yaml-cpp.git")
add_repositories("catch https://github.com/catchorg/Catch2.git")

local repo = "$(configdir)/repositories/"
local buildir = "$(buildir)/" -- FIX: find better place for parser 

before_build(function()
    if not os.exists(buildir .. "parser") then
        os.execv("antlr4", {
            "-Dlanguage=Cpp", "-no-listener", "-visitor", "-o",
            buildir .. "parser", "Lua.g4"
        })
    end
end)

target("lua-format")
set_kind("binary")
add_cxxflags("-std=c++17", "-fuse-ld=lld", {force = true})
add_files("src/*.cpp", repo .. "antlr/runtime/Cpp/runtime/src/**.cpp",
          repo .. "yaml/src/**.cpp", "build/parser/*.cpp")
add_includedirs("src", "build/parser", repo .. "args", repo .. "yaml/include",
                repo .. "antlr/runtime/Cpp/runtime/src")

target("test")
set_kind("binary")
add_cxxflags("-std=c++17", "-fuse-ld=lld", "-fpermissive", {force = true})
add_files("test/**.cpp", "src/**.cpp", repo .. "yaml/src/**.cpp",
          repo .. "antlr/runtime/Cpp/runtime/src/**.cpp",
          "$(buildir)/parser/*.cpp")
del_files("src/main.cpp")
add_includedirs("$(buildir)/parser", repo .. "yaml/include",
                repo .. "antlr/runtime/Cpp/runtime/src", "src", repo .. "args",
                repo .. "catch/include")
