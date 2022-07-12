#include "crypto_aead.h"
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include <stdio.h>
#include "data_arr_size.h"
#include "enc_data.h"

const char ftp_path[] = "../data/ftp";
char path[512];

int read_encrypted_data(const char *filepath, uint8_t *data, int data_size)
{
    FILE *fp;
    fp = fopen(filepath, "rb");
    if (!fp)
    {
        printf("No such file: %s\n", filepath);
        fclose(fp);
        return -1;
    }
    fread(data, sizeof(uint8_t), data_size + 16, fp);
    fclose(fp);
    return 0;
}

void decrypt_hr()
{
    unsigned char m[hr_array_size]; // Buffer to receive the plaintext
    unsigned long long mlen = hr_array_size; // Length of the plaintext

    unsigned char c[hr_array_size + 16]; // Buffer to receive output
    unsigned long long clen = hr_array_size + 16; // Output lenght which includes the ciphertext and the 16 bytes auth tag.

    DIR *d;
    struct dirent *dir;
    int s = sprintf(path, "%s/hr/", ftp_path); 
    d = opendir(path);
    printf("Timestamp;HrValue\n");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            s = sprintf(path, "%s/hr/%s", ftp_path, dir->d_name);
            if(path[s-1] == 'c' && path[s-2] == 'n' && path[s-3] == 'e'){
                if (read_encrypted_data(path, c, hr_array_size) == 0) {
                    int err = crypto_aead_decrypt(m, &mlen, &nsec, c, clen, ad, adlen, npub, k);
                    if (err != 0)
                        printf("Error in decryption: %d\n", err);

                    hr_data restored_data[hr_rows];
                    memcpy(restored_data, m, sizeof(m));

                    int i = 0;
                    for (i = 0; i < hr_rows; i++)
                        printf("%llu;%d\n", restored_data[i].timestamp, restored_data[i].value);
                }
            }
        }
        closedir(d);
    }
}

void decrypt_acc()
{
    unsigned char m[acc_array_size]; // Buffer to receive the plaintext
    unsigned long long mlen = acc_array_size; // Length of the plaintext

    unsigned char c[acc_array_size + 16]; // Buffer to receive output
    unsigned long long clen = acc_array_size + 16; // Output lenght which includes the ciphertext and the 16 bytes auth tag.

    DIR *d;
    struct dirent *dir;
    int s = sprintf(path, "%s/acc/", ftp_path); 
    d = opendir(path);
    printf("Timestamp;Accuracy;X;Y;Z\n");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            s = sprintf(path, "%s/acc/%s", ftp_path, dir->d_name);
            if(path[s-1] == 'c' && path[s-2] == 'n' && path[s-3] == 'e'){
                if (read_encrypted_data(path, c, acc_array_size) == 0) {
                    int err = crypto_aead_decrypt(m, &mlen, &nsec, c, clen, ad, adlen, npub, k);
                    if (err != 0)
                        printf("Error in decryption: %d\n", err);

                    acc_data restored_data[acc_rows];
                    memcpy(restored_data, m, sizeof(m));

                    int i = 0;
                    for (i = 0; i < acc_rows; i++)
                        printf("%lld;%d;%f;%f;%f\n", restored_data[i].timestamp, restored_data[i].accuracy, restored_data[i].x, restored_data[i].y, restored_data[i].z);
                }
            }
        }
        closedir(d);
    }
}

// void decrypt_pressure()
// {
//     unsigned char m[16*pressure_rows]; // Buffer to receive the plaintext
//     unsigned long long mlen = 16; // Length of the plaintext

//     unsigned char c[(16 * pressure_rows) + 16]; // Buffer to receive output
//     unsigned long long clen = (16 * pressure_rows) + 16; // Output lenght which includes the ciphertext and the 16 bytes auth tag.

//     DIR *d;
//     struct dirent *dir;
//     int s = sprintf(path, "%s/pressure/", ftp_path); 
//     d = opendir(path);
//     printf("Timestamp;Pressure\n");
//     if (d)
//     {
//         while ((dir = readdir(d)) != NULL)
//         {
//             s = sprintf(path, "%s/pressure/%s", ftp_path, dir->d_name);
//             if(path[s-1] == 'c' && path[s-2] == 'n' && path[s-3] == 'e'){
//                 if (read_encrypted_data(path, c, ((16 * pressure_rows) + 16)) == 0) {
//                     int err = crypto_aead_decrypt(m, &mlen, &nsec, c, clen, ad, adlen, npub, k);
//                     if (err != 0)
//                         printf("Error in decryption: %d\n", err);

//                     pressure_data restored_data[pressure_rows];
//                     memcpy(restored_data, m, sizeof(m));

//                     int i = 0;
//                     for (i = 0; i < pressure_rows; i++)
//                         printf("%llu;%f\n", restored_data[i].timestamp, restored_data[i].value);
//                 }
//             }
//         }
//         closedir(d);
//     }
// }


void decrypt_pressure()
{
    unsigned char m[sizeof(pressure_data)*pressure_rows]; // Buffer to receive the plaintext
    unsigned long long mlen = sizeof(pressure_data)*pressure_rows; // Length of the plaintext

    unsigned char c[sizeof(pressure_data)*(pressure_rows + 1)]; // Buffer to receive output
    unsigned long long clen = sizeof(pressure_data)*(pressure_rows + 1); // Output lenght which includes the ciphertext and the 16 bytes auth tag.

    DIR *d;
    struct dirent *dir;
    int s = sprintf(path, "%s/pressure/", ftp_path); 
    d = opendir(path);
    printf("Timestamp;Pressure\n");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            s = sprintf(path, "%s/pressure/%s", ftp_path, dir->d_name);
            if(path[s-1] == 'c' && path[s-2] == 'n' && path[s-3] == 'e'){
                if (read_encrypted_data(path, c, sizeof(pressure_data)*(pressure_rows + 1)) == 0) {
                    int err = crypto_aead_decrypt(m, &mlen, &nsec, c, clen, ad, adlen, npub, k);
                    if (err != 0)
                        printf("Error in decryption: %d\n", err);

                    pressure_data restored_data[pressure_rows];
                    memcpy(restored_data, m, sizeof(m));

                    int i = 0;
                    for (i = 0; i < pressure_rows; i++)
                        printf("%llu;%f\n", restored_data[i].timestamp, restored_data[i].value);
                }
            }
        }
        closedir(d);
    }
}

void decrypt_location()
{
    unsigned char m[sizeof(loc_data)*location_rows]; // Buffer to receive the plaintext
    unsigned long long mlen = sizeof(loc_data)*location_rows; // Length of the plaintext

    unsigned char c[(sizeof(loc_data)*(location_rows)) + 16]; // Buffer to receive output
    unsigned long long clen = (sizeof(loc_data)*(location_rows)) + 16; // Output lenght which includes the ciphertext and the 16 bytes auth tag.

    DIR *d;
    struct dirent *dir;
    int s = sprintf(path, "%s/location/", ftp_path); 
    d = opendir(path);
    printf("Timestamp;Altitude;Latitude;Longitude;Climb;Direction;Speed;Horizontal;Vertical;\n");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            s = sprintf(path, "%s/location/%s", ftp_path, dir->d_name);
            if(path[s-1] == 'c' && path[s-2] == 'n' && path[s-3] == 'e'){
                if (read_encrypted_data(path, c, (sizeof(loc_data)*(location_rows)) + 16) == 0) {
                    int err = crypto_aead_decrypt(m, &mlen, &nsec, c, clen, ad, adlen, npub, k);
                    if (err != 0)
                        printf("Error in decryption: %d\n", err);

                    loc_data restored_data[location_rows];
                    memcpy(restored_data, m, sizeof(m));

                    int i = 0;
                    for (i = 0; i < location_rows; i++)
                        printf("%llu;%f;%f;%f;%f;%f;%f;%f;%f\n", restored_data[i].timestamp, restored_data[i].altitude, restored_data[i].latitude, restored_data[i].longitude,
                        restored_data[i].climb, restored_data[i].direction, restored_data[i].speed, restored_data[i].horizontal, restored_data[i].vertical);
                }
            }
        }
        closedir(d);
    }
}

void decrypt_pedometer()
{
    unsigned char m[sizeof(pedometer_data)*pedometer_rows]; // Buffer to receive the plaintext
    unsigned long long mlen = sizeof(pedometer_data)*pedometer_rows; // Length of the plaintext

    unsigned char c[(sizeof(pedometer_data)*(pedometer_rows)) + 16]; // Buffer to receive output
    unsigned long long clen = (sizeof(pedometer_data)*(pedometer_rows)) + 16; // Output lenght which includes the ciphertext and the 16 bytes auth tag.

    DIR *d;
    struct dirent *dir;
    int s = sprintf(path, "%s/pedometer/", ftp_path); 
    d = opendir(path);
    printf("Timestamp;Steps;Walk_steps;Run_steps;Distance;Calories;Last_speed;Last_stepping_freq;State\n");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            s = sprintf(path, "%s/pedometer/%s", ftp_path, dir->d_name);
            if(path[s-1] == 'c' && path[s-2] == 'n' && path[s-3] == 'e'){
                if (read_encrypted_data(path, c, (sizeof(pedometer_data)*(pedometer_rows)) + 16) == 0) {
                    int err = crypto_aead_decrypt(m, &mlen, &nsec, c, clen, ad, adlen, npub, k);
                    if (err != 0)
                        printf("Error in decryption: %d\n", err);

                    pedometer_data restored_data[pedometer_rows];
                    memcpy(restored_data, m, sizeof(m));

                    int i = 0;
                    for (i = 0; i < pedometer_rows; i++)
                        printf("%llu;%d;%d;%d;%f;%f;%f;%f;%d\n", restored_data[i].timestamp, restored_data[i].steps, restored_data[i].walk_steps, restored_data[i].run_steps,
                        restored_data[i].distance, restored_data[i].calories, restored_data[i].last_speed, restored_data[i].last_stepping_freq, restored_data[i].state);
                }
            }
        }
        closedir(d);
    }
}


int main() {
    decrypt_hr();
    decrypt_acc();
    decrypt_pressure();
    decrypt_location();
    decrypt_pedometer();
}

