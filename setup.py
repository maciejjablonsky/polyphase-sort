import os
import subprocess
import sys
import shutil
import argparse


parser = argparse.ArgumentParser()
parser.add_argument('--clean', help='Remove binary directory before building', dest='clean', action='store_true')
parser.add_argument('--open-code', help='Open repository in visual studio code', action='store_true')
parser.add_argument('--open-vs', help='Open solution in visual studio if on Windows', action='store_true')
parser.add_argument('--build-release', help='Build release target with tests', action='store_true')
parser.add_argument('--build-debug', help='Build debug target', action='store_true')
args = parser.parse_args()

build_path = "../build-polyphase-sort"
current_dir = os.getcwd()
if args.clean:
    shutil.rmtree(build_path, ignore_errors=True)
if not os.path.isdir(build_path):
    os.mkdir(build_path)
    os.chmod(build_path, 0o777)

if args.open_code:
    subprocess.run(['code', current_dir], stderr=subprocess.STDOUT, shell=True)

os.chdir(build_path)
cmake_build_cmd = ['cmake', '../polyphase-sort', '-DTESTING=1']
subprocess.check_call(cmake_build_cmd, stderr=subprocess.STDOUT, shell=True)
if args.open_vs:
    os.startfile('polyphase_sort.sln')
if args.build_release:
    subprocess.check_call(['cmake', '--build', '.', '--config', 'Release', '-j'], stderr=subprocess.STDOUT, shell=True)
if args.build_debug:
    subprocess.check_call(['cmake', '--build', '.', '--config', 'Debug', '-j'], stderr=subprocess.STDOUT, shell=True)
os.chdir(current_dir)