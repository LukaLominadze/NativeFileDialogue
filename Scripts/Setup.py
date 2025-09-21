import platform
import subprocess
import sys
from ColoramaInit import *


def print_colored(config, msg):
    print(config + msg + Style.RESET_ALL)


def input_colored(config, msg):
    return input(config + msg + Style.RESET_ALL)


init()

print_colored(Style.BRIGHT + Fore.CYAN, f'Platform: {platform.system()} {platform.release()} {platform.version()}')

if platform.system() == 'Windows':
    out = subprocess.run(['premake/premake5', 'vs2022', '--file=../premake5.lua'], capture_output=True)
    print_colored(Fore.GREEN, out.stdout.decode())
elif platform.system() == 'Linux':
    out = subprocess.run(['premake/premake5', 'vs2022', '--file=../premake5.lua'], capture_output=True)
    print_colored(Fore.GREEN, out.stdout.decode())
else:
    print_colored(Fore.RED, "Unsupported platform!")

input_colored(Style.BRIGHT + Fore.WHITE, "Press any key to end setup...")