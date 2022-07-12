#include <service_app.h>
#include <sensor.h>
#include <forensics.h>
#include <stdio.h>
#include "log_files.h"
#include "crypto_aead.h"
#define data_size 1

typedef struct {
	long long timestamp;
	int accuracy;
	float x;
	float y;
	float z;
} acc_data;

#define acc_message_size  ((sizeof(acc_data)/sizeof(unsigned char))*data_size)

static int acc_counter = 0;

acc_data acc_arr[data_size];

static char acc_log[] = "/opt/usr/home/owner/data/acc.log";
static char acc_log_enc[] = "/opt/usr/home/owner/data/acc/";

sensor_h acc_sensor;
sensor_listener_h acc_listener;

void acc_cb(sensor_h sensor, sensor_event_s events[], int events_count,
		void *user_data);

void set_cb_acc();
