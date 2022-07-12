typedef struct {
	long long timestamp;
	int accuracy;
	float x;
	float y;
	float z;
} acc_data;

typedef struct {
	long long timestamp;
	int value;
} hr_data;

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

typedef struct {
	long long timestamp;
	int steps;
	int walk_steps;
	int run_steps;
	float distance;
	float calories;
	float last_speed;
	float last_stepping_freq;
	short state;
} pedometer_data;

typedef struct {
	long long timestamp;
	double value;
} pressure_data;