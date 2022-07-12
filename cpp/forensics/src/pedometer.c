#include "pedometer.h"

void pedometer_callback(sensor_h sensorHanlder, sensor_event_s *event,
		void *user_data) {

	sensor_type_e type;
	sensor_get_type(sensorHanlder, &type);

	if (type == SENSOR_HUMAN_PEDOMETER) {
		int steps = event->values[0];
		int walk_steps = event->values[1];
		int run_steps = event->values[2];
		float distance = event->values[3];
		float calories = event->values[4];
		float last_speed = event->values[5];
		float last_stepping_freq = event->values[6];
		sensor_pedometer_state_e last_state = event->values[7];

		int size = 0;
		char buffer[256];
		short state = 0;

		switch (last_state) {
		case SENSOR_PEDOMETER_STATE_UNKNOWN: {
			state = 0;
//			size = sprintf(buffer, "%lld;%d;%d;%d;%f;%f;%f;%f;%s\n",
//					get_unix_timestamp(), steps, walk_steps, run_steps, distance,
//					calories, last_speed, last_stepping_freq, "UNKNOWN");
//			write_to_logfile_line(steps_log, buffer, size);
			break;
		}
		case SENSOR_PEDOMETER_STATE_STOP: {
			state = 1;
//			size = sprintf(buffer, "%lld;%d;%d;%d;%f;%f;%f;%f;%s\n",
//					get_unix_timestamp(), steps, walk_steps, run_steps, distance,
//					calories, last_speed, last_stepping_freq, "STOP");
//			write_to_logfile_line(steps_log, buffer, size);
			break;
		}
		case SENSOR_PEDOMETER_STATE_WALK: {
			state = 2;
//			size = sprintf(buffer, "%lld;%d;%d;%d;%f;%f;%f;%f;%s\n",
//					get_unix_timestamp(), steps, walk_steps, run_steps, distance,
//					calories, last_speed, last_stepping_freq, "WALK");
//			write_to_logfile_line(steps_log, buffer, size);
			break;
		}
		case SENSOR_PEDOMETER_STATE_RUN: {
			state = 3;
//			size = sprintf(buffer, "%lld;%d;%d;%d;%f;%f;%f;%f;%s\n",
//					get_unix_timestamp(), steps, walk_steps, run_steps, distance,
//					calories, last_speed, last_stepping_freq, "RUN");
//			write_to_logfile_line(steps_log, buffer, size);
			break;
		}
		}

		if(pedometer_counter == data_size){
			pedometer_counter = 0;

			// Buffer that contains associated data to auth along with the packet but does not need to be enrypted
			unsigned char ad[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2,
					0xa6 };
			// Length of associated data in bytes
			unsigned long long adlen = 8;

			// Secret nonce - not used.
			unsigned char nsec = 0;
			// Points to the public nonce for the packet with must be 16 bytes in len.
			unsigned char npub[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae,
					0xd2, 0xa6, 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2,
					0xa6 };

			// Points to the 16 bytes od the key to use to encrypt the packet.
			unsigned char k[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2,
					0xa6, 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6 };

			unsigned char m[pedometer_message_size];
			unsigned long long clen = pedometer_message_size + 16;
			unsigned char c[pedometer_message_size+16];
			memcpy(m, pedometer_arr, sizeof(pedometer_arr));
			unsigned long long mlen = pedometer_message_size;
			int err = crypto_aead_encrypt(c, &clen, m, mlen, ad, adlen, nsec, npub, k);
			char file_path[256];
			unsigned long long name = get_unix_timestamp();
			int tmp =  sprintf(file_path, "/opt/usr/home/owner/data/%lld.enc", name);
			write_encrypted_data(c, file_path, pedometer_message_size+16);

			char upload_path[256];
			tmp = sprintf(upload_path, "ftp://ftp.cytr.us/pedometer/%lld.enc", name);

			upload_file_ftp(file_path, upload_path);
			remove(file_path);

		} else {
			pedometer_arr[pedometer_counter].timestamp = get_unix_timestamp();
			pedometer_arr[pedometer_counter].steps = steps;
			pedometer_arr[pedometer_counter].walk_steps = walk_steps;
			pedometer_arr[pedometer_counter].run_steps = run_steps;
			pedometer_arr[pedometer_counter].distance = distance;
			pedometer_arr[pedometer_counter].calories = calories;
			pedometer_arr[pedometer_counter].last_speed = last_speed;
			pedometer_arr[pedometer_counter].last_stepping_freq = last_stepping_freq;
			pedometer_arr[pedometer_counter].state = state;

			pedometer_counter = pedometer_counter + 1;
		}

	}
}

void set_cb_steps() {
	char head[256];
	int size = 0;
	size =
			sprintf(head, "%s\n",
					"Timestamp;Steps;Walk_steps;Run_steps;Distance;Calories;Last_speed;Last_stepping_freq;State");
	create_log_file(steps_log, head, size);

	int ret;
	ret = sensor_get_default_sensor(SENSOR_HUMAN_PEDOMETER, &pedometer_sensor);
	ret = sensor_create_listener(pedometer_sensor, &pedometer_listener);
	ret = sensor_listener_set_events_cb(pedometer_listener, pedometer_callback, NULL);
	ret = sensor_listener_set_interval(pedometer_listener, 12000);
	ret = sensor_listener_set_option(pedometer_listener, SENSOR_OPTION_ALWAYS_ON);
	ret = sensor_listener_start(pedometer_listener);
}
