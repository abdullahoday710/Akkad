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
    group "Dependencies"
      include "3rdparty/glad"
    group ""
    
    include "Akkad"
    include "sandbox"

    print("%{wks.location}/3rdparty/glad/include")