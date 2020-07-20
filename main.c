#include<stdio.h>
#include<pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include<time.h>
#include<stdlib.h>
//#include<error.h>

#define SNAPLEN 65535
//#define PCAP_ERRBUF_SIZE 128
void my_packet_handler(u_char *args, const struct pcap_pkthdr *packet_header, const u_char *packet_body);
void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header);
int main(){
  pcap_t *handle;
  int dev,i; /* name of the device to use */
  char errbuf[PCAP_ERRBUF_SIZE];
  const u_char *packet;
  struct pcap_pkthdr packet_header;
  char filter_exp[] = "tcp[tcpflags] & (tcp-syn) != 0";
  struct bpf_program filter;
  pcap_if_t *lista, *temp = NULL;
  dev = pcap_findalldevs(&lista,errbuf);
  bpf_u_int32 subnet_mask, ip;

  if(dev == -1)
    printf("ERRORE\n");
  else
    temp = lista;

  while( temp != NULL ){
    printf("\tDevice %s\n", temp->name);
    temp = temp->next;
  }
  int packet_count_limit = 1;
  int timeout_limit = 10000;

  if (pcap_lookupnet(lista[0].name, &ip, &subnet_mask, errbuf) == -1) {
        printf("Could not get information for device: %s\n", lista[0].name);
        ip = 0;
        subnet_mask = 0;
  }

  handle = pcap_open_live(lista[0].name, BUFSIZ, packet_count_limit,  timeout_limit, errbuf);

  if(handle == NULL){
    perror("Check your connection\n");
    exit(EXIT_FAILURE);
  }

if (pcap_compile(handle, &filter, filter_exp, 0, ip) == -1) {
       printf("Bad filter - %s\n", pcap_geterr(handle));
       return 2;
   }

   if (pcap_setfilter(handle, &filter) == -1) {
        printf("Error setting filter - %s\n", pcap_geterr(handle));
        return 2;
  }
  packet = pcap_next(handle, &packet_header );
  if (packet == NULL) {
     printf("No packet found.\n");
     return 2;
 }

 if (pcap_setfilter(handle, &filter) == -1) {
        printf("Error setting filter - %s\n", pcap_geterr(handle));
        return 2;
    }
 /* Our function to output some info */
 print_packet_info(packet, packet_header);

pcap_loop(handle, 0, my_packet_handler, NULL);

  return 0;
}

void my_packet_handler(u_char *args, const struct pcap_pkthdr *packet_header, const u_char *packet_body){
    print_packet_info(packet_body, *packet_header);
    return;
}

void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header) {
    printf("Packet capture length: %d\n", packet_header.caplen);
    printf("Packet total length %d\n", packet_header.len);
}
