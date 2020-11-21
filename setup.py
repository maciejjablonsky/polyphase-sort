import os
import subprocess


build_path = "../build-polyphase-sort"
current_dir = os.getcwd()
if not os.path.isdir(build_path):
    os.mkdir(build_path)

os.chdir(build_path)
cmake_build_cmd = ['cmake', '../polyphase-sort', '-DTESTING=1']
subprocess.check_call(cmake_build_cmd, stderr=subprocess.STDOUT, shell=True)
os.startfile('polyphase_sort.sln')
subprocess.check_call(['cmake', '--build', '.', '--config', 'Release', '-j'], stderr=subprocess.STDOUT, shell=True)
os.chdir(current_dir)