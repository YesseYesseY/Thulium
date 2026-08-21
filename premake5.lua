workspace "Thulium"
    configurations { "Release" }

project "ThuliumClient"
    kind "SharedLib"
    language "C++"
    cppdialect "C++23"
    targetdir "bin"
    objdir "obj/client"
    location "Src"
    files {
        "Src/**.cpp",
        "minhook/src/**.c",
    }
    includedirs {
        "minhook/include",
        "minhook/src/hde"
    }
    buildoptions { "/wd4369", "/wd4309" }
    architecture "x64"
    defines { "CLIENT=1", "SERVER=0" }

project "ThuliumServer"
    kind "SharedLib"
    language "C++"
    cppdialect "C++23"
    targetdir "bin"
    objdir "obj/server"
    location "Src"
    files {
        "Src/**.cpp",
        "minhook/src/**.c",
    }
    includedirs {
        "minhook/include",
        "minhook/src/hde"
    }
    buildoptions { "/wd4369", "/wd4309" }
    architecture "x64"
    defines { "SERVER=1", "CLIENT=0" }
