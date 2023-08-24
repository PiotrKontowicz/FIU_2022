#include <service_app.h>
#include <sensor.h>
#include <forensics.h>
#include <stdio.h>
#include "log_files.h"

static char acc_log[] = "/opt/usr/home/owner/data/acc.log";

sensor_h acc_sensor;
sensor_listener_h acc_listener;

void acc_cb(sensor_h sensor, sensor_event_s events[], int events_count,
		void *user_data);

void set_cb_acc();
