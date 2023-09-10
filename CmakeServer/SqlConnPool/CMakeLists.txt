# CMakeList.txt: SqlConnPool 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SqlConnPool")

set (RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_DIR})

set (LIBRARY_OUTPUT_DIRECTORY ${LIBARARY_DIR})

set (ARCHIVE_OUTPUT_DIRECTORY ${ARCHIVE_DIR})

set (CMAKE_CXX_STANDARD 14)

set (BOOST_ROOT D:/ExternalLibrary/boost_1_82_0)

include_directories (include include/mysql)

find_package(Boost REQUIRED COMPONENTS)

link_directories (${Boost_LIBRARY_DIRS} bin)

file (GLOB inlude_file include/*.h include/mysql/*.h)

aux_source_directory (src file)

add_library (SqlConnPool STATIC ${inlude_file} ${file})

set_target_properties(SqlConnPool PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries (SqlConnPool PRIVATE Boost::boost)

target_link_libraries (SqlConnPool PRIVATE libmysql)


# TODO: 如有需要，请添加测试并安装目标。
