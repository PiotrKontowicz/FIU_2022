#include <stdio.h>
#include "forensics.h"
#include <sensor.h>
#include <service_app.h>
#include "log_files.h"
#include <time.h>

sensor_listener_h hr_listener;
sensor_h hr_sensor;
static char hr_log[] = "/opt/usr/home/owner/data/hr.log";

void set_cb_hrm();
void hrm_callback(sensor_h sensorHanlder, sensor_event_s *event,
		void *user_data);

long long getTimeNow_ms();
