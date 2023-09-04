from auxiliary import *

TOOL_NAME = 'clang-tidy'
GENERATOR = 'Ninja'
C_COMPILER = 'clang'
CXX_COMPILER = 'clang++'
BUILD_DIR = path.join(OUTPUT_DIR, 'build_clang_tidy/')
SCRIPT_FILEPATH = path.join(CMAKE_DIR, 'run_clang_tidy.cmake')
OUTPUT_FILEPATH = path.join(OUTPUT_DIR, 'clang_tidy_log.txt')

class ClangTidyRunner():
    def __init__(self):
        if not is_tool_available(TOOL_NAME):
            raise Exception(f"{TOOL_NAME} not available")
        
        if not is_tool_available(GENERATOR):
            raise Exception(f"{GENERATOR} not available")

        if not is_tool_available(C_COMPILER) or not is_tool_available(CXX_COMPILER):
            raise Exception(f"{C_COMPILER} not available")

        self.configure_command = CMakeCommand()
        self.configure_command.add_option(f"-S .")
        self.configure_command.add_option(f"-B {BUILD_DIR}")
        self.configure_command.add_option(f"-G {GENERATOR}")
        self.configure_command.add_option(f"-DCMAKE_C_COMPILER={C_COMPILER}")
        self.configure_command.add_option(f"-DCMAKE_CXX_COMPILER={CXX_COMPILER}")
        self.configure_command.add_option(f"-DCMAKE_EXPORT_COMPILE_COMMANDS=ON")
        self.configure_command.add_option(f"-DBUILD_SHARED_LIBS=OFF")

        self.build_command = CMakeCommand()
        self.build_command.add_option(f"--build {BUILD_DIR}")
        self.build_command.add_option(f"--parallel")

        self.script_command = CMakeCommand()
        self.script_command.add_option(f"-DPROJECT_BINARY_DIR={BUILD_DIR}")
        self.script_command.add_option(f"-P {SCRIPT_FILEPATH}")

    def __prepare_compile_commands_file(self):
        COMPILE_COMMANDS_FILEPATH = path.join(BUILD_DIR, 'compile_commands.json')
        
        with open(COMPILE_COMMANDS_FILEPATH, 'r') as file:
            data = file.read()
        
        from re import sub
        modified_data = sub(r"(-I)([^ ]*third_party[^ ]*include\b)", r"-isystem \2", data)

        with open(COMPILE_COMMANDS_FILEPATH, 'w') as file:
            file.write(modified_data)

    def run(self):
        self.configure_command.run()
        self.build_command.run()
        self.__prepare_compile_commands_file()
        with open(OUTPUT_FILEPATH, 'w') as file:
            self.script_command.run(stdout=file)
