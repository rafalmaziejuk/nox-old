from auxiliary import *

TOOL_NAME = "clang-format"
SCRIPT_FILE_PATH = os.path.join(CMAKE_DIR, 'run_clang_format.cmake')

class ClangFormatRunner():
    def __init__(self):
        is_tool_available(TOOL_NAME)

        self.script_command = CMakeCommand().append(f"-P {SCRIPT_FILE_PATH}")
    
    def run(self):
        self.script_command.run()
