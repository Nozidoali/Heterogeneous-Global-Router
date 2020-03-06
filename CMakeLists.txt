cmake_minimum_required ( VERSION 2.7 )

set ( CMAKE_CXX_STANDARD 14 )
set ( CMAKE_CXX_FLAGS "-m64 -O -fPIC -fexceptions -DNDEBUG -DIL_STD -g -Wall" )

set ( SOURCE_FILES 
        src/router.h

        src/base/base.h      src/base/base.cpp
        src/base/hgrid.h     src/base/hgrid.cpp
        src/base/point.h     src/base/point.cpp
        src/base/route.h     src/base/route.cpp
        
        src/util/util.h     src/util/util.cpp
        src/util/solver.h   src/util/solver.cpp
        src/util/fileio.h   src/util/fileio.cpp
        src/util/RMST.h     src/util/RMST.cpp
)

set ( LIBS
        pthread
)

add_executable( ROUTER.exe src/main.cpp ${SOURCE_FILES} )
target_link_libraries ( ROUTER.exe ${LIBS} )