import subprocess
import time
import sys
import os
import resource

param = "0"

start_cpu_time = os.times()[0]
start_ram_usage = os.popen('ps -p %d -o rss=' % os.getpid()).read().strip()

start_time = time.time()

# Replace "program" with the name of your compiled C++ program
result = subprocess.Popen(["./main.bin", param], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
output, error = result.communicate()

end_time = time.time()
execution_time = end_time - start_time

end_cpu_time = os.times()[0]
end_ram_usage = os.popen('ps -p %d -o rss=' % os.getpid()).read().strip()

if result.returncode == 0:
    output = output.decode("utf-8")
    print(output.encode(sys.stdout.encoding, errors='replace').decode(sys.stdout.encoding))
else:
    error = error.decode("utf-8")
    print(error.encode(sys.stdout.encoding, errors='replace').decode(sys.stdout.encoding))

print("Execution time: {:.2f} seconds".format(execution_time))
print("CPU usage: {:.2f}%".format((end_cpu_time - start_cpu_time) / execution_time * 100))
print("RAM usage: {:.2f} MB".format((int(end_ram_usage) - int(start_ram_usage)) / 1024))