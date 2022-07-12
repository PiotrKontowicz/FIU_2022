#include <stdio.h>
#include "forensics.h"
#include <service_app.h>
#include "log_files.h"

#include <wifi-manager.h>


static char wifi_log[] = "/opt/usr/home/owner/data/wifi.log";

wifi_manager_h wifi;

void init_wifi();
