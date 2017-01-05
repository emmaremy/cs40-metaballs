INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} .)

set(SHDRS sphere.h square.h blob.h cylinder.h matrixstack.h marchingcubes.h grid.h)
set(SSRC sphere.cpp square.cpp blob.cpp cylinder.cpp matrixstack.cpp marchingcubes.cpp grid.cpp)

add_library(libcommon ${SHDRS} ${SSRC})
