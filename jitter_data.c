#include "jitter_data.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


#define SIZE 30
#define A 999
#define B 9758
#define P 999149
#define HASH(s) ((atoi(s) * A + B) % P) % SIZE

int add_to_stream(tcp_stream *stream, char *stream_name, long int packet_arrive_time);
int add_new_stream(tcp_stream *stream, char *stream_name, long int packet_arrive_time);
void add_packet_record(record *head, record *tail, long int packet_arrive_time);

tcp_stream* streams_map;

void initialize_map() {
	streams_map = (tcp_stream *) malloc(sizeof(tcp_stream) * SIZE);
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
	while(strcmp(stream->stream_name, stream_name) != 0 && stream->next_conflict != NULL){
		stream = stream->next_conflict;
	}
	if(strcmp(stream->stream_name, stream_name) == 0){
		add_packet_record(stream->head, stream->tail, packet_arrive_time);
		return 1;
	} else if(stream->next_conflict == NULL){
			//nuovo stream da monitorare!
			stream->next_conflict = (tcp_stream *) malloc(sizeof(tcp_stream));
			tcp_stream *new_stream = stream->next_conflict;
			new_stream->stream_name = stream_name;
			new_stream->jitter = 0;
			new_stream->pkts_num = 1;
			new_stream->head = NULL;
			new_stream->tail = NULL;
			new_stream->next_conflict = NULL;
			add_packet_record(new_stream->head, new_stream->tail, packet_arrive_time);
			return 1;
	}
	else
		return -1;
}

int add_new_stream(tcp_stream *stream, char *stream_name, long int packet_arrive_time){
	stream->stream_name = stream_name;
	stream->pkts_num = stream->pkts_num + 1;
	add_packet_record(stream->head, stream->tail, packet_arrive_time);
	return 1;
}

void add_packet_record(record *head, record *tail, long int packet_arrive_time){
	if(head == NULL){
		head = malloc(sizeof(record));
		head->next = NULL;
		tail = head;
		head->time = 0;
	}
	else{
		record *new_el = malloc(sizeof(record));
		new_el->time = packet_arrive_time - head->time;
		new_el->next = head;
		head = new_el;
	}
	return;
}
/*
typedef struct record{
	long int time;
	struct record *next;
} record;

typedef struct tcp_stream{
	float jitter;
	char *stream_name;
	unsigned int pkts_num;
	record *head;
	record *tail;
	struct tcp_stream *next_conflict;
} tcp_stream;*/

void print_stream(tcp_stream *str){
	if(str->stream_name == NULL)
		return;
	printf("\n\n   Comunication: %s\n", str->stream_name);
	printf("         Jitter: %.3f\n", str->jitter);
	printf("Packets sniffed: %d", str->pkts_num);
	record *tmp_r = str->head;
	int packet_num = 1;
	while(tmp_r != NULL){
		printf("Packet %d :arrived at time %ld\n", packet_num, tmp_r->time);
		packet_num++;
		tmp_r = tmp_r->next;
	}
	return;
}

void print_map(void){
	printf("Recorded streams:\n");
	int i;
	tcp_stream *tmp;
	for(i = 0; i < SIZE; i++){
		tmp = &streams_map[i];
		while(tmp != NULL){
			print_stream(tmp);
			tmp = tmp->next_conflict;
		}
	}
	return;
}
