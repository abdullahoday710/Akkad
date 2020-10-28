project "Akkad"
    kind "staticlib"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
	{
		"src/**.h",
		"src/**.cpp",
    }
    
    includedirs
	{
        "src",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.imgui}"
    }
    
    links
	{
        "opengl32.lib",
        "Glad",
        "imgui"
    }
    
    filter "system:windows"
    systemversion "latest"

    defines
    {
    }

filter "configurations:Debug"
    defines "AK_DEBUG"
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    defines "AK_RELEASE"
    runtime "Release"
    optimize "on"