cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug --parallel 6
cmake --install build --config Debug --prefix install_dir