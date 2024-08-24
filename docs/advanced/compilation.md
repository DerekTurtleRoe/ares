Ares build documentation:

make -j4 build=optimized local=false cores=n64 console=true

Make is just "build it"
-j is threadcount (-j1 uses 1 thread)
local=false doesn't make any optimizations for processors
build=[debug|stable|release|minified|optimized]
cores=*** is compiling with an isolated core
console=true to add the console output
hiro= sets the version of hiro to use
compiler=g++ will use g++ as the compiler, other options include Clang/GCC
lto=false default is enabled, disables link time optimizations
