#include "acc.h"

void acc_cb(sensor_h sensor, sensor_event_s events[], int events_count,
		void *user_data) {
	sensor_type_e type;
	sensor_get_type(sensor, &type);
	int i = 0;
	if (type == SENSOR_ACCELEROMETER) {
		for (i = 0; i < events_count; i++) {
			unsigned long long timestamp = get_unix_timestamp();
			int accuracy = events[i].accuracy;
			float x = events[i].values[0];
			float y = events[i].values[1];
			float z = events[i].values[2];
			char buffer[256];
			int size = 0;
			size = sprintf(buffer, "%lld;%d;%f;%f;%f\n", timestamp, accuracy, x,
					y, z);
			write_to_logfile_line(acc_log, buffer, size);
		}
	}
}

void set_cb_acc() {
	char head[256];
	int size = 0;
	size = sprintf(head, "Timestamp;Accuracy;X;Y;Z\n");
	create_log_file(acc_log, head, size);

	int ret;
	ret = sensor_get_default_sensor(SENSOR_ACCELEROMETER, &acc_sensor);
	ret = sensor_create_listener(acc_sensor, &acc_listener);
	ret = sensor_listener_set_events_cb(acc_listener, acc_cb, NULL);
	ret = sensor_listener_set_interval(acc_listener, 150); // Set interval

	ret = sensor_listener_set_option(acc_listener, SENSOR_OPTION_ALWAYS_ON);
	ret = sensor_listener_start(acc_listener);
}
