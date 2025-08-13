#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>

#define SWAP "swap"
#define ANON "anon"
#define CACHED "cached"
#define SHARED "shared" 

void getMemoryByProcess(int pid);
void getMemorySummary();
void getMemoryByType();
void getMemoryByTypeAndProcess(char* type, int pid);
void printLineIfMatch(FILE *f, char* key[], int key_size);
void printHelp();

int main(int argc, char *argv[]){
	int opt;
	int type_flag = 0;
	int pid_flag = 0;
	int summary_flag = 0;
	char* type = NULL;
	int pid = 0;
	char *endptr;
	struct option longopt[] =  {
		{"summary",no_argument,NULL,'s'},
		{"type",required_argument,NULL,'t'},
		{"help",no_argument,NULL,'h'},
		{"pid",required_argument,NULL,'p'},
	};
	while((opt = getopt_long(argc,argv,"ht:sp:",longopt,NULL)) != -1){
		switch(opt)
		{
		case 't':
			type_flag = 1;
			type = optarg;
			break;
		case 's':
			summary_flag = 1;
			break;
		case 'p':
			pid = strtol(optarg, &endptr, 10);
			pid_flag = 1;	
			break;
		case 'h':
			type_flag = 0; pid_flag = 0; summary_flag = 0;
			break;
		case '?':
			fprintf(stderr, "Unknown option: %s\n", argv[optind - 1]);
			printHelp();
			return 1;
		}
	}
	if(type_flag == 1 && pid_flag == 1){
		if(*endptr != '\0'){
			fprintf(stderr, "Invalid PID: %s\n", optarg);
   			return 1;	
		} else {
			getMemoryByTypeAndProcess(type,pid);
		}
	} else if(type_flag == 1) {
		if(strcmp(type,CACHED) == 0){
			getMemoryByType();
		} else {
    		fprintf(stderr, "Option --type must be used with --pid for type: %s\n", type);
    		return 1;
		}
	} else if(pid_flag == 1){
		if(*endptr !='\0'){
			fprintf(stderr,"Invalid PID: %s\n",optarg);
		} else {
			getMemoryByProcess(pid);
		} 

	} else if (summary_flag == 1){
		getMemorySummary();
	} else {
		printHelp();
	}
	return 0;
}

void getMemoryByProcess(int pid){
    char path[128];
    sprintf(path,"/proc/%d/status",pid); 
	FILE* f = fopen(path,"r");
	if (f == NULL){
		perror("Invalid pid");
		return;
	}
	char* key[] = {"Name","VmRSS","VmHWM","Threads"};
	printLineIfMatch(f,key,4);	
	fclose(f);	
	
}

void getMemorySummary(){
	FILE* f = fopen("/proc/meminfo","r");
	if (f == NULL){
		perror("Unavaible memory info\n");
		return;
	}
	char* key[] = {"MemAvailable","Buffers","Cached"};
	printLineIfMatch(f,key,3);	
	fclose(f);
}

void getMemoryByType() {
	FILE *f = fopen("/proc/meminfo","r");
	if (f == NULL){
		perror("Unavaible memory info\n");
	}
	char* key[] = {"Cached","SwapCached"};
	printLineIfMatch(f,key,2);	
	fclose(f);
}

void getMemoryByTypeAndProcess(char *type, int pid){
	char path[128];
	FILE* f_meminfo = fopen("/proc/meminfo","r");
	sprintf(path,"/proc/%d/status",pid);
	FILE* f_status = fopen(path,"r");
	if (f_status == NULL || f_meminfo == NULL){
		if (f_meminfo) fclose(f_meminfo);
		if (f_status) fclose(f_status);
		perror("Unavaible");
		return;
	}
	if(strcmp(type,"swap") == 0){
		char* key[] = {"SwapTotal","SwapFree"};
		printLineIfMatch(f_meminfo,key,2);	
	}
	char* key_status[] = {"RssShmem",""};
	if(strcmp(type,SHARED) == 0){
		printLineIfMatch(f_status,key_status,1);
	} else if(strcmp(type,ANON) == 0){
		key_status[0] = "RssAnon"; key_status[1] = "AnonPages";
		printLineIfMatch(f_status,key_status,2);
	} else if (strcmp(type,SWAP) == 0) {
		key_status[0] = "Pid";key_status[1] = "VmSwap";
		printLineIfMatch(f_status,key_status,2);
	} else {
		printf("Unknown type\n");
	}	
	 fclose(f_meminfo);
	 fclose(f_status);
}

void printLineIfMatch(FILE *f, char* key[], int key_size){
	char data[256];
	for(int i = 0; i< key_size;i++){
		int len = strlen(key[i]);
		rewind(f);
		while(fgets(data,256,f)){
			if(strncmp(data,key[i],len) == 0){
				printf("%s",data);
				break;
			}
		}
		
	}
}

void printHelp() {
    printf("memstat — display memory usage information\n\n");
    printf("Usage:\n");
    printf("  memstat [OPTIONS]\n\n");

    printf("Options:\n");
    printf("  -p, --process <PID>     Show memory usage for a specific process by PID\n");
    printf("  -t, --type <TYPE>       Filter memory info by type. Supported types:\n");
    printf("                          cached, shared, anon, swap\n");
    printf("  -s, --summary           Show a summary of overall memory usage from /proc/meminfo\n");
    printf("  -h, --help              Display this help message\n\n");

    printf("Examples:\n");
    printf("  memstat -p 1234         Show memory usage of process with PID 1234\n");
    printf("  memstat -t cached       Show only cached memory statistics\n");
    printf("  memstat -s              Show memory usage summary\n");
}
