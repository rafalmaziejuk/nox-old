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
