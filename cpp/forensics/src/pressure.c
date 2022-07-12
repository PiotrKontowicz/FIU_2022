#include "pressure.h"

void pressure_cb(sensor_h sensorHanlder, sensor_event_s *event,
		void *user_data) {

	sensor_type_e type;
	sensor_get_type(sensorHanlder, &type);

	if (type == SENSOR_PRESSURE) {
		//dlog_print(DLOG_INFO, "AAAAAA", "Timestamp: %lld Pressure: %f", event[0].timestamp, event->values[0]);
		//write_to_logfile(pressure_log, get_unix_timestamp(), event->values[0], 1);
		if (pressure_counter == data_size) {
			pressure_counter = 0;

			unsigned char ad[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6};
			// Length of associated data in bytes
			unsigned long long adlen = 8;
			// Secret nonce - not used.
			unsigned char nsec = 0;
			// Points to the public nonce for the packet with must be 16 bytes in len.
			unsigned char npub[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6};
			// Points to the 16 bytes od the key to use to encrypt the packet.
			unsigned char k[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6};

						unsigned char m[sizeof(pressure_arr)];
						unsigned long long clen = sizeof(pressure_arr) + 16;
						unsigned char c[sizeof(pressure_arr)+16];
						memcpy(m, pressure_arr, sizeof(pressure_arr));
						unsigned long long mlen = sizeof(pressure_arr);
						int err = crypto_aead_encrypt(c, &clen, m, mlen, ad, adlen, nsec, npub, k);
						err = crypto_aead_decrypt(m, &mlen, &nsec, c, clen, ad, adlen, npub, k);
						char file_path[256];
						unsigned long long name = get_unix_timestamp();
						int tmp =  sprintf(file_path, "/opt/usr/home/owner/data/%lld.enc", name);
						write_encrypted_data(c, file_path, sizeof(pressure_arr)+16);

						char upload_path[256];
						tmp = sprintf(upload_path, "ftp://ftp.cytr.us/pressure/%lld.enc", name);


						upload_file_ftp(file_path, upload_path);
						remove(file_path);
					} else {
						pressure_arr[pressure_counter].timestamp = get_unix_timestamp();
						pressure_arr[pressure_counter].value = event->values[0];
						pressure_counter = pressure_counter + 1;
					}
	}
}

void set_cb_pressure() {
	char head[256];
	int size = 0;
	size = sprintf(head, "%s;%s\n", "Timestamp", "Value");
	create_log_file(pressure_log, head, size);

	int ret;
	ret = sensor_get_default_sensor(SENSOR_PRESSURE, &preassure_sensor);
	ret = sensor_create_listener(preassure_sensor, &preassure_listener);
	ret = sensor_listener_set_events_cb(preassure_listener, (void*)pressure_cb, NULL);
	ret = sensor_listener_set_interval(preassure_listener, 6000);
	ret = sensor_listener_set_option(preassure_listener, SENSOR_OPTION_ALWAYS_ON);
	ret = sensor_listener_start(preassure_listener);
}
