#!/usr/bin/env python3

import argparse
from itertools import product
from pathlib import Path
import re
import subprocess
from typing import NamedTuple

class CompilationRunner():
    GENERATOR = "Ninja"
    C_COMPILERS = ("clang",)
    CXX_COMPILERS = ("clang++",)
    CXX_FLAGS="-std=c++17"
    BUILD_TYPE = ("Static", "Shared")
    CONFIG_TYPE = ("Debug", "Release")
    CONFIGURATION_MATRIX = list(product(*[BUILD_TYPE, CONFIG_TYPE]))

    class Config(NamedTuple):
        build_dir_name: str
        c_compiler : str
        cxx_compiler : str
        build_shared_libs : str
        config_type : str
     
    def __init__(self, output_dir_name : str):
        self.configure_commands = []
        self.build_commands = []
        self.output_file_names = []
        self.output_dir_name = output_dir_name

        for c_compiler, cxx_compiler in zip(self.C_COMPILERS, self.CXX_COMPILERS):
            for build_type, config_type in self.CONFIGURATION_MATRIX:
                config_postfix = f"{c_compiler}_{build_type.lower()}_{config_type.lower()}"
                build_dir_name = f"{output_dir_name}/build_{config_postfix}"
                output_file_name = f"{output_dir_name}/{config_postfix}.txt"
                build_shared_libs = self.__build_shared_libs(build_type)

                config = self.Config(build_dir_name,
                                         c_compiler, 
                                         cxx_compiler,
                                         build_shared_libs,
                                         config_type)
                configure_command = self.__prepare_configure_command(config)
                self.configure_commands.append(configure_command)

                build_command = self.__prepare_build_command(build_dir_name)
                self.build_commands.append(build_command)

                self.output_file_names.append(output_file_name)

                
    def __build_shared_libs(self, build_type : str):
        return "ON" if build_type == "Shared" else "OFF"
    
    def __prepare_configure_command(self, config : Config):
        command = []
        command.append(f"cmake")
        command.append(f"-S")
        command.append(f".")
        command.append(f"-B")
        command.append(f"{config.build_dir_name}")
        command.append(f"-G{self.GENERATOR}")
        command.append(f"-DCMAKE_C_COMPILER={config.c_compiler}")
        command.append(f"-DCMAKE_CXX_COMPILER={config.cxx_compiler}")
        command.append(f"-DCMAKE_CXX_FLAGS={self.CXX_FLAGS}")
        command.append(f"-DBUILD_SHARED_LIBS={config.build_shared_libs}")
        command.append(f"-DCMAKE_BUILD_TYPE={config.config_type}")
        return command
    
    def __prepare_build_command(self, build_dir_name):
        command = []
        command.append(f"cmake")
        command.append(f"--build")
        command.append(f"{build_dir_name}")
        command.append(f"--parallel")
        return command
    
    def run(self):
        for configure_command, build_command, output_file_name in zip(self.configure_commands, 
                                                                      self.build_commands,
                                                                      self.output_file_names):
            output_file = open(output_file_name, "w")
            subprocess.check_call(configure_command, shell=True)
            subprocess.check_call(build_command, stdout=output_file, stderr=output_file, shell=True)
         
class ClangTidyRunner():
    GENERATOR="Ninja"
    C_COMPILER="gcc"
    CXX_COMPILER="g++"
    CXX_FLAGS="-std=c++17"
    BUILD_DIR_NAME="build_clang_tidy"
    OUTPUT_FILE_NAME="clang_tidy_log.txt"
    CMAKE_SCRIPT="cmake/run_clang_tidy.cmake"

    def __init__(self, output_dir_name : str):
        self.output_dir_name = output_dir_name
        self.build_dir_name = f"{output_dir_name}/{self.BUILD_DIR_NAME}"
        self.output_file_name = f"{output_dir_name}/{self.OUTPUT_FILE_NAME}"

        self.__prepare_configure_command()
        self.__prepare_run_command()

    def __prepare_configure_command(self):
        self.configure_command = []
        self.configure_command.append(f"cmake")
        self.configure_command.append(f"-S")
        self.configure_command.append(f".")
        self.configure_command.append(f"-B")
        self.configure_command.append(f"{self.output_dir_name}/{self.BUILD_DIR_NAME}")
        self.configure_command.append(f"-G{self.GENERATOR}")
        self.configure_command.append(f"-DCMAKE_C_COMPILER={self.C_COMPILER}")
        self.configure_command.append(f"-DCMAKE_CXX_COMPILER={self.CXX_COMPILER}")
        self.configure_command.append(f"-DCMAKE_CXX_COMPILER={self.CXX_COMPILER}")
        self.configure_command.append(f"-DCMAKE_CXX_FLAGS={self.CXX_FLAGS}")
        self.configure_command.append(f"-DCMAKE_EXPORT_COMPILE_COMMANDS=ON")
        self.configure_command.append(f"-DBUILD_SHARED_LIBS=OFF")

    def __prepare_run_command(self):
        self.run_command = []
        self.run_command.append(f"cmake")
        self.run_command.append(f"-DPROJECT_BINARY_DIR={self.build_dir_name}")
        self.run_command.append(f"-P")
        self.run_command.append(f"{self.CMAKE_SCRIPT}")

    def __prepare_compile_commands_file(self):
        with open(f"{self.build_dir_name}/compile_commands.json", "r") as file:
            data = file.read()
        
        modified_data = re.sub(r"(-I)([^ ]*third_party[^ ]*include\b)", r"-isystem \2", data)

        with open(f"{self.build_dir_name}/compile_commands.json", "w") as file:
            file.write(modified_data)

    def run(self):
        output_file = open(f"{self.output_file_name}", "w")
        subprocess.check_call(self.configure_command, shell=True)
        self.__prepare_compile_commands_file()
        subprocess.check_call(self.run_command, stdout=output_file, shell=True)

class ClangFormatRunner():
    CMAKE_SCRIPT="cmake/run_clang_format.cmake"

    def __init__(self):
        self.__prepare_run_command()
    
    def __prepare_run_command(self):
        self.run_command = []
        self.run_command.append(f"cmake")
        self.run_command.append(f"-P")
        self.run_command.append(f"{self.CMAKE_SCRIPT}")

    def run(self):
        subprocess.check_call(self.run_command, shell=True)

def is_root_dir_correct(root_dir : Path):
    result = True
    result = result & root_dir.is_dir()
    result = result & Path("CMakeLists.txt").is_file()
    result = result & Path("scripts").is_dir()
    result = result & Path("cmake").is_dir()
    return result

def main():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("root_dir", help="project's root directory")
    parser.add_argument("-f", "--clang-format", action="store_true", help="run clang-format")
    parser.add_argument("-o", "--output-dir", help="script's output directory")
    parser.add_argument("-t", "--clang-tidy", action="store_true", help="run clang-tidy")
    parser.add_argument("-c", "--compile", action="store_true", help="compile project")
    args = parser.parse_args()
    config = vars(args)

    root_dir = Path(config["root_dir"])
    if not is_root_dir_correct(root_dir):
        raise ValueError(f"Given root_dir is not a root directory")
    
    if config["clang_format"]:
        ClangFormatRunner().run()
    
    output_dir_name = config["output_dir"]
    if config["clang_tidy"] or config["compile"]:
        Path(output_dir_name).mkdir(exist_ok=True)

    if config["clang_tidy"]:
        ClangTidyRunner(output_dir_name).run()

    if config["compile"]:
        CompilationRunner(output_dir_name).run()

if __name__ == "__main__":
     main()
