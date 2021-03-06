﻿cmake_minimum_required(VERSION 3.1)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project(server)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_TINY 0)
string(TIMESTAMP VERSION_YEAR "%Y")

include(CheckCXXCompilerFlag)
include(CheckLibraryExists)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/cmake")

option(ENABLE_BUILD_WERROR "All warnings being treated as errors" OFF)

if (MSVC)
	set(CompilerFlags
			CMAKE_CXX_FLAGS_DEBUG
			CMAKE_CXX_FLAGS_RELEASE
			CMAKE_CXX_FLAGS_MINSIZEREL
			CMAKE_CXX_FLAGS_RELWITHDEBINFO
			CMAKE_C_FLAGS_DEBUG
			CMAKE_C_FLAGS_RELEASE
			CMAKE_C_FLAGS_MINSIZEREL
			CMAKE_C_FLAGS_RELWITHDEBINFO
		)

	foreach(CompilerFlag ${CompilerFlags})
		string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
	endforeach()

	add_definitions(
		-D_CRT_SECURE_NO_DEPRECATE
		-D_CRT_SECURE_NO_WARNINGS
		-D_CRT_NONSTDC_NO_DEPRECATE
		-D_CRT_NONSTDC_NO_WARNINGS
		-D_SCL_SECURE_NO_DEPRECATE
		-D_SCL_SECURE_NO_WARNINGS
	)

	add_definitions(
		-DWIN32_LEAN_AND_MEAN
		-D_WIN32_WINNT=0x0601
		-DNOMINMAX
		-DUNICODE
		-D_UNICODE
		-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
	)

	add_definitions(
		-DBOOST_ALL_STATIC_LINK
		-DBOOST_THREAD_USE_LIB
		-DBOOST_FILESYSTEM_STATIC_LINK
		-DBOOST_USE_WINAPI_VERSION=0x0601
	)

	add_definitions(/MP)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
	set(CMAKE_CXX_STACK_SIZE "100000000") #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:100000000")

	if (MSVC_VERSION GREATER_EQUAL "1900")
		CHECK_CXX_COMPILER_FLAG("/std:c++17" HAS_CPP_LATEST_FLAG)
		if (HAS_CPP_LATEST_FLAG)
			add_compile_options("/std:c++17")
		endif()
	endif()
endif()

CHECK_CXX_COMPILER_FLAG(-fvisibility-inlines-hidden COMPILER_HAS_VISIBILITY_INLINE_HIDDEN)
CHECK_CXX_COMPILER_FLAG(-fvisibility=hidden COMPILER_HAS_VISIBILITY_HIDDEN)
CHECK_CXX_COMPILER_FLAG(-std=c++14 LIBCXX_HAS_STDCXX14_FLAG)
CHECK_CXX_COMPILER_FLAG(-std=c++17 LIBCXX_HAS_STDCXX17_FLAG)
CHECK_CXX_COMPILER_FLAG(-fdiagnostics-color=always COMPILER_HAS_COLOR)

if (${COMPILER_HAS_VISIBILITY_INLINE_HIDDEN})
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden")
endif()

if (${COMPILER_HAS_VISIBILITY_HIDDEN})
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
endif()

if (${COMPILER_HAS_COLOR})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
endif()

if (NOT MSVC)
	if(NOT APPLE)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
	endif()
	if (ANDROID)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIE -pie")
	endif()
	if(LIBCXX_HAS_STDCXX17_FLAG)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")
		add_definitions(-DUSE_STD_STRING_VIEW)
	elseif(LIBCXX_HAS_STDCXX14_FLAG)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3")
	else()
		message(FATAL_ERROR "need at least GCC 5 or clang 5")
	endif()
endif(NOT MSVC)

set(Boost_USE_STATIC_LIBS	ON)
set(Boost_USE_STATIC_RUNTIME	ON)

find_package(Threads)
link_libraries(${CMAKE_THREAD_LIBS_INIT})

set(BOOST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost)
set(CINATRA_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/cinatra/include)

add_definitions(-DBOOST_LOCALE_HIDE_AUTO_PTR)
add_definitions(-DBOOST_COROUTINES_NO_DEPRECATION_WARNING)
add_subdirectory(third_party/boost)
add_subdirectory(third_party/jsoncpp)

include_directories(third_party
	${BOOST_INCLUDE_DIRS}
	${CINATRA_INCLUDE_DIRS}
)

link_libraries(
#		Boost::coroutine
#		Boost::context
#		Boost::thread
		Boost::date_time
		Boost::log
#		Boost::filesystem
#		Boost::program_options
		jsoncpp
)

if (WIN32)
	link_libraries(
		Secur32.lib
	)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
endif()

add_subdirectory(server)
