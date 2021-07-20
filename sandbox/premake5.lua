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