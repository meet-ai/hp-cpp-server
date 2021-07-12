{
    files = {
        "build/.objs/test/macosx/arm64/release/test/test.cpp.o"
    },
    values = {
        "/usr/bin/xcrun -sdk macosx clang++",
        {
            "-arch",
            "arm64",
            "-stdlib=libc++",
            "-L/Users/meetai/.xmake/packages/g/gtest/1.10.0/dbaf6b5094c24dd7bf1da104bfec6517/lib",
            "-Wl,-x",
            "-lgtest_main",
            "-lgtest",
            "-lgmock",
            "-lz"
        }
    }
}