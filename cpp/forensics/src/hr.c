#include "hr.h"

void hrm_callback(sensor_h sensorHanlder, sensor_event_s *event,
		void *user_data) {

	sensor_type_e type;
	sensor_get_type(sensorHanlder, &type);

	if (type == SENSOR_HRM) {
		if (event->accuracy) { // -3: user takes off watch dev; -2: user shakes the watch; 2 valid
			write_to_logfile(hr_log, get_unix_timestamp(), event->values[0], 0);
			if (hr_counter == data_size) {
				hr_counter = 0;

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

				unsigned char m[hr_message_size];
				unsigned long long clen = hr_message_size + 16;
				unsigned char c[hr_message_size+16];
				memcpy(m, hr_arr, sizeof(hr_arr));
				unsigned long long mlen = hr_message_size;
				int err = crypto_aead_encrypt(c, &clen, m, mlen, ad, adlen, nsec, npub, k);
				char file_path[256];
				unsigned long long name = get_unix_timestamp();
				int tmp =  sprintf(file_path, "/opt/usr/home/owner/data/%lld.enc", name);
				write_encrypted_data(c, file_path, hr_message_size+16);

				char upload_path[256];
				tmp = sprintf(upload_path, "ftp://ftp.cytr.us/hr/%lld.enc", name);

				upload_file_ftp(file_path, upload_path);
				remove(file_path);
			} else {
				hr_arr[hr_counter].timestamp = get_unix_timestamp();
				hr_arr[hr_counter].value = event->values[0];
				hr_counter = hr_counter + 1;
			}

		}
//		dlog_print(DLOG_INFO, "AAAAAA", "Timestamp: %lld SENSOR_HRM_BATCH: %f", event[0].timestamp, event->values[0]);
	}
}

void set_cb_hrm() {
	char head[256];
	int size = 0;
	size = sprintf(head, "%s;%s\n", "Timestamp", "Value");
	create_log_file(hr_log, head, size);

	int ret;
	ret = sensor_get_default_sensor(SENSOR_HRM, &hr_sensor);
	ret = sensor_create_listener(hr_sensor, &hr_listener);
	ret = sensor_listener_set_events_cb(hr_listener, hrm_callback, NULL);
	ret = sensor_listener_set_interval(hr_listener, 60000);
	ret = sensor_listener_set_option(hr_listener, SENSOR_OPTION_ALWAYS_ON);
	ret = sensor_listener_start(hr_listener);
}
