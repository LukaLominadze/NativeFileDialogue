workspace "NativeFileDialogue"
	configurations {
		"Debug",
		"Release",
		"Distribution"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	architecture "x64"

	startproject "Test"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Library"
include "NativeFileDialogue"
group ""

group "Tests"
include "Test"
group ""