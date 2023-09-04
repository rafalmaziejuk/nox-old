from os import path

ROOT_DIR = path.abspath(path.join(path.dirname(__file__), '../'))
OUTPUT_DIR = path.join(ROOT_DIR, 'output/')
CMAKE_DIR = path.join(ROOT_DIR, 'cmake/')

class CMakeCommand():
    def __init__(self):
        self.command = []
        self.command.append('cmake')

    def add_option(self, option : str):
        self.command.extend(option.split())
    
    def run(self, stdout: str=None, stderr: str=None):
        from subprocess import check_call
        check_call(self.command, cwd=ROOT_DIR, stdout=stdout, stderr=stderr)

def is_tool_available(tool_name : str):
    from shutil import which
    return which(tool_name.lower()) is not None
