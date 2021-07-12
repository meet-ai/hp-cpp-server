{
    values = {
        "/usr/bin/xcrun -sdk macosx clang",
        {
            "-Qunused-arguments",
            "-arch",
            "arm64",
            "-fvisibility=hidden",
            "-fvisibility-inlines-hidden",
            "-O3",
            "-isystem",
            "/Users/meetai/.xmake/packages/g/gtest/1.10.0/dbaf6b5094c24dd7bf1da104bfec6517/include",
            "-isystem",
            "/Users/meetai/.xmake/packages/s/spdlog/v1.8.5/e3d09d62f31748529f5b700437f219d6/include",
            "-std=c++14",
            "-DNDEBUG"
        }
    },
    files = {
        "v1/main.cpp"
    },
    depfiles_gcc = "build/.objs/v1/macosx/arm64/release/v1/main.cpp.o: v1/main.cpp\
"
}