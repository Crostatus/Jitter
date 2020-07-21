#include<stdio.h>
#include<pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include<time.h>
#include<stdlib.h>

typedef struct ip_address{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
}ip_address;

typedef struct ip_header{
    u_char  ver_ihl;        // Version (4 bits) + Internet header length (4 bits)
    u_char  tos;            // Type of service
    u_short tlen;           // Total length
    u_short identification; // Identification
    u_short flags_fo;       // Flags (3 bits) + Fragment offset (13 bits)
    u_char  ttl;            // Time to live
    u_char  proto;          // Protocol
    u_short crc;            // Header checksum
    ip_address  saddr;      // Source address
    ip_address  daddr;      // Destination address
    u_int   op_pad;         // Option + Padding
}ip_header;

#define SNAPLEN 65535
//#define PCAP_ERRBUF_SIZE 128
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

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
 //print_packet_info(packet, packet_header);

pcap_loop(handle, 0, packet_handler, NULL);

  return 0;
}

/* Callback function invoked by libpcap for every incoming packet */
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
    //struct tm ltime;
    //char timestr[16];
    ip_header *ih;
    //udp_header *uh;
    u_int ip_len;
    //u_short sport,dport;
    //time_t local_tv_sec;



    /* convert the timestamp to readable format */
    //local_tv_sec = header->ts.tv_sec;
    //localtime_s(&ltime, &local_tv_sec);
    //strftime( timestr, sizeof timestr, "%H:%M:%S", &ltime);

    /* print timestamp and length of the packet */
    //printf("%s.%.6d len:%d ", timestr, header->ts.tv_usec, header->len);

    /* retireve the position of the ip header */
    ih = (ip_header *) (pkt_data +
        14); //length of ethernet header

    /* retireve the position of the udp header */
    /*ip_len = (ih->ver_ihl & 0xf) * 4;
    uh = (udp_header *) ((u_char*)ih + ip_len);
    */
    /* convert from network byte order to host byte order */
    /*sport = ntohs( uh->sport );
    dport = ntohs( uh->dport );
    */
    /* print ip addresses and udp ports */
    printf("%d.%d.%d.%d -> %d.%d.%d.%d\n",
        ih->saddr.byte1,
        ih->saddr.byte2,
        ih->saddr.byte3,
        ih->saddr.byte4,
        ih->daddr.byte1,
        ih->daddr.byte2,
        ih->daddr.byte3,
        ih->daddr.byte4
        );
}

void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header) {
    printf("Packet capture length: %d\n", packet_header.caplen);
    printf("Packet total length %d\n", packet_header.len);
}
