from auxiliary import *
from configs import *

class BuildRunner():
    def __init__(self):
        self.arguments = []

        for config in CONFIGS:
            skip = False
            generator_not_empty = config.generator != ''
            c_compiler_not_empty = config.c_compiler != ''
            cxx_compiler_not_empty = config.cxx_compiler != ''

            if generator_not_empty and c_compiler_not_empty and cxx_compiler_not_empty:
                skip = not is_tool_available(config.generator)
                skip = not is_tool_available(config.c_compiler)
                skip = not is_tool_available(config.cxx_compiler)

            if skip:
                continue

            argument = {'configure_command': CMakeCommand(),
                        'build_command': CMakeCommand(),
                        'output_filepath': str}
            
            library_type = "Shared" if config.build_shared_libs == "ON" else "Static"
            config_postfix = f"{config.c_compiler}_{library_type.lower()}_{config.build_type.lower()}"
            build_dir = path.join(OUTPUT_DIR, f"build_{config_postfix}")

            argument['configure_command'].add_option(f"-S .")
            argument['configure_command'].add_option(f"-B {build_dir}")
            argument['configure_command'].add_option(f"-DCMAKE_C_COMPILER={config.c_compiler}")
            argument['configure_command'].add_option(f"-DCMAKE_CXX_COMPILER={config.cxx_compiler}")
            argument['configure_command'].add_option(f"-DBUILD_SHARED_LIBS={config.build_shared_libs}")
            argument['configure_command'].add_option(f"-DCMAKE_BUILD_TYPE={config.build_type}")
            if generator_not_empty:
                argument['configure_command'].add_option(f"-G {config.generator}")

            argument['build_command'].add_option(f"--build {build_dir}")
            argument['build_command'].add_option(f"--parallel")

            argument['output_filepath'] = path.join(OUTPUT_DIR, f"log_{config_postfix}.txt")

            self.arguments.append(argument)

        if not self.arguments:
            raise Exception('Cannot execute any of the configurations')
                
    def run(self):
        for argument in self.arguments:
            argument['configure_command'].run()
            with open(argument['output_filepath'], 'w') as file:
                argument['build_command'].run(stdout=file, stderr=file)
