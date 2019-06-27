include(ExternalProject)
ExternalProject_Add(gtest-install
        URL https://github.com/google/googletest/archive/release-1.8.1.zip
        URL_HASH SHA1=7b41ea3682937069e3ce32cb06619fead505795e
 	SOURCE_DIR        "${CMAKE_BINARY_DIR}/gtest-src"
	INSTALL_DIR       "${CMAKE_BINARY_DIR}/gtest"
	CONFIGURE_COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -S <SOURCE_DIR> 
        BUILD_COMMAND make
        INSTALL_COMMAND make install
)
