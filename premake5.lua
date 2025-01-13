workspace "NeoVoxel"
    architecture "x86_64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Sandbox"

project "NeoVoxel"
    kind "StaticLib"
    language "C++"
    location "NeoVoxel"

    objdir "obj/%{cfg.system}-%{cfg.architecture}"
    targetdir "bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    defines {
        "NV_BUILD_MODE_DEBUG=0",
        "NV_BUILD_MODE_RELEASE=1",
        "NV_BUILD_MODE_DIST=2",
        "NV_PLATFORM_DESKTOP=0",
        "GLFW_INCLUDE_NONE", "GLFW_DLL"
    }

    files {
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/include",
        "%{prj.name}/vendor/glad/include",
        "%{prj.name}/vendor/glfw/include",
        "%{prj.name}/vendor/glm/include",
        "%{prj.name}/vendor/openal-soft/include",
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/vendor/stb/include"
    }

    libdirs {
        "%{prj.name}/vendor/glfw/lib/%{cfg.system}-%{cfg.architecture}",
        "%{prj.name}/vendor/openal-soft/lib/%{cfg.system}-%{cfg.architecture}"
    }

    pchheader "PCH.h"
    pchsource "%{prj.name}/src/PCH.cpp"

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"
        buildoptions { "/utf-8" }
        links { "opengl32.lib", "glfw3dll.lib", "OpenAL32dll.lib" }
        postbuildcommands {
            "{COPYFILE} vendor/glfw/lib/%{cfg.system}-%{cfg.architecture}/glfw3.dll ../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}",
            "{COPYFILE} vendor/openal-soft/lib/%{cfg.system}-%{cfg.architecture}/OpenAL32.dll ../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
        }
        defines {
            "NV_PLATFORM=NV_PLATFORM_DESKTOP",
            "WINVER=_WIN32_WINNT_WIN10", "_WIN32_WINNT=_WIN32_WINNT_WIN10", "NTDDI_VERSION=NTDDI_WIN10_RS4",
            "WIN32_LEAN_AND_MEAN", "NOMINMAX", "_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR"
        }

    filter "system:linux"
        defines { "NV_PLATFORM=NV_PLATFORM_DESKTOP" }

    filter "system:macosx"
        defines { "NV_PLATFORM=NV_PLATFORM_DESKTOP" }

    filter "configurations:Debug"
        defines { "NV_BUILD_MODE=NV_BUILD_MODE_DEBUG", "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG", "NV_BUILD_MODE=NV_BUILD_MODE_RELEASE", "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG" }
        runtime "Release"
        optimize "Debug"

    filter "configurations:Dist"
        defines { "NDEBUG", "NV_BUILD_MODE=NV_BUILD_MODE_DIST", "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_ERROR" }
        runtime "Release"
        optimize "Full"

project "Sandbox"
    language "C++"
    location "Sandbox"

    objdir "obj/%{cfg.system}-%{cfg.architecture}"
    targetdir "bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    defines {
        "NV_BUILD_MODE_DEBUG=0",
        "NV_BUILD_MODE_RELEASE=1",
        "NV_BUILD_MODE_DIST=2",
        "NV_PLATFORM_DESKTOP=0",
        "SPDLOG_NO_SOURCE_LOC"
    }

    files {
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/include",
        "NeoVoxel/include",
        "NeoVoxel/vendor/glm/include",
        "NeoVoxel/vendor/spdlog/include"
    }

    links { "NeoVoxel" }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"
        buildoptions { "/utf-8" }
        defines {
            "NV_PLATFORM=NV_PLATFORM_DESKTOP",
            "WINVER=_WIN32_WINNT_WIN10", "_WIN32_WINNT=_WIN32_WINNT_WIN10", "NTDDI_VERSION=NTDDI_WIN10_RS4",
            "WIN32_LEAN_AND_MEAN", "NOMINMAX"
        }

    filter "system:linux"
        defines { "NV_PLATFORM=NV_PLATFORM_DESKTOP" }

    filter "system:macosx"
        defines { "NV_PLATFORM=NV_PLATFORM_DESKTOP" }

    filter "configurations:Debug"
        kind "ConsoleApp"
        defines { "NV_BUILD_MODE=NV_BUILD_MODE_DEBUG", "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        kind "ConsoleApp"
        defines { "NDEBUG", "NV_BUILD_MODE=NV_BUILD_MODE_RELEASE", "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG" }
        runtime "Release"
        optimize "Debug"

    filter "configurations:Dist"
        kind "WindowedApp"
        defines { "NDEBUG", "NV_BUILD_MODE=NV_BUILD_MODE_DIST", "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO" }
        runtime "Release"
        optimize "Full"