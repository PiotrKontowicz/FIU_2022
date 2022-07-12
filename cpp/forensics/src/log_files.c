#include "log_files.h"

void write_encrypted_data(unsigned char *data, const char *filepath, unsigned int size) {
	FILE *fp;
	fp = fopen(filepath, "wb");
	fwrite(data, sizeof(unsigned char), size, fp);
	fclose(fp);
}

unsigned long long get_unix_timestamp() {
	time_t t = time(0);
	return t;
}

void read_encrypted_data(unsigned char *c, const char *filepath, unsigned long long *clen) {
    FILE *fp;
    fp = fopen(filepath, "rb");
    if(fp){
        fseek(fp, 0L, SEEK_END);
        *clen = ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        fread(c, sizeof(unsigned char), *clen, fp);
        fclose(fp);
    }
}

//void write_encrypted_data(unsigned char *c, const char *filepath, unsigned long long clen) {
//	FILE *fp;
//    //printf("Save file in: %s\n", filepath);
//	fp = fopen(filepath, "wb");
//        if(fp!=NULL){
//            fwrite(c, sizeof(unsigned char), clen, fp);
//            fclose(fp);
//        }
//        else
//            printf("Error: Can not write file!.\n");
//}

void create_log_file(char* path, char *data, int size) {
	FILE *fp;
	fp = fopen(path, "w");
	fwrite(data, sizeof(char), size, fp);
	fclose(fp);
}

void write_to_logfile(char* path, long long timestamp, double value,
bool value_bool) {
	FILE *fp;
	fp = fopen(path, "a");
	char buffer[128];
	int size;
	if (value_bool) {
		size = sprintf(buffer, "%lld;%f\n", timestamp, value);
	} else {
		size = sprintf(buffer, "%lld;%d\n", timestamp, (int) value);
	}

	fwrite(buffer, sizeof(char), size, fp);
	fclose(fp);
}

void write_to_logfile_line(char* path, char* data, int size) {
	FILE *fp;
	fp = fopen(path, "a");
	fwrite(data, sizeof(char), size, fp);
	fclose(fp);
}

static size_t read_callback(char *ptr, size_t size, size_t nmemb, void *stream) {
	curl_off_t nread;
	size_t retcode = fread(ptr, size, nmemb, stream);
	nread = (curl_off_t) retcode;
	return retcode;
}

bool upload_file_ftp(const char* file_path, const char* upload_url) {
	CURL *curl;
	CURLcode res;
	FILE *hd_src;
	struct stat file_info;
	curl_off_t fsize;

	if (stat(file_path, &file_info)) {
		dlog_print(DLOG_ERROR, "A_MY_TAG", "No such file!");
		return false;
	}
	fsize = (curl_off_t) file_info.st_size;
	hd_src = fopen(file_path, "rb");

	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_URL, upload_url);
		curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
		curl_easy_setopt(curl, CURLOPT_USERNAME, "tizen");
		curl_easy_setopt(curl, CURLOPT_PASSWORD, "ZHuNYZ9A");
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t )fsize);

		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
			dlog_print(DLOG_ERROR, "A_MY_TAG", "curl_easy_perform() failed: %s",
			        curl_easy_strerror(res));

		curl_easy_cleanup(curl);
	}
	fclose(hd_src);
	curl_global_cleanup();
	return true;
}
