#include <stdio.h>
#include "forensics.h"
#include <service_app.h>
#include "log_files.h"
#include <locations.h>
#include <locations.h>

#include "crypto_aead.h"
#define data_size 1

typedef struct {
	long long timestamp;
	double altitude;
	double latitude;
	double longitude;
	double climb;
	double direction;
	double speed;
	double horizontal;
	double vertical;
} loc_data;

#define acc_message_size  ((sizeof(loc_data)/sizeof(unsigned char))*data_size)

static int loc_counter = 0;

loc_data loc_arr[data_size];

static char location_log[] = "/opt/usr/home/owner/data/location.log";
location_manager_h manager;
static location_service_state_e service_state;
static double user_latitude;
static double user_longitude;

void init_location();
