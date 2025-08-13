#include <stdio.h>
#include <openssl/evp.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

static void hashString(EVP_MD_CTX *ctx, char *text);
static void hashFile();
static void printHelp();

int main(int argc, char *argv[]){
	int opt;
	setlocale(LC_ALL, "");
	struct option longopt[] = {
		{"file",required_argument,NULL,'f'},
		{"algoritm", required_argument,NULL,'a'},
		{"string", required_argument,NULL,'s'},
		{"help",no_argument,NULL,'h'}
	};
	bool f_flag = false;
	bool s_flag = false;
	char *text = NULL,*alg = NULL, *path;
	while((opt = getopt_long(argc,argv,"hf:a:s:",longopt,NULL)) != -1){
		switch(opt)
		{
			case 'f':
				f_flag = true;
				path = optarg;
				break;
			case 'a':
				alg = optarg;
				break;
			case 's':
				s_flag = true;
				text = optarg;
				break;				
			case 'h':
				f_flag = 0; s_flag = 0;
				printHelp();
				exit(1);
				break;
			case '?':
				fprintf(stderr,"Unknown option: %s\n",argv[optind - 1]);
				printHelp();
				break;
		}
	}
	EVP_MD_CTX *mdctx;
	const EVP_MD *md;	
	md = EVP_get_digestbyname((alg == NULL) ? "sha256" : alg);
	if (md == NULL){
		printf("Unknown message digest %s\n",alg);
		exit(1);
	}
	mdctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(mdctx,md,NULL);
	if (!f_flag && s_flag){
		hashString(mdctx,text);
	} else if (f_flag && !s_flag){
		hashFile(mdctx,path);
	} else if (f_flag && s_flag) {
		fprintf(stderr, "Cannot specify both --file and --string options simultaneously.\n");
		printHelp();
		exit(1);
	} else {
		printHelp();
		exit(0);
	}
	EVP_MD_CTX_free(mdctx);
	return 0;
}

void hashString(EVP_MD_CTX *ctx, char *text){
	unsigned char md_value[EVP_MAX_MD_SIZE];
	EVP_DigestUpdate(ctx,text,strlen(text));
	unsigned int md_len;
	EVP_DigestFinal_ex(ctx,md_value,&md_len);
	printf("Digest is: ");
	for (int i = 0;i<md_len;i++){
		printf("%02x",md_value[i]);
	}
	printf("\n");
}

void hashFile(EVP_MD_CTX *ctx, char *path){
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len;
	FILE* f = fopen(path, "rb"); // бинарный режим
	if (!f) {
		perror("fopen");
		return;
	}
	char data[256];
	size_t bytes_read;
	while((bytes_read = fread(data, 1, sizeof(data), f)) > 0){
		EVP_DigestUpdate(ctx, data, bytes_read);
	}
	fclose(f);
	EVP_DigestFinal_ex(ctx, md_value, &md_len);
	printf("Digest is: ");
	for (unsigned int i = 0; i < md_len; i++){
		printf("%02x", md_value[i]);
	}
	printf("\n");
}


void printHelp(){
	printf(
		"Usage: hashit [OPTIONS]\n"
		"\n"
		"Compute the hash of a string or file using various algorithms.\n"
		"\n"
		"Options:\n"
		"  -a, --algorithm <ALGO>   Hash algorithm to use (md5, sha1, sha256, sha512, etc.).\n"
		"  -f, --file <PATH>         Path to file to hash.\n"
		"  -s, --string <TEXT>       String to hash.\n"
		"  -h, --help                Show this help message and exit.\n"
		"\n"
		"Examples:\n"
		"  hashit -a sha256 -s \"Hello World\"\n"
		"  hashit --algorithm md5 --file ./example.txt\n"
		"\n"
		"Notes:\n"
		"  - Either --file or --string must be specified.\n"
		"  - Supported algorithms depend on the OpenSSL library available on your system.\n"
	);
}
