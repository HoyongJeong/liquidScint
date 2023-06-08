################################################################################
#   CMakeLists.txt for liquidScint
#
#     - Hoyong Jeong (hoyong5419@korea.ac.kr), Eunil Won (eunilwon@korea.ac.kr)
################################################################################

#------------------------------------------------------------------------------#
#   Setup the project
#------------------------------------------------------------------------------#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(LIQUIDSCINT)

#------------------------------------------------------------------------------#
#   Find Geant4 package, activating all available UI and Vis drivers by
# default. You can set WITH_GEANT4_UIVIS to OFF via the command line or
# ccmake/cmake-gui to build a batch mode only executable.
#------------------------------------------------------------------------------#
option(WITH_GEANT4_UIVIS "Build liquidScint with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
	find_package(Geant4 REQUIRED ui_all vis_all)
else()
	find_package(Geant4 REQUIRED)
endif()

#------------------------------------------------------------------------------#
#   Setup Geant4 include directories and compile definitions
#   Setup include directory for this project
#------------------------------------------------------------------------------#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#------------------------------------------------------------------------------#
#   Locate sources and headers for this project
#   NB: headers are included so they will show up in IDEs
#------------------------------------------------------------------------------#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#------------------------------------------------------------------------------#
#   Add the executable, and link it to the Geant4 libraries
#------------------------------------------------------------------------------#
add_executable(liquidScint liquidScint.cc ${sources} ${headers})
target_link_libraries(liquidScint ${Geant4_LIBRARIES})

#------------------------------------------------------------------------------#
#   Copy all scripts to the build directory, i.e. the directory in which we
# build liquidScint. This is so that we can run the executable directly because
# it relies on these scripts being in the current working directory.
#------------------------------------------------------------------------------#
set(LIQUIDSCINT_SCRIPTS
	README
	init_vis.mac
	vis.mac
)

foreach(_script ${LIQUIDSCINT_SCRIPTS})
	configure_file(
		${PROJECT_SOURCE_DIR}/${_script}
		${PROJECT_BINARY_DIR}/${_script}
		COPYONLY
	)
endforeach()

#------------------------------------------------------------------------------#
#   For internal Geant4 use - but has no effect if you build this liquidScint
# standalone
#------------------------------------------------------------------------------#
add_custom_target(LIQUIDSCINT DEPENDS liquidScint)

#------------------------------------------------------------------------------#
#   Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#------------------------------------------------------------------------------#
install(TARGETS liquidScint DESTINATION bin)

#------------------------------------------------------------------------------#
#   Flag setting
#------------------------------------------------------------------------------#
set (CMAKE_EXE_LINKER_FLAGS "-Wl,--no-as-needed")
