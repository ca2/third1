project(ca2plugin)

if(WIN32)

include_directories(SYSTEM
	"${CMAKE_SOURCE_DIR}/libobs"
)

elseif(APPLE)

include_directories(SYSTEM
	"${CMAKE_SOURCE_DIR}/libobs"
)

elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")

find_package(X11 REQUIRED)
if(NOT X11_Xcomposite_FOUND)
	message(STATUS "Xcomposite library not found, linux-capture plugin disabled")
	return()
endif()

find_package(XCB COMPONENTS XCB SHM XFIXES XINERAMA REQUIRED)
find_package(X11_XCB REQUIRED)
include_directories(SYSTEM
	"${CMAKE_SOURCE_DIR}/libobs"
	${X11_Xcomposite_INCLUDE_PATH}
	${X11_X11_INCLUDE_PATH}
	${XCB_INCLUDE_DIRS}
)

endif()

if(MSVC)
	set(ca2plugin_PLATFORM_DEPS
		w32-pthreads)
endif()

if(WIN32)
	set(ca2plugin_SOURCES
		win32.c
		obs-slideshow.c)
elseif(APPLE)
	set(ca2plugin_SOURCES
		win32.c
		obs-slideshow.c)
elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
	set(ca2plugin_SOURCES
		linux.c
		xcursor.c
		xcursor-xcb.c
		xhelpers.c
		xcomposite-main.cpp
		ca2plugin-main.cpp
		ca2plugin-helper.cpp)
endif()

add_library(ca2plugin MODULE
	${ca2plugin_SOURCES})

if(MSVC)
target_link_libraries(ca2plugin
	libobs
	glad
	${X11_LIBRARIES}
	${X11_Xfixes_LIB}
	${X11_X11_LIB}
	${X11_Xcomposite_LIB}
	${XCB_LIBRARIES}
	${ca2plugin_PLATFORM_DEPS})
else()
target_link_libraries(ca2plugin
	libobs
	glad
	${X11_LIBRARIES}
	${X11_Xfixes_LIB}
	${X11_X11_LIB}
	${X11_Xcomposite_LIB}
	${XCB_LIBRARIES})
endif()

install_obs_plugin_with_data(ca2plugin data)



