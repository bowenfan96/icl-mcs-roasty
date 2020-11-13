cmake_minimum_required(VERSION 3.10)
project(Roasty)

############################## Custom build modes ###############################

set(CMAKE_CXX_FLAGS_SANITIZE "-fno-omit-frame-pointer -fsanitize=address,signed-integer-overflow,null,alignment,bounds,function,return,vla-bound -O0 -g"
  CACHE STRING
  "Flags used by the C++ compiler during Sanitize builds."
  FORCE)
set(CMAKE_C_FLAGS_SANITIZE "-fno-omit-frame-pointer -fsanitize=address,signed-integer-overflow,null,alignment,bounds,function,return,vla-bound -O0 -g"
  CACHE STRING
  "Flags used by the C compiler during Sanitize builds."
  FORCE)
set(CMAKE_EXE_LINKER_FLAGS_SANITIZE
  ${CMAKE_EXE_LINKER_FLAGS_DEBUG} CACHE STRING
  "Flags used for linking binaries during Sanitize builds."
  FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_SANITIZE
  ${CMAKE_SHARED_LINKER_FLAGS_DEBUG} CACHE STRING
  "Flags used by the shared libraries linker during Sanitize builds."
  FORCE)
mark_as_advanced(
  CMAKE_CXX_FLAGS_SANITIZE		  CMAKE_EXE_LINKER_FLAGS_SANITIZE
  CMAKE_C_FLAGS_SANITIZE		  CMAKE_SHARED_LINKER_FLAGS_SANITIZE
  )

set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
  "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel Sanitize."
  FORCE)

############################### External Projects ###############################

include(ExternalProject)

ExternalProject_Add(catch2
	URL https://github.com/catchorg/Catch2/archive/v2.9.2.tar.gz
  DOWNLOAD_DIR $ENV{HOME}/.cmake-downloads
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${Roasty_BINARY_DIR}/deps -DCATCH_BUILD_TESTING=NO -DBUILD_TESTING=NO -DCATCH_ENABLE_WERROR=NO -DCATCH_INSTALL_DOCS=NO -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}  -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DDCMAKE_CXX_FLAGS=${DCMAKE_CXX_FLAGS}
  )

ExternalProject_Add(cpp-httplib
	URL "https://github.com/yhirose/cpp-httplib/archive/v0.7.8.tar.gz"
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${Roasty_BINARY_DIR}/deps -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}  -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DDCMAKE_CXX_FLAGS=${DCMAKE_CXX_FLAGS}
  )

ExternalProject_Add(json
	URL "https://github.com/nlohmann/json/archive/v3.9.1.tar.gz"
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${Roasty_BINARY_DIR}/deps -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}  -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DDCMAKE_CXX_FLAGS=${DCMAKE_CXX_FLAGS}
  )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#################################### Targets ####################################
include_directories(PUBLIC ${Roasty_BINARY_DIR}/deps/include)

# Subdirectory where any implementation files are defined
add_subdirectory(Source)

set(TestFiles Tests/RoastyTests.cpp Tests/SerialisationTests.cpp Tests/RoastTests.cpp)

add_executable(Roasty ${ImplementationFiles} ${ExecutableFiles})
target_link_libraries(Roasty PRIVATE Threads::Threads)
set_property(TARGET Roasty PROPERTY CXX_STANDARD 17)
target_include_directories(Roasty SYSTEM PUBLIC ${Roasty_BINARY_DIR}/deps/include)
add_dependencies(Roasty cpp-httplib json)

add_executable(Tests ${ImplementationFiles} ${TestFiles})
target_link_libraries(Tests PRIVATE Threads::Threads)
set_property(TARGET Tests PROPERTY CXX_STANDARD 17)
target_include_directories(Tests SYSTEM PUBLIC ${Roasty_BINARY_DIR}/deps/include)
add_dependencies(Tests catch2 cpp-httplib json)
