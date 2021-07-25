require "premake_modules/cmake"

workspace "Akkad"
    if _OPTIONS['target-emscripten'] then
      architecture "x86"
    else
      architecture "x86_64"
    end
    startproject "Editor"

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
    outputdir = ""
    if _OPTIONS['target-emscripten'] then
      outputdir = "%{cfg.buildcfg}-Web-%{cfg.architecture}"
    else
      outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    end

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
      if not _OPTIONS['target-emscripten'] then
        include "3rdparty/glad"
      end
      include "3rdparty/imgui"
      include "3rdparty/spdlog"
      include "3rdparty/glslang"
      include "3rdparty/SPIRV-Cross"
      include "3rdparty/box2d"
      include "3rdparty/ImGuizmo"
    group ""
    
    include "Akkad"
    include "sandbox"
    include "Runtime"
    
    if not _OPTIONS['target-emscripten'] then
      include "Editor"
      include "GameAssembly"
    end