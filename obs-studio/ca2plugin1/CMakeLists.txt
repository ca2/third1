project(ca2plugin)

if(MSVC)
	set(ca2plugin_PLATFORM_DEPS
		w32-pthreads)
endif()

set(ca2plugin_SOURCES
	ca2plugin.c
	obs-slideshow.c)

add_library(ca2plugin MODULE
	${ca2plugin_SOURCES})
target_link_libraries(ca2plugin
	libobs
	${ca2plugin_PLATFORM_DEPS})

install_obs_plugin_with_data(ca2plugin data)
