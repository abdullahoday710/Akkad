project "Akkad"
    kind "staticlib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
	{
		"src/**.h",
		"src/**.cpp",
    }

    excludes
    {
     "src/Akkad/Platforms/**.h",
     "src/Akkad/Platforms/**.cpp",
     "src/Akkad/Graphics/API/**",
    }
    
    includedirs
	{
        "src",
        "%{wks.location}/3rdparty/SPIRV-Cross/",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.json}",
        "%{IncludeDir.box2d}",
    }
    
    links
	{
        "imgui",
        "spdlog",
        "SPIRV-Cross",
        "box2d",
    }
    
    filter "system:windows"
        systemversion "latest"
        if not _OPTIONS['target-emscripten'] then
            links
            {
                "opengl32.lib",
                "Glad",
            }
        end
        files
        {
            "src/Akkad/Platforms/Desktop/Windows/**.h",
            "src/Akkad/Platforms/Desktop/Windows/**.cpp",
            "src/Akkad/Graphics/API/OpenGL/**.h",
            "src/Akkad/Graphics/API/OpenGL/**.cpp",
        }

        defines
        {
        }
    
    configuration "target-emscripten"
        excludes
        {
        "src/Akkad/Platforms/**.h",
        "src/Akkad/Platforms/**.cpp",
        "src/Akkad/Graphics/API/**",
        }
        files {
            "src/Akkad/Platforms/Web/**.h",
            "src/Akkad/Platforms/Web/**.cpp",
            "src/Akkad/Graphics/API/OpenGLES/**"
        }



filter "configurations:Debug"
    defines "AK_DEBUG"
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    defines "AK_RELEASE"
    runtime "Release"
    optimize "on"