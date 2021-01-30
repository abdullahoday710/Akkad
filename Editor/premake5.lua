project "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"%{IncludeDir.iconfontcpp}",
	}
		
    postbuildcommands {
	  "{ECHO} ------------ Build done, copying resources into output directory -------------------",

      "{COPY} %{wks.location}/%{prj.name}/res %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/res",

	  "{COPY} %{wks.location}/GameAssembly/src %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/GameAssembly/src",
	  "{COPY} %{wks.location}/GameAssembly/premake5.lua %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/GameAssembly",
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