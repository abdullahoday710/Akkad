workspace "GameAssembly"
	architecture "x86_64"
	configurations
	{
		"Debug",
		"Release",
	}

	IncludeDir = {}
	IncludeDir["glm"] = "%{wks.location}/include/glm/include"
	IncludeDir["entt"] = "%{wks.location}/include/entt/single_include"
	IncludeDir["spdlog"] = "%{wks.location}/include/spdlog/include"
	IncludeDir["box2d"] = "%{wks.location}/include/box2d/include"
	IncludeDir["stb"] = "%{wks.location}/include/stb/include"

	IncludeDir["Akkad"] = "%{wks.location}/include/Akkad/"

	project "GameAssembly"
		kind "SharedLib"
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
		}

		defines {"AK_GAME_ASSEMBLY"}

		filter "system:windows"
			systemversion "latest"

		filter "configurations:Debug"
			defines "AK_DEBUG"
			runtime "Debug"
			symbols "off"
			optimize "on"

		filter "configurations:Release"
			defines "AK_RELEASE"
			runtime "Release"
			optimize "on"