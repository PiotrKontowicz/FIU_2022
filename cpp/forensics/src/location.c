#include "location.h"

static void location_cb(location_service_state_e state, void *user_data) {
	double altitude;
	double latitude;
	double longitude;
	double climb;
	double direction;
	double speed;
	double horizontal;
	double vertical;
	location_accuracy_level_e level;
	time_t timestamp;

	if (state == LOCATIONS_SERVICE_ENABLED) {
		int ret = location_manager_get_location(manager, &altitude, &latitude,
				&longitude, &climb, &direction, &speed, &level, &horizontal,
				&vertical, &timestamp);

		if(loc_counter == data_size){
			loc_counter = 0;

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
			memcpy(m, loc_arr, sizeof(loc_arr));
			unsigned long long mlen = acc_message_size;
			int err = crypto_aead_encrypt(c, &clen, m, mlen, ad, adlen, nsec, npub, k);
			char file_path[256];
			unsigned long long name = get_unix_timestamp();
			int tmp =  sprintf(file_path, "/opt/usr/home/owner/data/%lld.enc", name);
			write_encrypted_data(c, file_path, acc_message_size+16);

			char upload_path[256];
			tmp = sprintf(upload_path, "ftp://ftp.cytr.us/location/%lld.enc", name);

			upload_file_ftp(file_path, upload_path);
			remove(file_path);

		} else {
			loc_arr[loc_counter].timestamp = get_unix_timestamp();
			loc_arr[loc_counter].altitude = altitude;
			loc_arr[loc_counter].latitude = latitude;
			loc_arr[loc_counter].longitude = longitude;
			loc_arr[loc_counter].climb = climb;
			loc_arr[loc_counter].speed = speed;
			loc_arr[loc_counter].horizontal = horizontal;
			loc_arr[loc_counter].vertical = vertical;

			loc_counter = loc_counter + 1;
		}

//		char buffer[256];
//		int size = 0;
//		size = sprintf(buffer, "%lld;%f;%f\n", get_unix_timestamp(), latitude, longitude);
//
//		write_to_logfile_line(location_log, buffer, size);
//		dlog_print(DLOG_DEBUG, "AAAAAA",
//				"CHANGED CB latitude: %f longitude: %f", latitude, longitude);
	}
}

//static void
//position_updated(double latitude, double longitude, double altitude,
//                 time_t timestamp, void *user_data)
//{
//    user_latitude = latitude;
//    user_longitude = longitude;
//
//    dlog_print(DLOG_DEBUG, "AAAAAA", "POSITION UPDATAED latitude: %f longitude: %f", latitude, longitude);
//
//	char buffer[256];
//	int size = 0;
//	size = sprintf(buffer, "%lld;%f;%f\n", timestamp, latitude, longitude);
//
//    write_to_logfile_line(location_log, buffer, size);
//}

void init_location() {

	char head[256];
	int size = 0;
	size = sprintf(head, "Timestamp;Latitude;Longitude\n");
	create_log_file(location_log, head, size);

	int ret;

	ret = location_manager_create(LOCATIONS_METHOD_GPS, &manager);
	dlog_print(DLOG_DEBUG, "AAAAAA", "create %d", ret);
	//ret = location_manager_set_service_state_changed_cb(manager, __state_changed_cb, NULL);
	ret = location_manager_set_position_updated_cb(manager, location_cb, 30000,
			NULL);
	dlog_print(DLOG_DEBUG, "AAAAAA", "Changed %d", ret);
	ret = location_manager_start(manager);
	dlog_print(DLOG_DEBUG, "AAAAAA", "start %d", ret);

	double altitude;
	double latitude;
	double longitude;
	double climb;
	double direction;
	double speed;
	double horizontal;
	double vertical;
	location_accuracy_level_e level;
	time_t timestamp;
	ret = location_manager_get_last_location(manager, &altitude, &latitude,
			&longitude, &climb, &direction, &speed, &level, &horizontal,
			&vertical, &timestamp);
	dlog_print(DLOG_DEBUG, "AAAAAA", "Get last location %d", ret);

	char buffer[256];

	size = sprintf(buffer, "%lld;%f;%f\n", get_unix_timestamp(), latitude, longitude);

	write_to_logfile_line(location_log, buffer, size);

	loc_arr[loc_counter].timestamp = get_unix_timestamp();
	loc_arr[loc_counter].altitude = altitude;
	loc_arr[loc_counter].latitude = latitude;
	loc_arr[loc_counter].longitude = longitude;
	loc_arr[loc_counter].climb = climb;
	loc_arr[loc_counter].speed = speed;
	loc_arr[loc_counter].horizontal = horizontal;
	loc_arr[loc_counter].vertical = vertical;

	loc_counter = loc_counter + 1;


	loc_counter = 0;

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
	memcpy(m, loc_arr, sizeof(loc_arr));
	unsigned long long mlen = acc_message_size;
	int err = crypto_aead_encrypt(c, &clen, m, mlen, ad, adlen, nsec, npub, k);
	char file_path[256];
	unsigned long long name = get_unix_timestamp();
	int tmp =  sprintf(file_path, "/opt/usr/home/owner/data/%lld.enc", name);
	write_encrypted_data(c, file_path, acc_message_size+16);

	char upload_path[256];
	tmp = sprintf(upload_path, "ftp://ftp.cytr.us/location/%lld.enc", name);

	upload_file_ftp(file_path, upload_path);
	remove(file_path);

//	ret = location_manager_set_position_updated_cb(manager, position_updated, 2, NULL);
//	dlog_print(DLOG_DEBUG, "AAAAAA", "Pos update %d", ret);
}
