workspace "Akkad"
    architecture "x86_64"
    startproject "Editor"
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
    IncludeDir["iconfontcpp"] = "%{wks.location}/3rdparty/iconfontcpp/include"
    IncludeDir["box2d"] = "%{wks.location}/3rdparty/box2d/include"

    group "Dependencies"
      include "3rdparty/glad"
      include "3rdparty/imgui"
      include "3rdparty/spdlog"
      include "3rdparty/glslang"
      include "3rdparty/SPIRV-Cross"
      include "3rdparty/box2d"
      include "3rdparty/ImGuizmo"
    group ""
    
    include "Akkad"
    include "Editor"
    include "GameAssembly"
    include "sandbox"
    include "Runtime"