-- needs premake5, currently unreleased
-- nightly binaries here:
-- http://sourceforge.net/projects/premake/files/Premake/nightlies/
solution "MyApplication"
	configurations { "Debug", "Release" }
	location "build"
	platforms { "x32", "x64" }

	-- create the "standard" VisualStudio meta-folders
	vpaths { [ "Headers" ] = { "**.h" } }
	vpaths { [ "Source" ] = { "**.cpp", "**.c" } }
	
	-- A project defines one build target
	project "MyApplication"
		kind "ConsoleApp"
		language "C++" 
		includedirs { "include" }
		local srcdir = "src/"
		files { ( srcdir .. "*.h" ), ( srcdir .. "*.cpp" ), ( srcdir .. "*.c" ) }
		configuration( "Debug" )
			defines { "DEBUG" }
			flags { "Symbols" }
			-- flags { "FatalWarnings" } 
		configuration { "Release" }
			defines { "NDEBUG" }
			-- flags { "LinkTimeOptimization" }
			optimize "Full" 
			
	configuration { "Windows" }
		flags { "MultiProcessorCompile", "StaticRuntime" }
		defines { "GLEW_STATIC", "FREEGLUT_STATIC" }
		links { "opengl32", "freeglut_static" }
		-- adjust to point to wherever you put your includes/libs
		local basedir = "C:/projects/CommonLibs/"
		includedirs { ( basedir .. "include" ) }
		configuration { "Windows", "x32" }
			libdirs { ( basedir .. "lib32" ) }
		configuration { "Windows", "x64" }
			libdirs { ( basedir .. "lib" ) }
		configuration { "Windows", "Debug" }
			linkoptions { "/nodefaultlib:libcmt.lib" }
	configuration { "MacOSX" }
		-- untested
		-- linkoptions { "-framework Carbon -framework OpenGL -framework AGL -framework Glut" }
	configuration { "not Windows", "not MacOSX" }
		links { "GL", "glut" }
	configuration {}			