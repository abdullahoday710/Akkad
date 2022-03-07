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
		"%{IncludeDir.concurrentqueue}",
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
	
	if _OPTIONS['target-emscripten'] then
		webmodules = path.getabsolute("WebModules/include_assets.js")
		GameAssembly = path.getabsolute("gameassembly/libGameAssembly.a")
		Common = path.getabsolute("gameassembly/libCommon.a")
		LZ4 = path.getabsolute("gameassembly/liblz4.a")
		executable_suffix (".html")
		filter "configurations:Debug"
		executable_suffix (".html")
		buildoptions   {"-O3", "-s NO_DISABLE_EXCEPTION_CATCHING", "-fPIC", "-pthread", "-s FORCE_FILESYSTEM=1", "-s USE_WEBGL2=1", "-s FULL_ES3=1", "-s OFFSCREEN_FRAMEBUFFER=1"}
		linkoptions    {"-O3", "-s EXCEPTION_DEBUG", "-s NO_DISABLE_EXCEPTION_CATCHING", "-s LLD_REPORT_UNDEFINED", "-fPIC", "-s TOTAL_MEMORY=2500MB", "-pthread", "-s PTHREAD_POOL_SIZE=3", "-s USE_PTHREADS=1", "-s ALLOW_TABLE_GROWTH", "-s ASSERTIONS=1", "-s EXPORTED_RUNTIME_METHODS=['FS']", "-s DYNCALLS=1", "-s USE_WEBGL2=1", "-s FULL_ES3=1", "-s OFFSCREEN_FRAMEBUFFER=1","-s FORCE_FILESYSTEM=1", "--profiling","-s EXPORT_ALL=1", "--pre-js=" .. webmodules .. " " .. GameAssembly .. " " .. Common .. " " .. LZ4}
	end