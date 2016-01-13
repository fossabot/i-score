set(CMAKE_BUILD_TYPE Release)
set(ISCORE_COTIRE True)
set(DEPLOYMENT_BUILD True)
set(ISCORE_MERGE_COMPONENTS True)
set(ISCORE_STATIC_PLUGINS True)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_MAKE_PROGRAM mingw32-make)
set(Boost_USE_STATIC_LIBS ON)
set(BOOST_ROOT "$ENV{BOOST_ROOT}")
set(BOOST_LIBRARYDIR "$ENV{BOOST_LIBRARYDIR}")
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};$ENV{QTDIR}/lib/cmake/Qt5;$ENV{JAMOMA_ROOT}/share/cmake/jamoma")

include(default-plugins)
