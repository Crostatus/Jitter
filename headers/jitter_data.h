#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef struct record{
	long int t_arrive;
	long int t_delay;
	     int src_port;
			 int dst_port;
	struct record *next;
} record;

typedef struct tcp_stream{
	int jitter;
	int jitter_test;
	char *stream_name;
	unsigned int pkts_num;
	record *head;
	record *tail;
	struct tcp_stream *next_conflict;
} tcp_stream;

extern tcp_stream* streams_map;

extern void initialize_map(void);
extern int add_record(char *stream_name, long int packet_arrive_time, int src_port, int dst_port);
extern void print_map(void);
