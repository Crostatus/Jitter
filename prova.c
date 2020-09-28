#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

void print_app_usage(void) {
	printf("Usage: %s [packet_number]\n", "10");
	printf("\n");
	printf("Argument:\n");
	printf("(optional)   packet_number       Capture the next <packet_number> packets.\n");
	printf("\nIf no packet_number is provided, this process will start sniffing non-stop.\n");
	printf("In both cases until it can be correctly stopped sending a SIGINT interruption to this process.\n");
    return;
}

int main(int argc, char **argv){
    int num_packets;			/* number of packets to capture */
    int i = 0;
    char string[40];

    switch(argc){
		case 1:
			num_packets = 0;
			break;
		case 2:
            strcpy(string, argv[1]);
            if(strcmp("-h", string) == 0){
                print_app_usage();
                sleep(2);
                num_packets = 0;
                break;
            }
            while(i < strlen(string) ){
                if(!(isdigit(string[i]))){
                    fprintf(stderr, "error: unrecognized command-line arguments\n\n");
        			print_app_usage();
        			exit(EXIT_FAILURE);
                }
                i++;
            }
			num_packets = atoi(argv[1]);
			break;
        case 3:{
            strcpy(string, argv[1]);
            if(strcmp("-h", string) == 0){
                print_app_usage();
                sleep(2);
                strcpy(string, argv[2]);
                while(i < strlen(string) ){
                    if(!(isdigit(string[i]))){
                        fprintf(stderr, "error: unrecognized command-line arguments\n\n");
                        print_app_usage();
                        exit(EXIT_FAILURE);
                    }
                    i++;
                }
                num_packets = atoi(argv[2]);
                break;
            }
            while(i < strlen(string) ){
                if(!(isdigit(string[i]))){
                    fprintf(stderr, "error: unrecognized command-line arguments\n\n");
                    print_app_usage();
                    exit(EXIT_FAILURE);
                }
                i++;
            }
            num_packets = atoi(argv[1]);
            strcpy(string, argv[2]);
            if(strcmp("-h", string) == 0){
                print_app_usage();
                sleep(2);
                break;
            }else{
                fprintf(stderr, "error: unrecognized command-line arguments\n\n");
                print_app_usage();
                exit(EXIT_FAILURE);
            }
            break;
        }
		default:
			fprintf(stderr, "error: unrecognized command-line arguments\n\n");
			print_app_usage();
			exit(EXIT_FAILURE);
	}
	printf("NUM: %d\n", num_packets);
    return 0;
}
