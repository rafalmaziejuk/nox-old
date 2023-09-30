from auxiliary import *

TOOL_NAME = 'clang-tidy'
GENERATOR = 'Ninja'
C_COMPILER = 'clang'
CXX_COMPILER = 'clang++'
BUILD_DIR = os.path.join(OUTPUT_DIR, 'build_clang_tidy/')
SCRIPT_FILEPATH = os.path.join(CMAKE_DIR, 'run_clang_tidy.cmake')

class ClangTidyRunner():
    def __init__(self):
        is_tool_available(TOOL_NAME)
        is_tool_available(GENERATOR)
        is_tool_available(C_COMPILER)
        is_tool_available(CXX_COMPILER)
        
        self.configure_command = CMakeCommand()\
                                 .append(f"-S .")\
                                 .append(f"-B {BUILD_DIR}")\
                                 .append(f"-G {GENERATOR}")\
                                 .append(f"-DCMAKE_C_COMPILER={C_COMPILER}")\
                                 .append(f"-DCMAKE_CXX_COMPILER={CXX_COMPILER}")\
                                 .append(f"-DCMAKE_EXPORT_COMPILE_COMMANDS=ON")\
                                 .append(f"-DBUILD_SHARED_LIBS=OFF") 

        self.build_command = CMakeCommand()\
                             .append(f"--build {BUILD_DIR}")\
                             .append(f"--parallel")

        self.script_command = CMakeCommand()\
                              .append(f"-DPROJECT_BINARY_DIR={BUILD_DIR}")\
                              .append(f"-P {SCRIPT_FILEPATH}")

    def __prepare_compile_commands_file(self):
        COMPILE_COMMANDS_FILEPATH = os.path.join(BUILD_DIR, 'compile_commands.json')
        
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
        
        result = self.script_command.run()
        if result.returncode != 0:
            print(result.stdout)
