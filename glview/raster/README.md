# About

This library is being use to organize the glview project. Also note that the tests are disabled by default, that way it can be compiled faster.

My college hybrid-raytracer was built ontop of glut. I'm choosing to use glfw now, so most of the old code will be deleted. This library is isolating the glfw code so I can potentially use it elsewhere.

## Linting

This library includes cpplint within CMakeLists.txt. It would be good to enable cpplint with a ci system like github actions. It can also be installed for fast local development.

Install and run with python
```
$ pip install cpplint
$ cpplint --recursive --root=include .
```

Using cmake, does not include the header files. So I removed this.
``` bash
cmake .. "-DCMAKE_CXX_CPPLINT=cpplint" ..
```

How you could enable with a flag
``` bash
$ cmake .. -DWITH_LINT=ON
```
``` CMakeLists.txt 
if (WITH_LINT)
  set(CMAKE_CXX_CPPLINT "cpplint")
endif()
```

## Build and run

Build and run the tests.
``` sh
mkdir build
cd build
cmake .. -DWITH_TEST=ON
make
ctest
```
