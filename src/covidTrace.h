#ifndef COVIDTRACE_H
#define COVIDTRACE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 121
#define SCAN_SUCCESS_PROBABILITY 50
#define DELETE_CONTACT_THRESH 1200
#define DELETE_CLOSE_THRESH 14*24*60 
#define LOWER_CLOSE_LIMIT 240
#define UPPER_CLOSE_LIMIT 1200

struct timeval tic(void);
double toc(struct timeval begin);
int get_seconds_of_tod(void);
int get_useconds_of_tod(void);

// ------------------ CONTACTS ------------------
typedef struct contact_details{
    int address;
    int time_found;
    bool is_close;
} contact_details;

void init_contacts(contact_details* contacts);
void insert_contact(contact_details* contacts, contact_details contact);
void print_contacts(contact_details* contacts);
int search_contact_by_address(contact_details* contacts, int address);
void delete_contact_in_index(contact_details* contacts, int index);

// ------------------ ACTIONS ------------------
int scan_bt(void);
void create_new_contact(contact_details* contacts, int address);
void delete_non_close_contacts(contact_details* contacts);
void delete_close_contacts(contact_details* contacts);
void convert_to_close_if_eligible(contact_details* contacts, int contact_index);
bool testCOVID(void);
void uploadContacts(contact_details* contacts);
void log_scan_time(FILE* fh);

#endif