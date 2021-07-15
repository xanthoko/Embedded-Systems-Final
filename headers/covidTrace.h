#ifndef COVIDTRACE_H
#define COVIDTRACE_H

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 121
#define SPEED_FACTOR 100.0
#define TOTAL_DURATION 30 * 24 * 60 * 60 / SPEED_FACTOR
#define SEARCH_INTERVAL 10 / SPEED_FACTOR
#define LOWER_CLOSE_LIMIT 4 * 60 / SPEED_FACTOR
#define UPPER_CLOSE_LIMIT 20 * 60 / SPEED_FACTOR
#define COVID_TEST_INTERVAL 4 * 60 * 60 / SPEED_FACTOR
#define DELETE_CONTACT_THRESH 20 * 60 / SPEED_FACTOR
#define DELETE_CLOSE_THRESH 14 * 24 * 60 * 60 / SPEED_FACTOR


// ------------------ TIMER ------------------
struct timeval tic(void);
double toc(struct timeval begin);
double get_tod_in_mill(void);
char *get_datetime(void);

// ------------------ CONTACTS ------------------
typedef struct int48 {
    int64_t x:48;
} __attribute__((packed)) mac_address;

typedef struct contact_details{
    mac_address address;
    double time_found;
    bool is_close;
} contact_details;

void init_contacts(contact_details* contacts);
void insert_contact(contact_details* contacts, contact_details contact);
void print_contacts(contact_details* contacts);
int search_contact_by_address(contact_details* contacts, mac_address address);
void delete_contact_in_index(contact_details* contacts, int index);

// ------------------ ACTIONS ------------------
mac_address scan_bt(void);
void create_new_contact(contact_details* contacts, mac_address address);
void delete_non_close_contacts(contact_details* contacts);
void delete_close_contacts(contact_details* contacts);
void convert_to_close_if_eligible(contact_details* contacts, int contact_index);
bool testCOVID(void);
void uploadContacts(contact_details* contacts);
void log_scan_time(FILE* fh);

#endif