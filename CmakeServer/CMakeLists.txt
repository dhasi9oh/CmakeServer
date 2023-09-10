# CMakeList.txt: CmakeServer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("CmakeServer")

# 将源代码添加到此项目的可执行文件。

set (CMAKE_CXX_STANDARD 14)

set (TOP_DIR ${CMAKE_SOURCE_DIR})

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TOP_DIR}/bin)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${TOP_DIR}/bin)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${TOP_DIR}/lib)

set (RUNTIME_DIR ${TOP_DIR}/bin)

set (LIBARARY_DIR ${TOP_DIR}/bin)

set (ARCHIVE_DIR ${TOP_DIR}/lib)

set (LOG_DIR ${TOP_DIR}/Log)

set (SQLCONNPOOL_DIR ${TOP_DIR}/SqlConnPool)

set (TCP_DIR ${TOP_DIR}/Tcp)

set (BOOST_ROOT D:/ExternalLibrary/boost_1_82_0)

find_package (Boost COMPONENTS REQUIRED)

include_directories (
    include
    ${LOG_DIR}/include
    ${SQLCONNPOOL_DIR}/include
    ${SQLCONNPOOL_DIR}/include/mysql
    ${TCP_DIR}/include
)

add_subdirectory (${LOG_DIR})

add_subdirectory (${SQLCONNPOOL_DIR})

add_subdirectory (${TCP_DIR})

link_directories (
    ${LIBARARY_DIR}
    ${ARCHIVE_DIR}
    ${Boost_LIBRARY_DIRS}
)

aux_source_directory (src src_file)

add_executable (CmakeServer ${src_file})

target_link_libraries (CmakeServer PRIVATE Log)

target_link_libraries (CmakeServer PRIVATE libmysql)

target_link_libraries (CmakeServer PRIVATE Boost::boost)

target_link_libraries (CmakeServer PRIVATE Tcp)


# TODO: 如有需要，请添加测试并安装目标。
