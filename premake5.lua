workspace "Kaleidoscope"
	architecture "x32"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "GLFW"
	GLFWLocation = "Kaleidoscope/vendor/GLFW/"
	kind "StaticLib"

	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		GLFWLocation .. "include/GLFW/glfw3.h",
		GLFWLocation .. "include/GLFW/glfw3native.h",
		GLFWLocation .. "src/glfw_config.h",
		GLFWLocation .. "src/context.c",
		GLFWLocation .. "src/init.c",
		GLFWLocation .. "src/input.c",
		GLFWLocation .. "src/monitor.c",
		GLFWLocation .. "src/vulkan.c",
		GLFWLocation .. "src/window.c"
		}

	filter "system:windows"
		staticruntime "on"
		systemversion "latest"

		files {
			GLFWLocation .. "src/win32_init.c",
			GLFWLocation .. "src/win32_joystick.c",
			GLFWLocation .. "src/win32_monitor.c",
			GLFWLocation .. "src/win32_time.c",
			GLFWLocation .. "src/win32_thread.c",
			GLFWLocation .. "src/win32_window.c",
			GLFWLocation .. "src/wgl_context.c",
			GLFWLocation .. "src/egl_context.c",
			GLFWLocation .. "src/osmesa_context.c",
		}

		defines {
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter { "system:windows", "configurations: Release" }
		buildoptions "/MT"

project "Glad"
	GladLocation = "Kaleidoscope/vendor/Glad/"
	kind "StaticLib"

	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		GladLocation .. "include/glad/glad.h",
		GladLocation .. "include/KHR/khrplatform.h",
		GladLocation .. "src/glad.c",
		}

	includedirs {
		GladLocation .. "include"
	}

	filter "system:windows"
		staticruntime "on"
		systemversion "latest"

	filter { "system:windows", "configurations: Release" }
		buildoptions "/MT"

project "Kaleidoscope"
	location "Kaleidoscope"
	kind "ConsoleApp"

	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
		}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/Glad/include"
	}

	libdirs {
	

	}

	links {
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "off"
		systemversion "latest"

		defines {
			"KS_PLATFORM_WINDOWS",
		}
	
	filter "configurations:Debug"
		defines "KS_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "KS_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "KS_DIST"
		optimize "On"