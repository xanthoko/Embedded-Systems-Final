#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "covidTrace.h"


int scan_bt(){
    // probability of successful scan
    int upper = 100, lower = 0;
    int random_number = (rand() % (upper - lower + 1)) + lower;
    if (random_number > SCAN_SUCCESS_PROBABILITY){
        printf("Scan failed\n");
        return 0;
    }

    upper = 1000;
    lower = 1;
    int mac_address = (rand() % (upper - lower + 1)) + lower;
    printf("Scan found: %d\n", mac_address);
    return mac_address;
}


void create_new_contact(contact_details* contacts, int address){
    int time_found = get_seconds_of_tod();
    contact_details cd;

    cd.address = address;
    cd.time_found = time_found;
    cd.is_close = false;

    insert_contact(contacts, cd);
}


void delete_non_close_contacts(contact_details* contacts){
    for (int i=0; i< SIZE; i++){
        if(contacts[i].address != -1){
            contact_details cd = contacts[i];
            int diff_from_time_found = get_seconds_of_tod() - cd.time_found;
            if (!cd.is_close && diff_from_time_found > DELETE_CONTACT_THRESH){
                delete_contact_in_index(contacts, i);
            }
        }
    }
}


void delete_close_contacts(contact_details* contacts){
    for(int i=0;i<SIZE;i++){
        if (contacts[i].address != -1){
            contact_details cd = contacts[i];
            int current_secs = get_seconds_of_tod();
            int diff_from_time_found = current_secs - cd.time_found;
            if (cd.is_close && diff_from_time_found > DELETE_CLOSE_THRESH){
                delete_contact_in_index(contacts, i);
            }
        }
    }
}


void convert_to_close_if_eligible(contact_details* contacts, int contact_index){
    contact_details contact = contacts[contact_index];
    int diff_from_time_found = get_seconds_of_tod() - contact.time_found;

    if (diff_from_time_found > LOWER_CLOSE_LIMIT && diff_from_time_found < UPPER_CLOSE_LIMIT){
        contact.is_close = true;
        contacts[contact_index] = contact;
    }
}


bool testCOVID(){
    return false;
}


void uploadContacts(contact_details* contacts){
    for(int i=0; i<SIZE; i++){
        if(contacts[i].address != -1 && contacts[i].is_close){
            // upload to server
        }
    }
}


void log_scan_time(FILE* fh){
    int secs = get_seconds_of_tod();
    int usecs = get_useconds_of_tod();
    fwrite(&secs, sizeof(secs), 1, fh);
    fwrite(&usecs, sizeof(usecs), 1, fh);
}