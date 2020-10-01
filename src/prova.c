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

int is_number(char *msg){
    int i = 0;
    while(i < strlen(msg)){
        if(!(isdigit(msg[i]))){
        	return 0;
        }
        i++;
    }
    return 1;
}

int is_help_flag(char *msg){
    if(strcmp("-h", msg) == 0)
        return 1;
    else
        return 0;        
}


int main(int argc, char **argv){
    int num_packets;			/* number of packets to capture */
    int i = 0;
    char param[40];
    
    switch(argc){
		case 1:
			num_packets = 0;
			break;
		case 2:
            strcpy(param, argv[1]);
            if(is_help_flag(param)){
                print_app_usage();
                num_packets = 0;
                sleep(2);
            }
            else if(is_number(param)){
                num_packets = atoi(param);
                if(num_packets < 0){
                    print_app_usage();
                    exit(EXIT_FAILURE);
                }
            }
            else{
                print_app_usage();
        		exit(EXIT_FAILURE);
            }
			break;
        case 3:{                      
            strcpy(param, argv[1]);
            if(is_help_flag(param)){
                print_app_usage();
                strcpy(param, argv[2]);
                if(!is_number(param)){
                    exit(EXIT_FAILURE); 
                }
                num_packets = atoi(param);
                if(num_packets < 0){
                    exit(EXIT_FAILURE);
                }
                sleep(2);
                break;
            }
            else if(is_number(param)){
                num_packets = atoi(param);
                print_app_usage();
                if(num_packets < 0){
                    exit(EXIT_FAILURE);
                }
                strcpy(param, argv[2]);
                if(!is_help_flag(param)){
                    exit(EXIT_FAILURE);      
                }
                sleep(2);
                break;
            }
            else {
                print_app_usage();
                exit(EXIT_FAILURE);
            }

        }
		default:
			fprintf(stderr, "error: unrecognized command-line arguments\n\n");
			print_app_usage();
			exit(EXIT_FAILURE);
	}

	printf("NUM: %d\n", num_packets);
    return 0;
}