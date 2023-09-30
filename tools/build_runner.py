from auxiliary import *
from typing import NamedTuple

class Config(NamedTuple):
    generator: str
    c_compiler: str
    cxx_compiler: str
    build_type: str
    build_shared_libs: str

CONFIGS = [Config(generator='Ninja', c_compiler='clang', cxx_compiler='clang++', build_type='Debug', build_shared_libs='OFF'),
           Config(generator='Ninja', c_compiler='clang', cxx_compiler='clang++', build_type='Release', build_shared_libs='OFF'),
           Config(generator='Ninja', c_compiler='clang', cxx_compiler='clang++', build_type='Debug', build_shared_libs='ON'),
           Config(generator='Ninja', c_compiler='clang', cxx_compiler='clang++', build_type='Release', build_shared_libs='ON'),
           Config(generator='Ninja', c_compiler='gcc', cxx_compiler='g++', build_type='Debug', build_shared_libs='OFF'),
           Config(generator='Ninja', c_compiler='gcc', cxx_compiler='g++', build_type='Release', build_shared_libs='OFF'),
           Config(generator='Ninja', c_compiler='gcc', cxx_compiler='g++', build_type='Debug', build_shared_libs='ON'),
           Config(generator='Ninja', c_compiler='gcc', cxx_compiler='g++', build_type='Release', build_shared_libs='ON')]

class BuildRunner():
    def __init__(self):
        self.arguments = []

        for config in CONFIGS:
            skip = False
            skip = not is_tool_available(config.generator, check=False)
            skip = not is_tool_available(config.c_compiler, check=False)
            skip = not is_tool_available(config.cxx_compiler, check=False)

            if skip:
                continue
            
            library_type = "Shared" if config.build_shared_libs == "ON" else "Static"
            config_postfix = f"{config.c_compiler}_{library_type.lower()}_{config.build_type.lower()}"
            build_dir = os.path.join(OUTPUT_DIR, f"build_{config_postfix}")

            argument = {'configure_command': CMakeCommand(),
                        'build_command': CMakeCommand()}
            
            argument['configure_command'].append(f"-S .")\
                                         .append(f"-B {build_dir}")\
                                         .append(f"-G {config.generator}")\
                                         .append(f"-DCMAKE_C_COMPILER={config.c_compiler}")\
                                         .append(f"-DCMAKE_CXX_COMPILER={config.cxx_compiler}")\
                                         .append(f"-DBUILD_SHARED_LIBS={config.build_shared_libs}")\
                                         .append(f"-DCMAKE_BUILD_TYPE={config.build_type}")

            argument['build_command'].append(f"--build {build_dir}")\
                                     .append(f"--parallel")

            self.arguments.append(argument)

        if not self.arguments:
            raise Exception('Cannot execute any of the configurations')
                
    def run(self):
        for argument in self.arguments:
            argument['configure_command'].run()
            
            result = argument['build_command'].run()
            if result.returncode != 0:
                print(result.stdout)
                break
