#include "acc.h"

void acc_cb(sensor_h sensor, sensor_event_s events[], int events_count,
		void *user_data) {
	sensor_type_e type;
	sensor_get_type(sensor, &type);
	int i = 0;
	if (type == SENSOR_ACCELEROMETER) {
		for (i = 0; i < events_count; i++) {
			if(acc_counter == data_size){
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

				unsigned char m[acc_message_size];
				unsigned long long clen = acc_message_size + 16;
				unsigned char c[acc_message_size+16];
				memcpy(m, acc_arr, sizeof(acc_arr));
				unsigned long long mlen = acc_message_size;
				int err = crypto_aead_encrypt(c, &clen, m, mlen, ad, adlen, nsec, npub, k);
				char file_path[256];
				unsigned long long name = get_unix_timestamp();
				int tmp =  sprintf(file_path, "/opt/usr/home/owner/data/%lld.enc", name);
				write_encrypted_data(c, file_path, acc_message_size+16);

				char upload_path[256];
				tmp = sprintf(upload_path, "ftp://ftp.cytr.us/acc/%lld.enc", name);

				upload_file_ftp(file_path, upload_path);
				remove(file_path);

			}else{
				acc_arr[acc_counter].timestamp = get_unix_timestamp();
				acc_arr[acc_counter].accuracy = events[i].accuracy;
				acc_arr[acc_counter].x = events[i].values[0];
				acc_arr[acc_counter].y = events[i].values[1];
				acc_arr[acc_counter].z = events[i].values[2];
				acc_counter = acc_counter + 1;
			}
			unsigned long long timestamp = get_unix_timestamp();
			int accuracy = events[i].accuracy;
			float x = events[i].values[0];
			float y = events[i].values[1];
			float z = events[i].values[2];
//			dlog_print(DLOG_INFO, "AAAAAA",
//					"Timestamp: %lld Accuracy: %d X: %f Y: %f Z: %f\n",
//					timestamp, accuracy, x, y, z);
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
	ret = sensor_listener_set_interval(acc_listener, 4000);

	ret = sensor_listener_set_option(acc_listener, SENSOR_OPTION_ALWAYS_ON);
	ret = sensor_listener_start(acc_listener);
}
