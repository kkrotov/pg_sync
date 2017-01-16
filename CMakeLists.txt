cmake_minimum_required(VERSION 2.8)
project(pg_sync)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")

#include_directories(/usr/include/postgresql libs/jsoncpp-master/include)
include_directories(/usr/pgsql-9.4/include libs/jsoncpp-master/include libs/boost)
link_directories(/usr/pgsql-9.4/lib)

string(TIMESTAMP RUN_DATE "%Y-%m-%d" UTC)
string(TIMESTAMP RUN_TIME "%H:%M:%S" UTC)
execute_process (COMMAND git rev-parse --verify HEAD OUTPUT_VARIABLE CURRENT_HEAD
                 OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE)
add_definitions(-D_DATECOMPILED_="${RUN_DATE}"
                -D_TIMECOMPILED_="${RUN_TIME}"
                -D_CURRENT_HEAD_="${CURRENT_HEAD}")


set(BOOST_SOURCES
        libs/boost/system/src/error_code.cpp
        libs/boost/program_options/src/cmdline.cpp
        libs/boost/program_options/src/config_file.cpp
        libs/boost/program_options/src/convert.cpp
        libs/boost/program_options/src/options_description.cpp
        libs/boost/program_options/src/parsers.cpp
        libs/boost/program_options/src/positional_options.cpp
        libs/boost/program_options/src/split.cpp
        libs/boost/program_options/src/utf8_codecvt_facet.cpp
        libs/boost/program_options/src/value_semantic.cpp
        libs/boost/program_options/src/variables_map.cpp
        libs/boost/program_options/src/winmain.cpp
)

set(JSON_SOURCES
        libs/jsoncpp-master/src/lib_json/json_value.cpp
        libs/jsoncpp-master/src/lib_json/json_reader.cpp
        libs/jsoncpp-master/src/lib_json/json_writer.cpp
)

set(BILL_SOURCES
        src/main.cpp
        src/common.cpp
        src/version.cpp

        src/classes/daemon/DaemonApp.cpp
        src/classes/daemon/DaemonWorker.cpp
        src/classes/daemon/DaemonMonitor.cpp
        src/classes/App.cpp
        src/classes/AppBill.cpp
        src/classes/BaseLogWriter.cpp
        src/classes/BDb.cpp
        src/classes/BDbResult.cpp
        src/classes/BDbTransaction.cpp
        src/classes/DbException.cpp
        src/classes/Exception.cpp
        src/classes/Log.cpp
        src/classes/Logger.cpp
        src/classes/LogWriterFile.cpp
        src/classes/LogWriterScreen.cpp
        src/classes/LogWriterSyslog.cpp
        src/classes/LogWriterGraylog.cpp
        src/classes/Conf.cpp
        src/classes/ConfBill.cpp
        src/classes/Spinlock.cpp
        src/classes/Task.cpp
        src/classes/Thread.cpp
        src/classes/ThreadInterrupted.cpp
        src/classes/ThreadPool.cpp
        src/classes/Timer.cpp

        src/sync/BasePull.cpp
        src/sync/ManagerPull.cpp
        src/sync/PullTable.cpp
        src/sync/PullTable.h

        src/threads/ThreadLog.cpp
        src/threads/ThreadSyncMaster.cpp
        src/threads/ThreadWeb.cpp
        src/threads/ThreadSyncSlave.cpp
        src/threads/ThreadSyncSlave.h

        src/web/http/mime_types.cpp
        src/web/http/reply.cpp
        src/web/http/request_parser.cpp
        src/web/http/server.cpp
        src/web/BasePage.cpp
        src/web/HttpHandler.cpp
        src/web/PageHome.cpp
        src/web/PageLog.cpp
        src/web/PageSyncMaster.cpp
        src/web/PageSyncSlave.cpp
        src/web/PageSyncSlave.h)

#add_custom_target(invalidate_files ALL
#        COMMAND ${CMAKE_COMMAND} -E touch src/version.cpp)

add_definitions(-DHAVE_SYS_IOCTL_H -DHAVE_SOCKLEN_T)
#file(GLOB_RECURSE ACE_RADIUS_SOURCES "libs/ace-radius/*.cpp")

add_executable(pg_sync ${BILL_SOURCES} ${BOOST_SOURCES} ${JSON_SOURCES})
 #${ACE_RADIUS_SOURCES})
target_link_libraries(pg_sync rt pthread pq curl crypto)
