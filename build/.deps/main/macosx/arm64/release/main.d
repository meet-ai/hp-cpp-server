{
    files = {
        "build/.objs/main/macosx/arm64/release/src/main.cpp.o"
    },
    values = {
        "/usr/bin/xcrun -sdk macosx clang++",
        {
            "-arch",
            "arm64",
            "-stdlib=libc++",
            "-Wl,-x",
            "-lz"
        }
    }
}