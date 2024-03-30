#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

using namespace std;

// Function to get CPU usage
float get_cpu_usage() {
    string line;
    float cpu_usage = 0.0;

    ifstream file("/proc/stat");
    if (file.is_open()) {
        getline(file, line);
        file.close();

        // CPU data from the first line of /proc/stat
        istringstream iss(line);
        string cpu_label;
        int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
        iss >> cpu_label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

        // Calculating total CPU time
        int total_cpu_time = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;

        // Calculating idle time
        int idle_time = idle + iowait;

        // Calculating CPU usage percentage
        cpu_usage = 100.0 * (1.0 - (idle_time / (float)total_cpu_time));
    }

    return cpu_usage;
}

// get memory usage
void get_memory_usage(long& total_memory, long& free_memory) {
    string line;

    ifstream file("/proc/meminfo");
    if (file.is_open()) {
        while (getline(file, line)) {
            istringstream iss(line);
            string key;
            long value;
            iss >> key >> value;
            if (key == "MemTotal:") {
                total_memory = value;
            } else if (key == "MemFree:") {
                free_memory = value;
                break;
            }
        }
        file.close();
    }
}

int main() {
    long total_memory, free_memory;
    float cpu_usage;

    while (true) {
        cpu_usage = get_cpu_usage();
        get_memory_usage(total_memory, free_memory);

        cout << "CPU Usage: " << cpu_usage << "% | ";
        cout << "Memory Usage: " << (total_memory - free_memory) / 1024 << " KB / " << total_memory / 1024 << " KB" << endl;

        // Sleep for 1 second before updating again
        sleep(1);
    }

    return 0;
}