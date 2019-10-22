cmake_minimum_required(VERSION 3.10)

set(replication_sources src/main.cpp
                        src/game_object.cpp
                        src/streams.cpp
						src/compress.cpp
						src/uncompress.cpp
						src/player.cpp
						src/enemy.cpp
						src/classregistry.cpp
			            src/LinkingContext.cpp)

include_directories(include)
set(replication_headers include/game_object.hpp
                        include/utils.hpp
                        include/streams.hpp
						include/coordinates.hpp
						include/compress.hpp
						include/uncompress.hpp
						include/player.hpp
						include/enemy.hpp
						include/classregistry.hpp
			            include/LinkingContext.hpp)

add_executable(tp2 ${replication_sources} ${replication_headers})
target_link_libraries(tp2 PUBLIC uvw_interface GSL)