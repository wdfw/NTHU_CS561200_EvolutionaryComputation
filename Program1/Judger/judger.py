import subprocess
import numpy as np
import os
import re
import platform

test_num = 30 # Number of trials to run and average for each testcases

GREEN = '\033[92m'
RED = '\033[91m'
RESET = '\033[0m'

system_os = platform.system()
executable = None

def check_python_version(command):
    try:
        result = subprocess.run([command, "--version"], capture_output=True, text=True)
        version_output = result.stdout if result.stdout else result.stderr
        return bool(re.search(r"Python \d+\.\d+", version_output))
    except FileNotFoundError:
        return False

if system_os == "Windows":
    if os.path.exists(".\\main.exe"):
        executable = ".\\main.exe"
    elif os.path.exists(".\\main.py"):
        executable = "python" if check_python_version("python") else "python3"
    script_path = ".\\main.py" if os.path.exists(".\\main.py") else None
elif system_os == "Linux":
    if os.path.exists("./main"):
        executable = "./main"
    elif os.path.exists("./main.py"):
        executable = "python" if check_python_version("python") else "python3"
    script_path = "./main.py" if os.path.exists("./main.py") else None
elif system_os == "Darwin":  
    if os.path.exists("./main"):
        executable = "./main"
    elif os.path.exists("./main.py"):
        executable = "python" if check_python_version("python") else "python3"
    script_path = "./main.py" if os.path.exists("./main.py") else None

print(f"Executable: {executable}, Script path: {script_path}")

if executable is None:
    raise FileNotFoundError("Neither compiled executable nor Python script found.")

def run_test(executable, args, script_path=None):
    cmd = [executable] + args if script_path is None else [executable, script_path] + args
    try:
        process = subprocess.run(cmd, capture_output=True, text=True, check=True)
        float_strs = process.stdout.strip().split()
        floats = [float(f) for f in float_strs]
        return floats
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {' '.join(cmd)}")
        print(f"Return code: {e.returncode}")
        print(f"Output: {e.output}")
        return []

def test_student_code(test_input_file, test_output_file, executable):
    with open(test_input_file, 'r') as f:
        params = f.readline().strip().split()

    with open(test_output_file, 'r') as f:
        expected_mean = float(f.readline().strip())
        expected_std_dev = float(f.readline().strip())

    fitness_values = [run_test(executable, params, script_path) for _ in range(test_num)]

    calculated_mean = np.mean(fitness_values)

    lower_bound = expected_mean - 2 * expected_std_dev
    upper_bound = expected_mean + 2 * expected_std_dev
    if lower_bound <= calculated_mean <= upper_bound:
        result = f"{test_input_file:<10} {GREEN}Correct{RESET}"
    else:
        result = f"{test_input_file:<10} {RED}Wrong (Calculated Mean: {calculated_mean}, Expected Mean: {expected_mean}, Std Dev: {expected_std_dev}){RESET}"
    print(result)

for i in range(1, 11):
    test_input_file = f"testcase/{i:02d}.in"
    test_output_file = f"testcase/{i:02d}.out"
    test_student_code(test_input_file, test_output_file, executable)