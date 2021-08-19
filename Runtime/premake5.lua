project "Runtime"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	debugdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp"
	}
	includedirs {
		"src/",
		"%{wks.location}/Akkad/src",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.json}",
		"%{IncludeDir.box2d}",
		"%{IncludeDir.stb}",
	}

	links {
	"Akkad"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "AK_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "AK_RELEASE"
		runtime "Release"
		optimize "on"
	
	configuration "target-emscripten"
		executable_suffix (".html")
		filter "configurations:Debug"
		executable_suffix (".html")
		buildoptions   {"-s MAIN_MODULE=1", "-s FORCE_FILESYSTEM=1", "-s USE_WEBGL2=1", "-s FULL_ES3=1", "-s OFFSCREEN_FRAMEBUFFER=1"}
		linkoptions    {"-O3","-s ASYNCIFY","-s ASYNCIFY_IMPORTS=['doLoadLibrary']","-s ALLOW_MEMORY_GROWTH=1","-s ALLOW_TABLE_GROWTH", "-s EXPORTED_RUNTIME_METHODS=['FS']", "-s DYNCALLS=1","-s MAIN_MODULE=1", "-s USE_WEBGL2=1", "-s FULL_ES3=1", "-s OFFSCREEN_FRAMEBUFFER=1","-s FORCE_FILESYSTEM=1", "--profiling"}