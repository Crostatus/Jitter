#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 30
#define A 999
#define B 9758
#define P 999149
#define HASH(s) ((atoi(s) * A + B) % P) % SIZE

typedef struct record{
	long int time;
	struct record *next;
} record;

typedef struct tcp_stream{
	unsigned int jitter;
	char *stream_name;
	unsigned int pkts_num;
	record *head;
	record *tail;
	struct tcp_stream *next_conflict;
} tcp_stream;

int add_to_stream(tcp_stream *stream, char *stream_name, long int packet_arrive_time);
int add_new_stream(tcp_stream *stream, char *stream_name, long int packet_arrive_time);
void add_packet_record(record *head, record *tail, unsigned int pkts_num, long int packet_arrive_time);

tcp_stream streams_map[SIZE];

void initialize_map() {
	int i;
	tcp_stream *tmp;
	for(i = 0; i < SIZE; i++){
		tmp = &streams_map[i];
		tmp->jitter = 0;
		tmp->stream_name = NULL;
		tmp->pkts_num = 0;
		tmp->head = NULL;
		tmp->tail = NULL;
		tmp->next_conflict = NULL;
	}

return;
}

int add_record(char *stream_name, long int packet_arrive_time){
	if(stream_name == NULL){
		fprintf(stderr, "jitter_data.add_record: NULL stream name.\n");
		return -1;
	}
	int map_index = HASH(stream_name);
	tcp_stream *tmp = &streams_map[map_index];
	if(tmp->stream_name == NULL){
		//Empty stream slot
		return add_new_stream(tmp, stream_name, packet_arrive_time);
	}
	else {
		//May be conflicts
		return add_to_stream(tmp, stream_name, packet_arrive_time);
	}
}

int add_to_stream(tcp_stream *stream, char *stream_name, long int packet_arrive_time){
	while(strcmp(stream->name, stream_name) || stream->next_conflict == NULL){
		stream = stream->next-conflict;
	}





}

int add_new_stream(tcp_stream *stream, char *stream_name, long int packet_arrive_time){
	stream->stream_name = stream_name;
	stream->pkts_num = stream->pkts_num + 1;
	add_packet_record(stream->head, stream->tail,stream->pkts_num, packet_arrive_time);
	return 1;
}

void add_packet_record(record *head, record *tail, unsigned int pkts_num, long int packet_arrive_time){
	if(head == NULL){
		head = malloc(sizeof(record));
		head->next = NULL;
		tail = head;
		head->time = 0;
	}
	else{
		record *new_el = malloc(sizeof(record));
		new_el->time = packet_arrive_time - tail->time;
		tail->next = new_el;
		tail = tail->next;
	}

}
