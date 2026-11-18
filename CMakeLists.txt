# Debian packaging (CPack). The binaries are built by the Makefile first:
#   make package   ->   build/package/ivs-calc_<version>_<arch>.deb
cmake_minimum_required(VERSION 3.16)

project(IVS_Calc VERSION 1.0.0 LANGUAGES NONE)

set(BUILD_DIR "${CMAKE_SOURCE_DIR}/build")
set(INSTALL_DIR "/opt/${PROJECT_NAME}")

install(PROGRAMS "${BUILD_DIR}/calc" "${BUILD_DIR}/profiling"
        DESTINATION "${INSTALL_DIR}")
install(FILES LICENSE
        DESTINATION "${INSTALL_DIR}")
install(FILES packaging/IVScalc.png
        DESTINATION /usr/share/icons/hicolor/32x32/apps)
install(FILES packaging/IVS_Calc.desktop
        DESTINATION /usr/share/applications)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME "ivs-calc")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_CONTACT "xnemecv00@stud.fit.vutbr.cz")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Arbitrary-precision calculator (IVS project 2)")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_SECTION "math")
# Runtime library dependencies (libmpfr, libX11, ...) are detected from the binaries.
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
# raylib loads OpenGL at runtime (dlopen), so it is not detected automatically.
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libgl1")

include(CPack)
