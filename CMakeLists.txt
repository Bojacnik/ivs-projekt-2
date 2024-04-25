
cmake_minimum_required(VERSION 3.0)

project(IVS_Calc)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_CONTACT "xnemecv00@stud.fit.vutbr.cz")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Calculator Installer")
set(CPACK_PACKAGE_VERSION "0.42.69")

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libmpfr-dev, libasound2-dev, libx11-dev, libxrandr-dev, libxi-dev, libgl1-mesa-dev,
libglu1-mesa-dev, libxcursor-dev, libxinerama-dev, libwayland-dev, libxkbcommon-dev")

install(FILES build/calc
        DESTINATION /opt/${PROJECT_NAME}
        PERMISSIONS WORLD_EXECUTE GROUP_EXECUTE OWNER_READ OWNER_WRITE)
install(FILES IVScalc.png
        DESTINATION /usr/share/icons/hicolor/32x32/apps)
install(FILES profiling/profiling
        DESTINATION /opt/${PROJECT_NAME}
        PERMISSIONS WORLD_EXECUTE OWNER_WRITE OWNER_READ)
install(FILES IVS_Calc.desktop
        DESTINATION /usr/share/applications)
install(FILES LICENSE
        DESTINATION /opt/${PROJECT_NAME})

include(CPack)