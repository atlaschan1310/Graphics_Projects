#!/bin/bash
cwd=$(pwd)

cd ..
if [ -d ./vcpkg ]; then
  echo "vcpkg has already been installed"
else
  git clone https://github.com/microsoft/vcpkg.git
fi

cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg update
./vcpkg install glfw3 glew glm cimg

cd $cwd
