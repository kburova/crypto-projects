cmake_minimum_required(VERSION 3.6)
project(Prog2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -I/Users/kseniaburova/Documents/school/Fall2016/cs483/Prog2/include -I/usr/local/opt/openssl/include " )


set(SOURCE_FILES src/LockDriver.cpp src/lock.cpp src/sig.cpp src/rsa.cpp src/aes.cpp src/MAC.cpp src/unlock.cpp)

add_executable(key ${SOURCE_FILES})