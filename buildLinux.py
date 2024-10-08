import os
import subprocess

# Ruta a los directorios
root_dir = os.path.abspath(os.path.dirname(__file__))  # Ruta al directorio raíz
build_dir = os.path.join(root_dir, 'build', 'Release')  # Ruta a build/Release

# Comandos
cmake_command = [
    "cmake", "../..", 
    "-DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake", 
    "-DCMAKE_BUILD_TYPE=Release"
]

build_command = ["cmake", "--build", "."]


def run_command(command, cwd=None):
    """Ejecuta un comando en la terminal."""
    try:
        # Ejecutar el comando sin capturar la salida
        result = subprocess.run(command, cwd=cwd, check=True, text=True)

    except subprocess.CalledProcessError as e:
        print(f"Error al ejecutar el comando: {e}")
        raise

def main():
    # Verificar si el directorio build/Release existe
    if not os.path.exists(build_dir):
        print(f"El directorio {build_dir} no existe.")
        return
    
    # Ejecutar el comando cmake en build/Release
    print(f"Moviéndose a {build_dir} y ejecutando CMake...")
    run_command(cmake_command, cwd=build_dir)
    
    # Compilar el proyecto
    print("Compilando el proyecto...")
    run_command(build_command, cwd=build_dir)
    
if __name__ == "__main__":
    main()


#cmake  -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release ../..