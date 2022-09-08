# Module Development Library for C++

This is a C++ library to aid on module development for locenv.

## Why C++, not C?

We don't recommend C due to how Lua error works. Lua has 2 mechanism for trigger errors:

- Using `setjmp` and `longjmp`.
- Using C++ exception.

We cannot use the first one due to it will cause resource leak on Rust, so the only choices available for us is C++. If you use C to create a module keep in mind that you don't have a chance to free the resources when Lua error is triggered. So with C you are going to run into resource leak for sure.

## Develop a locenv module with C++

First create a new public repository on GitHub. Currently locenv only support installing a module from public repository on GitHub. Then clone the repository to your computer and change a directory to it. Add https://github.com/locenv/mdl-cpp.git as a sub-module:

```sh
git submodule add https://github.com/locenv/mdl-cpp.git mdl
```

Make sure you have [CMake](https://cmake.org) installed before proceed to the next step. When ready create a `CMakeLists.txt` with the following content:

```cmake
cmake_minimum_required(VERSION 3.13)

project(yourmodule CXX)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_subdirectory(mdl)

add_library(${CMAKE_PROJECT_NAME} SHARED src/main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} locenv)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -static-libstdc++)
endif()
```

Don't forget to change your module name in `project(yourmodule CXX)`.

### Sample module

```cpp
// src/main.cpp
#include <locenv/loader.hpp>

#include <array>

using locenv::api;
using locenv::lua;
using locenv::lua_reg;

static int myfunction(lua lua)
{
	// We can access the context here because we make it as the upvalue for this function in the
	// loader.
	auto &context = locenv::context::get(lua, locenv::lua_upvalue(1));

	return 0;
}

static int loader(lua lua)
{
	static const std::array functions{
		lua_reg{ "myfunction", myfunction },
		lua_reg{ nullptr, nullptr }
	};

	// More information about 'loader': https://www.lua.org/manual/5.4/manual.html#6.3
	// The loader data is locenv::context (the actual object is stored within Lua userdata).
	api->lua_createtable(lua, 0, static_cast<int>(functions.size()) - 1);
	api->lua_pushvalue(lua, 2); // Push the loader data as upvalue for all functions in `functions`.
	api->aux_setfuncs(lua, functions.data(), 1);

	// Return a function table that we just created on above.
	return 1;
}

LOCENV_MODULE_LOADER(loader)
```

We recommend the Lua official [manual](https://www.lua.org/manual/5.4/manual.html#4) for a quick reference. For more detailed we recommend this online [book](https://www.lua.org/pil/24.html).

### Create module definition

```yaml
# locenv-module.yml
name: yourmodule
```

Fields other than `name` will be automatically populate for you if you use GitHub Actions to publish your release as in the next section.

### Setup GitHub Actions to publish the module

```yaml
name: CD
on:
  push:
    tags:
    - '*'
jobs:
  build:
    name: Build
    strategy:
      matrix:
        os: [ubuntu-20.04, macos-11, windows-2022]
        include:
        - os: ubuntu-20.04
          cmake: cmake -B build -D CMAKE_BUILD_TYPE=Release
          build: cmake --build build
          binary: build/libyourmodule.so
          artifact: amd64-linux
        - os: macos-11
          cmake: cmake -B build -D CMAKE_BUILD_TYPE=Release
          build: cmake --build build
          binary: build/libyourmodule.dylib
          artifact: amd64-darwin
        - os: windows-2022
          cmake: cmake -B build
          build: cmake --build build --config Release
          binary: build/Release/launcher.dll
          artifact: amd64-win32
    runs-on: ${{ matrix.os }}
    steps:
    - name: Checkout source
      uses: actions/checkout@v3
      with:
        submodules: 'recursive'
    - name: Run CMake
      run: ${{ matrix.cmake }}
    - name: Build
      run: ${{ matrix.build }}
    - name: Upload module binary
      uses: actions/upload-artifact@v3
      with:
        name: ${{ matrix.artifact }}
        path: ${{ matrix.binary }}
  release:
    name: Release
    runs-on: ubuntu-20.04
    permissions:
      contents: write
    needs: build
    steps:
    - name: Checkout source
      uses: actions/checkout@v3
    - name: Download Linux binary
      uses: actions/download-artifact@v3
      with:
        name: amd64-linux
        path: amd64-linux
    - name: Download macOS binary
      uses: actions/download-artifact@v3
      with:
        name: amd64-darwin
        path: amd64-darwin
    - name: Download Windows binary
      uses: actions/download-artifact@v3
      with:
        name: amd64-win32
        path: amd64-win32
    - name: Prepare package content
      run: |
        mkdir -pv package
        mv -v amd64-linux/libyourmodule.so package/amd64-linux.so
        mv -v amd64-darwin/libyourmodule.dylib package/amd64-darwin.dylib
        mv -v amd64-win32/yourmodule.dll package/amd64-win32.dll
    - name: Transform module definition
      run: |
        require 'yaml'

        mod = YAML.load_file('locenv-module.yml')
        mod['version'] = Integer(ENV['GITHUB_REF_NAME'])
        mod['program'] = {
          'linux' => {
            'amd64' => 'amd64-linux.so'
          },
          'darwin' => {
            'amd64' => 'amd64-darwin.dylib'
          },
          'win32' => {
            'amd64' => 'amd64-win32.dll'
          }
        }

        File.open('package/locenv-module.yml', 'w') { |f| f.write mod.to_yaml.gsub("---\n", '') }
      shell: ruby {0}
    - name: Create package
      run: zip -r ../package.zip *
      working-directory: package
    - name: Create release
      uses: softprops/action-gh-release@v1
      with:
        files: package.zip
```

Each time you push a new tag, which tag name required to be a non-negative number (e.g. 0, 1, 2 and so on); it will create a release for you automatically.

### Install your module

```sh
locenv mod install github:user/repository
```

### Using your module

Here is the example how to use your module from build script:

```lua
# locenv-service.yml
linux:
  build: |
    local foo = require 'yourmodule'

    foo.myfunction()
```

## License

MIT
