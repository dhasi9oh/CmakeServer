# CMakeList.txt: Log 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Log")

set (CMAKE_CXX_STANDARD 14)

set (RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_DIR})

set (LIBRARY_OUTPUT_DIRECTORY ${LIBARARY_DIR})

set (ARCHIVE_OUTPUT_DIRECTORY ${ARCHIVE_DIR})

# 将源代码添加到此项目的可执行文件。
add_subdirectory (spdlog)

include_directories (./include ./spdlog/include)

aux_source_directory (./src src_file)

file (GLOB include_file include/*.h)

add_library (Log SHARED ${src_file} ${include_file})

target_link_libraries (Log PUBLIC spdlog::spdlog_header_only)



# TODO: 如有需要，请添加测试并安装目标。
