import subprocess
import sys

while True :
    shell_input = input("minishell$ ");
    if (shell_input == '^D' or shell_input == "logout") :
        break;
    inputs = shell_input.split();
    subprocess.run(inputs, shell=False)
