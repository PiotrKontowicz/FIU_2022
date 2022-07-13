#include "hr.h"

void hrm_callback(sensor_h sensorHanlder, sensor_event_s *event,
		void *user_data) {
	sensor_type_e type;
	sensor_get_type(sensorHanlder, &type);
	if (type == SENSOR_HRM) {
		int accuracy = event->accuracy;
		float hrm = event->values[0];
		char buffer[256];
		int size = 0;
		unsigned long long timestamp = get_unix_timestamp();
		size = sprintf(buffer, "%lld;%d;%f\n", timestamp, accuracy, hrm);
		write_to_logfile_line(hr_log, buffer, size);
	}
}

void set_cb_hrm() {
	char head[256];
	int size = 0;
	size = sprintf(head, "%s;%s;%s\n", "Timestamp", "Accuracy", "Value");
	create_log_file(hr_log, head, size);

	int ret;
	ret = sensor_get_default_sensor(SENSOR_HRM, &hr_sensor);
	ret = sensor_create_listener(hr_sensor, &hr_listener);
	ret = sensor_listener_set_events_cb(hr_listener, hrm_callback, NULL);
	ret = sensor_listener_set_interval(hr_listener, 500);
	ret = sensor_listener_set_option(hr_listener, SENSOR_OPTION_ALWAYS_ON);
	ret = sensor_listener_start(hr_listener);
}
