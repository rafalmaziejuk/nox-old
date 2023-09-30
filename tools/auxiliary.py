import os
import subprocess

ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '../'))
OUTPUT_DIR = os.path.join(ROOT_DIR, 'output/')
CMAKE_DIR = os.path.join(ROOT_DIR, 'cmake/')

def is_tool_available(tool: str, check: bool = True):
    from shutil import which
    result = which(tool.lower()) is not None

    if result == False and check:
        raise Exception(f"{tool} is not available")

    return result

class CMakeCommand():
    def __init__(self):
        self._command = ['cmake']

    def append(self, option: str):
        self._command.extend(option.split())
        return self

    def run(self):
        print(f"Running {self._command}", flush=True)
        output = subprocess.run(self._command,
                                cwd=ROOT_DIR,
                                capture_output=True,
                                text=True)
        if output.returncode == 0:
            print('Success\n', flush=True)
        else:
            print('Error\n', flush=True)

        return output
