#include <stdio.h>
#include <service_app.h>
#include <time.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include "forensics.h"

void create_log_file(char* path, char *data, int size);

void write_to_logfile(char* path, long long timestamp, double value,
		bool value_bool);

void write_to_logfile_line(char* path, char* data, int size);

unsigned long long get_unix_timestamp();
