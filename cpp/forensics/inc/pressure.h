#include <stdio.h>
#include "forensics.h"
#include <sensor.h>
#include <service_app.h>
#include "log_files.h"
#include "crypto_aead.h"


#define data_size 1

typedef struct {
	long long timestamp;
//	double value;
	float value;
} pressure_data;

#define pressure_message_size  ((sizeof(pressure_data)/sizeof(unsigned char))*data_size)

static int pressure_counter = 0;

pressure_data pressure_arr[data_size];
sensor_listener_h preassure_listener;
sensor_h preassure_sensor;

static char pressure_log[] = "/opt/usr/home/owner/data/pressure.log";

void set_cb_pressure();
void pressure_cb(sensor_h sensorHanlder, sensor_event_s *event,
		void *user_data);
