cmake_minimum_required(VERSION 3.4.1)

message("SOUND_DRIVER = ${SOUND_DRIVER}")

if(NOT SOUND_DRIVER)
	set(SOUND_DRIVER ALSA CACHE INTERNAL "Build Default SOUND_DRIVER Seting ALSA" FORCE)
endif()

add_compile_options(-g -O2 -Wall -Iinclude -std=c++11)
include_directories("include")

set(mp3emulate_SOURCES)
list(APPEND mp3emulate_SOURCES 
	main.cpp
	AudioPlayer.cpp
)

if(SOUND_DRIVER STREQUAL ALSA)
	message("SOUND_DRIVER build ALSA")
	list(APPEND mp3emulate_SOURCES 
		ALSADriver.cpp
	)
endif()
add_executable(mp3emulate ${mp3emulate_SOURCES})
target_link_libraries(mp3emulate asound avcodec avformat swresample avutil)
