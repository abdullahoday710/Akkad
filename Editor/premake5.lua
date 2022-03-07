project "Editor"
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
		"%{wks.location}/3rdparty/glslang/",
		"%{wks.location}/3rdparty/SPIRV-Cross/",
		"%{wks.location}/3rdparty/ImGuizmo/",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.json}",
		"%{IncludeDir.iconfontcpp}",
		"%{IncludeDir.box2d}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.concurrentqueue}",
	}
		
    postbuildcommands {
	  "python %{wks.location}/Scripts/postbuild.py %{wks.location} %{prj.name} " .. outputdir,

    }

	links {
	"Akkad",
	"glslang",
	"ImGuizmo"
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