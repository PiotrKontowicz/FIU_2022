#include "dev_wifi.h"

static void __wifi_manager_activated_cb(wifi_manager_error_e result,
		void *user_data) {
	if (result == WIFI_MANAGER_ERROR_NONE)
		dlog_print(DLOG_INFO, LOG_TAG, "Success to activate Wi-Fi device!");
}

static const char*
print_state(wifi_manager_connection_state_e state) {
	switch (state) {
	case WIFI_MANAGER_CONNECTION_STATE_DISCONNECTED:
		return "Disconnected";
	case WIFI_MANAGER_CONNECTION_STATE_ASSOCIATION:
		return "Association";
	case WIFI_MANAGER_CONNECTION_STATE_CONNECTED:
		return "Connected";
	case WIFI_MANAGER_CONNECTION_STATE_CONFIGURATION:
		return "Configuration";
	}
}

bool __wifi_manager_found_ap_cb(wifi_manager_ap_h ap, void *user_data) {
	int error_code = 0;
	char *ap_name = NULL;
	wifi_manager_connection_state_e state;

	error_code = wifi_manager_ap_get_essid(ap, &ap_name);
	if (error_code != WIFI_MANAGER_ERROR_NONE) {
		dlog_print(DLOG_INFO, LOG_TAG, "Failed to get AP name.");

		return false;
	}
	error_code = wifi_manager_ap_get_connection_state(ap, &state);
	if (error_code != WIFI_MANAGER_ERROR_NONE) {
		dlog_print(DLOG_INFO, LOG_TAG, "Failed to get state.");

		return false;
	}
	//dlog_print(DLOG_INFO, LOG_TAG, "AP name: %s, state: %s", ap_name, print_state(state));

	char buffer[256];
	int size = 0;
	size = sprintf(buffer, "%lld;%s;%s\n", get_unix_timestamp(), ap_name, print_state(state));
	write_to_logfile_line(wifi_log, buffer, size);

	return true;
}

void __scan_request_cb(wifi_manager_error_e error_code, void *user_data) {
	error_code = wifi_manager_foreach_found_ap(wifi, __wifi_manager_found_ap_cb, NULL);
	if (error_code != WIFI_MANAGER_ERROR_NONE)
		dlog_print(DLOG_INFO, LOG_TAG, "Failed to scan");
}

void init_wifi() {

	char head[256];
	int size = 0;
	size = sprintf(head, "Timestamp;Name\n");
	create_log_file(wifi_log, head, size);

	int error_code;
	error_code = wifi_manager_initialize(&wifi);

	switch (error_code) {
		case WIFI_MANAGER_ERROR_ALREADY_INITIALIZED: {
			dlog_print(DLOG_INFO, "AAAAAA", "WIFI_MANAGER_ERROR_ALREADY_INITIALIZED");
			break;
		}
		case WIFI_MANAGER_ERROR_INVALID_PARAMETER: {
			dlog_print(DLOG_INFO, "AAAAAA", "WIFI_MANAGER_ERROR_INVALID_PARAMETER");
			break;
		}
		case WIFI_MANAGER_ERROR_INVALID_OPERATION: {
			dlog_print(DLOG_INFO, "AAAAAA", "WIFI_MANAGER_ERROR_INVALID_OPERATION");
			break;
		}
		case WIFI_MANAGER_ERROR_OPERATION_FAILED: {
			dlog_print(DLOG_INFO, "AAAAAA", "WIFI_MANAGER_ERROR_OPERATION_FAILED");
			break;
		}
		case WIFI_MANAGER_ERROR_PERMISSION_DENIED: {
			dlog_print(DLOG_INFO, "AAAAAA", "WIFI_MANAGER_ERROR_PERMISSION_DENIED");
			break;
		}
		case WIFI_MANAGER_ERROR_OUT_OF_MEMORY: {
			dlog_print(DLOG_INFO, "AAAAAA", "WIFI_MANAGER_ERROR_OUT_OF_MEMORY");
			break;
		}
		case WIFI_MANAGER_ERROR_NOT_SUPPORTED: {
			dlog_print(DLOG_INFO, "AAAAAA", "WIFI_MANAGER_ERROR_NOT_SUPPORTED");
			break;
		}
	}
	if (error_code != WIFI_MANAGER_ERROR_NONE)
		return;

//	error_code = wifi_manager_activate(wifi, __wifi_manager_activated_cb, NULL);

	error_code = wifi_manager_scan(wifi, __scan_request_cb, NULL);
}
