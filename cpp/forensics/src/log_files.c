#include "log_files.h"

unsigned long long get_unix_timestamp() {
	time_t t = time(0);
	return t;
}

void create_log_file(char* path, char *data, int size) {
	FILE *fp;
	fp = fopen(path, "w");
	fwrite(data, sizeof(char), size, fp);
	fclose(fp);
}

void write_to_logfile_line(char* path, char* data, int size) {
	FILE *fp;
	fp = fopen(path, "a");
	fwrite(data, sizeof(char), size, fp);
	fclose(fp);
}
