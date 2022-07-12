#include <stdio.h>
#include "forensics.h"
#include <sensor.h>
#include <service_app.h>
#include "log_files.h"
#include <time.h>
#include "crypto_aead.h"

#define data_size 10

//const unsigned int hr_arr_size =  data_size;

typedef struct {
	long long timestamp;
	int value;
} hr_data;

#define hr_message_size  ((sizeof(hr_data)/sizeof(unsigned char))*data_size)

static unsigned int hr_counter = 0;

hr_data hr_arr[data_size];

sensor_listener_h hr_listener;
sensor_h hr_sensor;
static char hr_log[] = "/opt/usr/home/owner/data/hr.log";
static char hr_log_enc[] = "/opt/usr/home/owner/data/";

void set_cb_hrm();
void hrm_callback(sensor_h sensorHanlder, sensor_event_s *event,
		void *user_data);

long long getTimeNow_ms();
