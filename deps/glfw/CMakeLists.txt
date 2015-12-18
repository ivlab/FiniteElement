#------------------------------------------
# Download dependencies
#------------------------------------------

set(glfw_checkout_Dir ${CMAKE_SOURCE_DIR}/deps/glfw)
make_directory(${glfw_checkout_Dir})
include(ExternalProject)

set(glfw_PREFIX "${glfw_checkout_Dir}")
set(glfw_INSTALL_DIR "${glfw_checkout_Dir}/install")

set(glfw_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${glfw_INSTALL_DIR} -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON -DGLFW_BUILD_DOCS=OFF)
set(glfw_DIR "${glfw_INSTALL_DIR}")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(glfw_CMAKE_ARGS ${glfw_CMAKE_ARGS} -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD="c++11" -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY="libc++")
endif()

#   if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
#		set(GIT_EXECUTABLE "C:/Program Files (x86)/Git/bin/git.exe")
#   endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
   
ExternalProject_add(glfw
		PREFIX ${glfw_PREFIX}
		GIT_REPOSITORY https://github.com/ivlab/glfw.git
		GIT_TAG gpu-affinity
		INSTALL_DIR ${glfw_INSTALL_DIR}
		CMAKE_ARGS ${glfw_CMAKE_ARGS}
		UPDATE_COMMAND ""
)
 
set_property(TARGET glfw PROPERTY FOLDER "Dependencies")