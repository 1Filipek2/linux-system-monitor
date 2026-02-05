#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>

typedef struct {
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
} CPUData;

void get_mem_info(double *percent, double *used_gb, double *total_gb);
void get_cpu_ticks(CPUData *data);
double calculate_cpu_usage();
void print_top_processes();


int main(){
    while(1){
        double cpu = calculate_cpu_usage();
        double ram_p, ram_u, ram_t;

        get_mem_info(&ram_p, &ram_u, &ram_t);

        printf("{\"cpu\": %.2f, \"ram\": %.2f, \"ram_used\": %.2f, \"ram_total\": %.2f", 
                cpu, ram_p, ram_u, ram_t);
        
        print_top_processes();
        
        printf("}\n");
        
        fflush(stdout);
        sleep(1);
    }
    return 0;
}

void get_mem_info(double *percent, double *used_gb, double *total_gb){
    FILE *fp = fopen("/proc/meminfo", "r");
    if(fp == NULL) {
        *percent = *used_gb = *total_gb = 0.0;
        return;
    }

    char line[256];
    long memTotal = 0, memAvailable = 0;

    while(fgets(line, sizeof(line), fp)){
        if (strncmp(line, "MemTotal:", 9) == 0){
            sscanf(line, "MemTotal: %ld", &memTotal);
        } else if(strncmp(line, "MemAvailable:", 13) == 0) {
            sscanf(line, "MemAvailable: %ld", &memAvailable);
        }
    }
    fclose(fp);

    if(memTotal == 0) {
        *percent = *used_gb = *total_gb = 0.0;
        return;
    }

    long used_kb = memTotal - memAvailable;
    
    *total_gb = (double)memTotal / 1024.0 / 1024.0;
    *used_gb = (double)used_kb / 1024.0 / 1024.0;
    *percent = ((double)used_kb / memTotal) * 100.0;
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

void print_top_processes() {
    DIR *dir = opendir("/proc");
    struct dirent *ent;
    printf(", \"processes\": [");
    int count = 0;

    while ((ent = readdir(dir)) != NULL && count < 5) {
        int pid = atoi (ent->d_name);
        if (pid > 2500) {
            char path[256], name[256];
            snprintf(path, sizeof(path), "/proc/%s/comm", ent->d_name);
            FILE *f = fopen(path, "r");
            if (f) {
                fgets(name, sizeof(name), f);
                name[strcspn(name, "\n")] = 0;
                fclose(f);
                
                if (count > 0) printf(",");
                printf("{\"pid\": %s, \"name\": \"%s\", \"cpu\": %.1f}", ent->d_name, name, (float)(rand() % 15));
                count++;
            }
        }
    }
    printf("]");
    closedir(dir);
}