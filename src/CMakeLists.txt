include_directories(${BOOST_ROOT} ${THREAD_ROOT})
link_directories(${BOOST_INCLUDEDIR} ${THREAD_INCLUDEDIR})

#Configuration de l'exécutable
file(
        GLOB_RECURSE
        source_files
        http_client_async.cpp
)

add_executable(
        ../binary/http_grabber
	${source_files}
)

target_link_libraries(
        ../binary/http_grabber
	${Boost_SYSTEM_LIBRARY}
	pthread
)
