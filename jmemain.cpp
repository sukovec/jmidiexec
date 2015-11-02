#include "jmemain.h"


void sigh(int sig);
void close();
int  proccb(jack_nframes_t, void*);

jack_client_t * client;
jack_port_t * port;
bool run;

int main(int argc, char ** argv) {
	signal(SIGINT, sigh);
	JackStatus retst;

	printf("Starting jack... ");
	client = jack_client_open("jme", JackNullOption, &retst);
	if (retst != JackServerStarted && retst != 0) {
		printf("FAIL\n");
		return 1;
	}
	printf("OK\n");


	printf("Creating port...");
	port = jack_port_register(client, "JMEReceive", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
	if (port == 0) {
		printf("FAIL\n");
		close();
		return 2;
	}
	printf("OK\n");

	jack_set_process_callback(client, proccb, 0);

	jack_activate(client);


	run = true;
	while(run) {
		sleep(1);
	}

	return 0;
}

void close() {
	if (port != 0) {
		printf("Unregister port...\n");
		jack_port_unregister(client, port);
	}

	printf("Closing jack\n");
	jack_client_close(client);
}

void sigh(int sig) {
	close();
	run = false;
}

int proccb(jack_nframes_t nframes, void* arg) {
//	printf("proccb: %d \n", nframes);

	jack_default_audio_sample_t *in;
	char * buf = (char*)jack_port_get_buffer(port, nframes);

	jack_nframes_t num = jack_midi_get_event_count(buf);
	if (num != 0) {
		printf("Count: %d\n", num);

		for (int i = 0; i < num; i++) {
			jack_midi_event_t evt;

			int ret = jack_midi_event_get(&evt, buf, i);

			if (ret != 0) { printf("Get event error\n"); continue; }

			printf("Event time: %d, size: %d\n", evt.time, evt.size);
		}
	}


/*	for (int i = 0; i < nframes; i++) {	printf("%x ", (char)buf[i]); }
	printf("\n");*/
	
/*
	char buffer[1024];
	jack_nframes_t nf = jack_midi_get_event_count(buffer);

	printf("jmegec: %d, %d", nf, ((int*)buffer)0);*/

	return 0;
}
