#include <sys/time.h>
#include "covidTrace.h"


mac_address scan_bt(){
    mac_address ma;

    // probability of successful scan
    int upper = 100, lower = 0;
    int random_number = (rand() % (upper - lower + 1)) + lower;
    if (random_number > SCAN_SUCCESS_PROBABILITY){
        printf("Scan failed\n");
        ma.x = -1;
        return ma;
    }

    upper = 1000;
    lower = 1;
    int random_address = (rand() % (upper - lower + 1)) + lower;
    printf("Scan found: %d\n", random_address);

    ma.x = random_address;
    return ma;
}


void create_new_contact(contact_details* contacts, mac_address address){
    int time_found = get_seconds_of_tod();
    contact_details cd;

    cd.address = address;
    cd.time_found = time_found;
    cd.is_close = false;

    insert_contact(contacts, cd);
}


void delete_non_close_contacts(contact_details* contacts){
    for (int i=0; i< SIZE; i++){
        if(contacts[i].address.x != -1){
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
        if (contacts[i].address.x != -1){
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
    FILE *fh = fopen ("../data/close_contacts.txt", "w");

    char *current_datetime = malloc(30 * sizeof(char));
    current_datetime = get_datetime();

    fprintf(fh, "Positive Covid Test at %s\n", current_datetime);

    for(int i=0; i<SIZE; i++){
        if(contacts[i].address.x != -1 && contacts[i].is_close){
            // upload to server (write to file)
            int current_seconds = get_seconds_of_tod();
            int seconds_till_time_found = current_seconds - contacts[i].time_found;
            int h, m, s;
            h = (seconds_till_time_found / 3600);
            m = (seconds_till_time_found - (3600 * h)) / 60;
            s = (seconds_till_time_found - (3600 * h) - (m * 60));
            fprintf(fh, "Contact address: %llu, Time since last contact: %d:%d:%d\n", contacts[i].address.x, h, m ,s);
        }
    }

    fclose(fh);
}


void log_scan_time(FILE* fh){
    int secs = get_seconds_of_tod();
    int usecs = get_useconds_of_tod();
    fwrite(&secs, sizeof(secs), 1, fh);
    fwrite(&usecs, sizeof(usecs), 1, fh);
}