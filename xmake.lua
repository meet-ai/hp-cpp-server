add_rules("mode.debug", "mode.release")

add_rules("mode.debug", "mode.release")
add_cxxflags("-std=c++14")
add_requires("gtest")
add_requires("spdlog")
--add_requires("cpp-httplib")
--add_requires("libcurl", {system=false})
--add_requires("openssl")
--add_requires("co", {system=false, configs={openssl=true, libcurl=true}})
--add_requires("vcpkg::fmt")
--add_requires("vcpkg::boost-algorithm")
--add_requires("vcpkg::nlohmann-json")
--add_defines("HAS_LIBCURL")
--add_defines("CODBG")
----add_defines("_GLIBCXX_USE_CXX11_ABI=0")
--add_ldflags("-ldl -lcrypto")

--
--
--
target("main")
    set_kind("binary")
    add_files("src/*.cpp")
--add_files("src/chess/*.cpp")
--    add_packages("co")
--    add_packages("vcpkg::nlohmann-json")
--    add_packages("cpp-httplib")
    add_packages("spdlog")
--add_packages("vcpkg::fmt")
target_end()

target("test")
    set_kind("binary")
    add_files("test/*.cpp")
    add_packages("gtest")
    add_packages("spdlog")
    add_packages("vcpkg::fmt")
target_end()

target("v1")
    set_kind("binary")
    add_files("v1/*.cpp")
    add_packages("gtest")
    add_packages("spdlog")
    add_packages("vcpkg::fmt")
    target_end()

target("v1-2")
    set_kind("binary")
    add_files("v1-2/*.cpp")
    add_packages("gtest")
    add_packages("spdlog")
    add_packages("vcpkg::fmt")
target_end()
--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

