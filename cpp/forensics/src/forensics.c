#include <tizen.h>
#include <service_app.h>
#include "forensics.h"
#include <sensor.h>
#include <privacy_privilege_manager.h>
#include <time.h>
#include <stdio.h>

#include "log_files.h"
#include "acc.h"
#include "hr.h"

// uninstall command: pkgcmd -u -n org.example.forensics
// Log files loction: /opt/usr/home/owner/data/*.log

bool checkSupport(sensor_type_e type) {
	bool support;
	sensor_is_supported(type, &support);
	return support;
}

void init_cb_sensors() {
	set_cb_hrm();
	dlog_print(DLOG_DEBUG, "APP_FIU", "Set hrm.");
	set_cb_acc();
	dlog_print(DLOG_DEBUG, "APP_FIU", "Set acc.");

}

void app_request_response_cb(ppm_call_cause_e cause,
		ppm_request_result_e result, const char *privilege, void *user_data) {
	if (cause == PRIVACY_PRIVILEGE_MANAGER_CALL_CAUSE_ERROR) {
		/* Log and handle errors */
		return;
	}
	switch (result) {
	case PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_ALLOW_FOREVER:
		init_cb_sensors();
		break;
	case PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_FOREVER:
		dlog_print(DLOG_ERROR, "APP_FIU", "DENY FOREVER");
		break;
	case PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_ONCE:
		dlog_print(DLOG_ERROR, "APP_FIU", "DENY ONCE");
		break;
	}
}

void sensor_init() {
	if ( checkSupport(SENSOR_HRM) && checkSupport(SENSOR_ACCELEROMETER)) {
		ppm_check_result_e result;
		ppm_check_permission("http://tizen.org/privilege/healthinfo", &result);
		switch (result) {
			case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ALLOW:
				dlog_print(DLOG_ERROR, "APP_FIU",
						"PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ALLOW");
				break;
			case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_DENY:
				dlog_print(DLOG_ERROR, "APP_FIU",
						"PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_DENY");
				break;
			case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ASK:
				dlog_print(DLOG_INFO, "APP_FIU",
						"Privacy manager request permission.");
				ppm_request_permission("http://tizen.org/privilege/healthinfo",
						app_request_response_cb, NULL);
				break;
		}
	} else {
		dlog_print(DLOG_ERROR, "APP_FIU", "Pressure is not supported");
	}

}

void _sensor_stop_cb(void *data, void *event_info) {
	return;
}

bool service_app_create(void *data) {
	sensor_init();
	return true;
}

void service_app_terminate(void *data) {
	// Todo: add your code here.
	return;
}

void service_app_control(app_control_h app_control, void *data) {
	// Todo: add your code here.
	return;
}

static void service_app_lang_changed(app_event_info_h event_info,
		void *user_data) {
	/*APP_EVENT_LANGUAGE_CHANGED*/
	return;
}

static void service_app_region_changed(app_event_info_h event_info,
		void *user_data) {
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void service_app_low_battery(app_event_info_h event_info,
		void *user_data) {
	/*APP_EVENT_LOW_BATTERY*/
}

static void service_app_low_memory(app_event_info_h event_info, void *user_data) {
	/*APP_EVENT_LOW_MEMORY*/
}

int main(int argc, char* argv[]) {
	char ad[50] = { 0, };
	service_app_lifecycle_callback_s event_callback;
	app_event_handler_h handlers[5] = { NULL, };

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	service_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY],
			APP_EVENT_LOW_BATTERY, service_app_low_battery, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY],
			APP_EVENT_LOW_MEMORY, service_app_low_memory, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED],
			APP_EVENT_LANGUAGE_CHANGED, service_app_lang_changed, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED],
			APP_EVENT_REGION_FORMAT_CHANGED, service_app_region_changed, &ad);

	return service_app_main(argc, argv, &event_callback, ad);
}
