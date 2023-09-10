# CMakeList.txt: Tcp 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Tcp")

set (RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_DIR})

set (LIBRARY_OUTPUT_DIRECTORY ${LIBARARY_DIR})

set (ARCHIVE_OUTPUT_DIRECTORY ${ARCHIVE_DIR})

set (BOOST_ROOT D:/ExternalLibrary/boost_1_82_0)

add_subdirectory (jsoncpp)

find_package (Boost COMPONENTS REQUIRED)

include_directories (include jsoncpp/include)

file (GLOB include_file jsoncpp/include/json/*.h jsoncpp/src/lib_json/*.h include/*.h)

# 将源代码添加到此项目的可执行文件。
file (GLOB file src/*.cpp jsoncpp/src/lib_json/*.cpp)

add_library (Tcp STATIC ${file} ${include_file})

target_link_libraries (Tcp PRIVATE Boost::boost)

target_link_libraries (Tcp PRIVATE SqlConnPool)

target_link_libraries (Tcp PRIVATE spdlog)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Tcp PROPERTY CXX_STANDARD 14)
endif()

# TODO: 如有需要，请添加测试并安装目标。
