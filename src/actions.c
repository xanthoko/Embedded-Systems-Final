#include "covidTrace.h"

// close contacts: 1, 32, 111,/ 309, 2/, 8397
int current_scan_index = 0;
int current_array_index = 0;
int scan_indexes[] = {500, 600, 610, 4000, 5000, 10000, 10050, 12000, 12002, 30123, 54000, 54109, 100000, 100992, 110399, 110450, 169722, 170000, 170100, 200000, 230999, 240000, 240110};
int addresses[] = {    1,   2,   1,   50,   12,   32,    32,    30,    30,    220,   111,   111,   1032,   1033,   309,    309,    1,       2,       2,    7096,    503,   8397,   8397};

int current_covid_index = 0;
int current_carray_index = 0;
int test_indexes[] = {37, 124};


mac_address scan_bt(){
    mac_address ma;

    int upcoming_scan_index = scan_indexes[current_array_index];
    if (current_scan_index == upcoming_scan_index){
        ma.x = addresses[current_array_index];
        current_array_index++;
        printf("Scan found. address: %lld scan_index: %d\n", ma.x, current_scan_index);
    }
    else{
        ma.x = -1;
    }
    current_scan_index++;
    return ma;
}


void create_new_contact(contact_details* contacts, mac_address address){
    contact_details cd;

    cd.address = address;
    cd.time_found = get_tod_in_mill();
    cd.is_close = false;

    insert_contact(contacts, cd);
}


void delete_non_close_contacts(contact_details* contacts){
    for (int i=0; i< SIZE; i++){
        if(contacts[i].address.x != -1){
            contact_details cd = contacts[i];
            float diff_from_time_found = (get_tod_in_mill() - cd.time_found) / 1000;

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
            float diff_from_time_found = (get_tod_in_mill() - cd.time_found) / 1000;

            if (cd.is_close && diff_from_time_found > DELETE_CLOSE_THRESH){
                delete_contact_in_index(contacts, i);
            }
        }
    }
}


void convert_to_close_if_eligible(contact_details* contacts, int contact_index){
    contact_details contact = contacts[contact_index];
    float diff_from_time_found = (get_tod_in_mill() - contact.time_found) / 1000;

    if (diff_from_time_found > LOWER_CLOSE_LIMIT && diff_from_time_found < UPPER_CLOSE_LIMIT){
        contact.is_close = true;
        contacts[contact_index] = contact;
    }
}


bool testCOVID(){
    bool ret = false;
    if (current_covid_index == test_indexes[current_carray_index]){
        current_carray_index++;
        ret = true;
    }
    printf("Covid test: %d. Result: %d\n", current_covid_index, ret);
    current_covid_index++;
    return ret;
}


void uploadContacts(contact_details* contacts){
    char *current_datetime = malloc(30 * sizeof(char));
    current_datetime = get_datetime();

    FILE *fh = fopen ("../data/close_contacts.txt", "a");
    fprintf(fh, "Positive Covid Test at %s\n", current_datetime);

    for(int i=0; i<SIZE; i++){
        if(contacts[i].address.x != -1 && contacts[i].is_close){
            // upload to server (write to file)
            float ms_till_time_found = get_tod_in_mill() - contacts[i].time_found;
            int seconds_till_time_found = ms_till_time_found / 1000;
            int h, m, s;
            h = (seconds_till_time_found / 3600);
            m = (seconds_till_time_found - (3600 * h)) / 60;
            s = (seconds_till_time_found - (3600 * h) - (m * 60));
            fprintf(fh, "Contact address: %llu, Time since last contact: %d:%d:%d\n", contacts[i].address.x, h, m ,s);
        }
    }
    fprintf(fh,"----------------------------------------\n\n");

    fclose(fh);
}


void log_scan_time(FILE* fh){
    double time_in_mills = get_tod_in_mill();
    fwrite(&time_in_mills, sizeof(time_in_mills), 1, fh);
}


void clear_close_contacts_file(){
    FILE *fh_temp = fopen ("../data/close_contacts.txt", "w");
    fclose(fh_temp);
}