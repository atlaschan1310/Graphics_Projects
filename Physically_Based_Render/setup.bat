cd ..

if exist vcpkg\ (
  echo vcpkg has already been installed
) else (
    git clone https://github.com/microsoft/vcpkg.git
)

cd vcpkg
call bootstrap-vcpkg.bat
SET VCPKG_DEFAULT_TRIPLET=x64-windows

vcpkg update
vcpkg install glfw3 glew glm cimg assimp