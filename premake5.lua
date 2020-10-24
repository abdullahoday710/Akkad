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
    group "Dependencies"
    
    group ""
    
    include "Akkad"
    include "sandbox"