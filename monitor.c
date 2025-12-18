#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void get_mem_usage();

int main(){
    while(1){
        get_mem_usage();
        sleep(2);
    }
    return 0;
}

void get_mem_usage(){
    FILE *fp = fopen("/proc/meminfo", "r");
    if(fp == NULL){
        perror("Chyba pri otvarani /proc/meminfo");
        return;
    }

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

    long used = memTotal - memAvailable;
    double percent = (double)used / memTotal * 100;

    printf("RAM USAGE: %ld / %ld KB (%.2f%%)\n", used, memTotal, percent);

}