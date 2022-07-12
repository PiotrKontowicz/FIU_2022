#include "types.h"

const unsigned int hr_rows = 10;
const unsigned int hr_array_size = (sizeof(hr_data) / sizeof(unsigned char)) * hr_rows;

const unsigned int acc_rows = 1;
const unsigned int acc_array_size = ((sizeof(acc_data)/sizeof(unsigned char)) * acc_rows);

const unsigned int pressure_rows = 1;
const unsigned int pressure_array_size = ((sizeof(pressure_data)/sizeof(unsigned char)) * pressure_rows);

const unsigned int location_rows = 1;
const unsigned int pedometer_rows = 1;