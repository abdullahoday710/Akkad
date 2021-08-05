require "premake_modules/cmake"

workspace "GameAssembly"
	if _OPTIONS['target-emscripten'] then
		architecture "x86"
	else
		architecture "x86_64"
	end

	newoption {
		trigger     = "target-emscripten",
		description = "Target emscripten platform"
	 }

	configurations
	{
		"Debug",
		"Release",
	}
	flags
	{
		"MultiProcessorCompile"
    }
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    IncludeDir = {}
    IncludeDir["Glad"] = "%{wks.location}/3rdparty/glad/include"
    IncludeDir["imgui"] = "%{wks.location}/3rdparty/imgui"
    IncludeDir["glm"] = "%{wks.location}/3rdparty/glm/include"
    IncludeDir["stb"] = "%{wks.location}/3rdparty/stb/include"
    IncludeDir["spdlog"] = "%{wks.location}/3rdparty/spdlog/include"
    IncludeDir["entt"] = "%{wks.location}/3rdparty/entt/single_include"
    IncludeDir["json"] = "%{wks.location}/3rdparty/json/include"
    IncludeDir["box2d"] = "%{wks.location}/3rdparty/box2d/include"

	IncludeDir["Akkad"] = "%{wks.location}/Engine/Akkad/src"

    group "Dependencies"
      if not _OPTIONS['target-emscripten'] then
        include "3rdparty/glad"
      end
      include "3rdparty/imgui"
      include "3rdparty/spdlog"
      include "3rdparty/SPIRV-Cross"
      include "3rdparty/box2d"
    group ""

	group "Engine"
		include "Engine/Akkad"
	group ""

	project "GameAssembly"
		if  _OPTIONS['target-emscripten'] then
        	kind "ConsoleApp"
		else
			kind "SharedLib"
      	end
		language "C++"
		cppdialect "C++17"
		staticruntime "off"

		targetdir ("%{wks.location}/build/")
		objdir ("%{wks.location}/build-int/")

		files {
			"src/**.h",
			"src/**.cpp"
		}
		includedirs {
			"src/",
			"%{IncludeDir.Akkad}",
			"%{IncludeDir.spdlog}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.entt}",
			"%{IncludeDir.box2d}",
			"%{IncludeDir.stb}",
			"%{IncludeDir.json}",
			"%{IncludeDir.Glad}",
		}

		defines {"AK_GAME_ASSEMBLY"}

		links {"Akkad"}

		filter "system:windows"
		systemversion "latest"
		
		filter "configurations:Debug"
		defines "AK_DEBUG"
		runtime "Debug"
		symbols "on"
		optimize "on"
		if _OPTIONS['target-emscripten'] then
				buildoptions   {"-s SIDE_MODULE=2","-fPIC"," -s NO_DISABLE_EXCEPTION_CATCHING", "-s FORCE_FILESYSTEM=1", "-s USE_WEBGL2=1", "-s FULL_ES3=1", "-s OFFSCREEN_FRAMEBUFFER=1"}
				linkoptions    {"-s SIDE_MODULE=2","-fPIC","-s WASM=1", "-s NO_DISABLE_EXCEPTION_CATCHING", "-s ALLOW_MEMORY_GROWTH", "-s USE_WEBGL2=1", "-s FULL_ES3=1", "-s OFFSCREEN_FRAMEBUFFER=1", "-s FORCE_FILESYSTEM=1", "-s DYNCALLS=1"}
		end

		filter "configurations:Release"
			defines "AK_RELEASE"
			runtime "Release"
			optimize "on"
