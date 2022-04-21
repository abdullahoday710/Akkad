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
        "%{IncludeDir.curl}",
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
                "curl-lib",
                "Ws2_32.lib",
                "Wldap32.lib"

            }
        end
        files
        {
            "src/Akkad/Platforms/Desktop/Windows/**.h",
            "src/Akkad/Platforms/Desktop/Windows/**.cpp",
            "src/Akkad/Graphics/API/OpenGL/**.h",
            "src/Akkad/Graphics/API/OpenGL/**.cpp",
        }


    
    configuration "target-emscripten"
    excludes
    {
        "src/Akkad/Platforms/**.h",
        "src/Akkad/Platforms/**.cpp",
        "src/Akkad/Graphics/API/**",
        "src/Akkad/Net/HTTP/CurlHTTPHandler.cpp",
    }
    files
    {
        "src/Akkad/Platforms/Web/**.h",
        "src/Akkad/Platforms/Web/**.cpp",
        "src/Akkad/Graphics/API/OpenGLES/**"
    }
    
    buildoptions{"-fPIC -g -pthread -O3"};
    linkoptions{"-fPIC -g -pthread -O3"};



filter "configurations:Debug"
    defines {"AK_DEBUG", "CURL_STATICLIB"}
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    defines {"AK_RELEASE", "CURL_STATICLIB"}
    runtime "Release"
    optimize "on"