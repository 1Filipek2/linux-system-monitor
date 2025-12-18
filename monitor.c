#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
} CPUData;

double get_mem_usage();
void get_cpu_ticks(CPUData *data);
double calculate_cpu_usage();


int main(){
    while(1){
        double cpu = calculate_cpu_usage();
        double ram = get_mem_usage();

        printf("{\"cpu\": %.2f, \"ram\": %.2f}\n", cpu, ram);
        
        fflush(stdout);
        sleep(1);
    }
    return 0;
}

double get_mem_usage(){
    FILE *fp = fopen("/proc/meminfo", "r");
    if(fp == NULL) return 0.0;

    char line[256];
    long memTotal = 0, memFree = 0, memAvailable = 0;

    while(fgets(line, sizeof(line), fp)){
        if (strncmp(line, "MemTotal:", 9) == 0){
            sscanf(line, "MemTotal: %ld", &memTotal);
        } else if(strncmp(line, "MemAvailable:", 13) == 0) {
            sscanf(line, "MemAvailable: %ld", &memAvailable);
        }
    }
    fclose(fp);

    if(memTotal == 0) return 0.0;

    long used = memTotal - memAvailable;
    return ((double)used / memTotal) * 100;
}

void get_cpu_ticks(CPUData *data){
    FILE *fp = fopen("/proc/stat", "r");
    if(fp == NULL) return;

    char line[256];
    fgets(line, sizeof(line), fp);

    sscanf(line, "cpu %llu %llu %llu %llu %llu %llu %llu %llu", &data->user, &data->nice, &data->system, &data->idle, &data->iowait, &data->irq, &data->softirq, &data->steal);
    
    fclose(fp);
}

double calculate_cpu_usage() {
    CPUData t1, t2;
    get_cpu_ticks(&t1);
    usleep(100000);
    get_cpu_ticks(&t2);

    unsigned long long total1 = t1.user + t1.nice + t1.system + t1.idle + t1.iowait + t1.irq + t1.softirq + t1.steal;
    unsigned long long total2 = t2.user + t2.nice + t2.system + t2.idle + t2.iowait + t2.irq + t2.softirq + t2.steal;

    unsigned long long difference_total = total2 - total1;
    unsigned long long difference_idle = t2.idle - t1.idle;

    if(difference_total == 0) return 0.0;

    return (1.0 -((double)difference_idle / difference_total)) * 100.0;
}