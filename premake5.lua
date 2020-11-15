workspace "Akkad"
    architecture "x86_64"
    startproject "sandbox"
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

    group "Dependencies"
      include "3rdparty/glad"
      include "3rdparty/imgui"
      include "3rdparty/spdlog"
    group ""
    
    include "Akkad"
    include "Editor"
    include "sandbox"

    print("%{wks.location}/3rdparty/glad/include")