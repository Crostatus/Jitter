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

int  add_to_stream(tcp_stream *stream, char *stream_name, long int packet_arrive_time, int src_port, int dst_port);
int  add_new_stream(tcp_stream *stream, char *stream_name, long int packet_arrive_time, int src_port, int dst_port);
void add_packet_record(record **head, record **tail, long int packet_arrive_time, int src_port, int dst_port);
void update_jitter(tcp_stream *stream);

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

void update_jitter(tcp_stream *stream){
	int i;
	int tmp_sum = 0;
	record *t_head = stream->head;

	for(i = 0; i < stream->pkts_num; i++){
				tmp_sum += t_head->t_delay;
				t_head = t_head->next;
	}
	stream->jitter = tmp_sum / (stream->pkts_num - 1);
}

int add_record(char *stream_name, long int packet_arrive_time, int src_port, int dst_port){
	if(stream_name == NULL){
		fprintf(stderr, "jitter_data.add_record: NULL stream name.\n");
		return -1;
	}
	int map_index = HASH(stream_name);
	tcp_stream *tmp = &streams_map[map_index];
	if(tmp->stream_name == NULL){
		//Empty stream slot
		return add_new_stream(tmp, stream_name, packet_arrive_time, src_port, dst_port);
	}
	else {
		//May be conflicts
		return add_to_stream(tmp, stream_name, packet_arrive_time, src_port, dst_port);
	}
}

int add_to_stream(tcp_stream *stream, char *stream_name, long int packet_arrive_time, int src_port, int dst_port){
	while(strcmp(stream->stream_name, stream_name) != 0 && stream->next_conflict != NULL){
		stream = stream->next_conflict;
	}
	if(strcmp(stream->stream_name, stream_name) == 0){
		stream->pkts_num = stream->pkts_num + 1;
		add_packet_record(&stream->head, &stream->tail, packet_arrive_time, src_port, dst_port);
		if(stream->pkts_num >= 3)
			update_jitter(stream);

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
			add_packet_record(&new_stream->head, &new_stream->tail, packet_arrive_time, src_port, dst_port);
			return 1;
	}
	else
		return -1;
}

int add_new_stream(tcp_stream *stream, char *stream_name, long int packet_arrive_time, int src_port, int dst_port){
	stream->stream_name = stream_name;
	stream->pkts_num = stream->pkts_num + 1;
	add_packet_record(&stream->head, &stream->tail, packet_arrive_time, src_port, dst_port);
	return 1;
}

void add_packet_record(record **head, record **tail, long int packet_arrive_time, int src_port, int dst_port){
	if(*head == NULL){
		*head = (record *) malloc(sizeof(record));
		(*head)->next = NULL;
		tail = head;
		(*head)->t_arrive = packet_arrive_time;
		(*head)->t_delay = 0;
		(*head)->src_port = src_port;
		(*head)->dst_port = dst_port;
	}
	else{
		record *new_el = (record *) malloc(sizeof(record));
		new_el->t_arrive = packet_arrive_time;
		new_el->t_delay =  packet_arrive_time - (*head)->t_arrive;
		new_el->src_port = src_port;
		new_el->src_port = dst_port;
		new_el->next = *head;
		*head = new_el;
	}
	return;
}

void print_record(record *tmp_r, int pkts_num){
	if(tmp_r == NULL)
		return;

	print_record(tmp_r->next, pkts_num - 1);
	if(pkts_num == 1)
		printf("                    [Packet %d] Arrive time: %ld. Ports: %d -> %d;\n", pkts_num, tmp_r->t_arrive, tmp_r->src_port, tmp_r->dst_port);
	else
		printf("                    [Packet %d] Arrive time: %ld. Delay from previous packet: %ld. Ports: %d -> %d;\n", pkts_num, tmp_r->t_arrive, tmp_r->t_delay, tmp_r->src_port, tmp_r->dst_port);
}


void print_stream(tcp_stream *str){
	if(str->stream_name == NULL)
		return;
	printf("\n   Comunication: %s\n", str->stream_name);
	if(str->pkts_num >= 3)
		printf("         Jitter: %d ms\n", str->jitter);
	printf("Packets sniffed: %d [\n", str->pkts_num);
	record *tmp_r = str->head;
	print_record(tmp_r, str->pkts_num);
	printf("                   ]\n");
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
