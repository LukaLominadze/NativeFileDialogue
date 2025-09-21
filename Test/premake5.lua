project "Test"
	kind "ConsoleApp"
	staticruntime "off"

	language "C++"
	cppdialect "C++20"

	targetdir ("%{wks.location}/bin/" ..outputdir.. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" ..outputdir.. "/%{prj.name}")
	
	includedirs {
		"%{wks.location}/NativeFileDialogue/src"
	}

	links {
		"NativeFileDialogue"
	}

	files {
		"src/**.h",
		"src/**.cpp"
	}

	vpaths {
		["Source Files"] = "src/**.cpp",
		["Header Files"] = "src/**.h"
	}

	configurations {
		"Debug",
		"Release",
		"Distribution"
	}

	filter "toolset:msc*"
  		buildoptions { "/utf-8" }

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"
		
		defines { "PLATFORM_WINDOWS" }
	
	filter "system:linux"
		cppdialect "C++20"
		systemversion "latest"
		
		defines { "PLATFORM_LINUX" }

	filter "configurations:Debug"
		
		defines { "_DEBUG", "_CONSOLE" }
		
		architecture "x64"
		symbols "on"

	filter "configurations:Release"
		
		defines { "NDEBUG", "_CONSOLE" }

		architecture "x64"
		optimize "on"

	filter "configurations:Distribution"

		defines { "DISTRIBUTION" }
	
		architecture "x64"
		optimize "on"
