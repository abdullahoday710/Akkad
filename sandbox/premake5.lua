project "sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp"
	}
	includedirs {
		"%{wks.location}/Akkad/src",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.box2d}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.json}",

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
	--"-s ASYNCIFY_IMPORTS=['doLoadLibrary']"
	filter "configurations:Debug"
		if _OPTIONS['target-emscripten'] then
				executable_suffix (".html")
				buildoptions   {"-s NO_DISABLE_EXCEPTION_CATCHING","-s MAIN_MODULE=1", "-s FORCE_FILESYSTEM=1", "-s USE_WEBGL2=1", "-s FULL_ES3=1", "-s OFFSCREEN_FRAMEBUFFER=1","--profiling"}
				linkoptions    {"-O3","-s ALLOW_MEMORY_GROWTH=1","-s ALLOW_TABLE_GROWTH", "-s EXPORTED_RUNTIME_METHODS=['FS']", "-s DYNCALLS=1","-s SAFE_HEAP=1", "-s ASYNCIFY","--profiling", "-s ASYNCIFY_IMPORTS=['doLoadLibrary']", "-s NO_DISABLE_EXCEPTION_CATCHING","-s MAIN_MODULE=1", "-s USE_WEBGL2=1", "-s FULL_ES3=1", "-s OFFSCREEN_FRAMEBUFFER=1","-s FORCE_FILESYSTEM=1"}
		end