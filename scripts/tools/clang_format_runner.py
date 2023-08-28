from auxiliary import *

TOOL_NAME = "clang-format"
SCRIPT_FILE_PATH = path.join(CMAKE_DIR, 'run_clang_format.cmake')

class ClangFormatRunner():
    def __init__(self):
        if not is_tool_available(TOOL_NAME):
            raise Exception(f"{TOOL_NAME} not available")

        self.script_command = CMakeCommand()
        self.script_command.add_option(f"-P {SCRIPT_FILE_PATH}")
    
    def run(self):
        self.script_command.run()
