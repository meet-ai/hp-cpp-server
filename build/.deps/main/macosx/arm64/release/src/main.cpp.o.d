{
    files = {
        "src/main.cpp"
    },
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
            "/Users/meetai/.xmake/packages/s/spdlog/v1.8.5/e3d09d62f31748529f5b700437f219d6/include",
            "-std=c++14",
            "-DNDEBUG"
        }
    },
    depfiles_gcc = "build/.objs/main/macosx/arm64/release/src/main.cpp.o: src/main.cpp\
"
}