#include <stdio.h>
#include "forensics.h"
#include <sensor.h>
#include <service_app.h>
#include "log_files.h"
#include "crypto_aead.h"


#define data_size 1

typedef struct {
	long long timestamp;
	int steps;
	int walk_steps;
	int run_steps;
	float distance;
	float calories;
	float last_speed;
	float last_stepping_freq;
	short state;
} pedometer_data;

#define pedometer_message_size  ((sizeof(pedometer_data)/sizeof(unsigned char))*data_size)

static int pedometer_counter = 0;

pedometer_data pedometer_arr[data_size];

static char steps_log[] = "/opt/usr/home/owner/data/steps.log";

sensor_listener_h pedometer_listener;
sensor_h pedometer_sensor;

void pedometer_callback(sensor_h sensorHanlder, sensor_event_s *event,
		void *user_data);

void set_cb_steps();
