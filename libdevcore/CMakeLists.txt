set(CMAKE_CXX_FLAGS "-std=c++0x -fPIC ${CMAKE_CXX_FLAGS} -DSTATICLIB")

aux_source_directory(. SRC_LIST)

file(GLOB HEADERS "*.h")

add_library(devcore ${SRC_LIST} ${HEADERS})

#find_package(Boost QUIET REQUIRED COMPONENTS thread filesystem random system)

set(Boost_INCLUDE_DIRS /usr/local/include/boost)
set(Boost_FILESYSTEM_LIBRARIES /usr/local/lib/stage/lib/libboost_filesystem.a)
set(Boost_RANDOM_LIBRARIES /usr/local/lib/stage/lib/libboost_random.a)
set(Boost_SYSTEM_LIBRARIES /usr/local/lib/stage/lib/libboost_system.a)
set(Boost_THREAD_LIBRARIES /usr/local/lib/stage/lib/libboost_thread.a)
target_include_directories(devcore PRIVATE ..)
target_include_directories(devcore SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(devcore ${Boost_FILESYSTEM_LIBRARIES})
target_link_libraries(devcore ${Boost_RANDOM_LIBRARIES})
target_link_libraries(devcore ${Boost_SYSTEM_LIBRARIES})
target_link_libraries(devcore ${Boost_THREAD_LIBRARIES})

if (NOT EMSCRIPTEN)
    #    find_package(LevelDB)
    set(LEVELDB_INCLUDE_DIRS /usr/include/leveldb)
    set(LEVELDB_LIBRARIES /usr/lib/x86_64-linux-gnu)
    target_include_directories(devcore SYSTEM PUBLIC ${LEVELDB_INCLUDE_DIRS})
    target_link_libraries(devcore ${LEVELDB_LIBRARIES})
endif()

install(TARGETS devcore RUNTIME DESTINATION bin ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
