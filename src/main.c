#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "covidTrace.h"

#define SEARCH_INTERVAL 2
#define COVID_TEST_INTERVAL 14400


contact_details contacts[SIZE];

// ------------------ TESTS ------------------
void create_dummy_contacts_for_deleting_non_close_contacts(){
    contact_details cd1, cd2, cd3;
    // case 1: non close contact, time threshold has passed, must be deleted
    cd1.address = 1;
    cd1.is_close = false;
    cd1.time_found = get_seconds_of_tod() - DELETE_CONTACT_THRESH - 2;
    insert_contact(contacts, cd1);
    // case 2: close contact, must NOT be deleted
    cd2.address = 2; 
    cd2.is_close = true;
    cd2.time_found = get_seconds_of_tod() - DELETE_CONTACT_THRESH - 2;
    insert_contact(contacts, cd2);
    // case 3: non close contact, time threshold not passed, must NOT be deleted
    cd3.address = 3; 
    cd3.is_close = false;
    cd3.time_found = get_seconds_of_tod();
    insert_contact(contacts, cd3);
}

void create_dummy_contacts_for_deleting_close_contacts(){
    contact_details cd1, cd2, cd3;
    // case 1, close contact, time threshold passed, must be deleted
    cd1.address = 1;
    cd1.is_close = true;
    cd1.time_found = get_seconds_of_tod() - DELETE_CLOSE_THRESH - 5;
    insert_contact(contacts, cd1);
    // case 2, non close contact, must NOT be deleted
    cd2.address = 2; 
    cd2.is_close = false;
    cd2.time_found = get_seconds_of_tod() - DELETE_CLOSE_THRESH - 5;
    insert_contact(contacts, cd2);
    // case 3, close contact, time threshold not passed, must NOT be deleted
    cd3.address = 3; 
    cd3.is_close = true;
    cd3.time_found = get_seconds_of_tod();
    insert_contact(contacts, cd3);
}

void create_dummy_contact_for_converting_to_close(){
    contact_details cd1, cd2, cd3;
    // case 1, non close contact, inside time range, must convert
    cd1.address = 1;
    cd1.is_close = false;
    cd1.time_found = get_seconds_of_tod() - LOWER_CLOSE_LIMIT - 10;
    insert_contact(contacts, cd1);

    // case 2, non close contact, inside time range, must convert
    cd2.address = 2;
    cd2.is_close = false;
    cd2.time_found = get_seconds_of_tod() - UPPER_CLOSE_LIMIT + 10;
    insert_contact(contacts, cd2);

    // case 3, non close contact, outside time range, must NOT convert
    cd3.address = 3;
    cd3.is_close = false;
    cd3.time_found = get_seconds_of_tod();
    insert_contact(contacts, cd3);
}

void test_deleting_non_close_contacts(){
    printf("[TEST] Deleting non close contacts\n");
    init_contacts(contacts);
    create_dummy_contacts_for_deleting_non_close_contacts();

    delete_non_close_contacts(contacts);

    // check that only the first contact is deleted
    assert(contacts[0].address == -1);
    assert(contacts[1].address != -1);
    assert(contacts[2].address != -1);
    printf("---------------------------  ✅\n");
}

void test_deleting_close_contacts(){
    printf("[TEST] Deleting close contacts\n");
    init_contacts(contacts);
    create_dummy_contacts_for_deleting_close_contacts();

    delete_close_contacts(contacts);

    // check that only the first contact is deleted
    assert(contacts[0].address == -1);
    assert(contacts[1].address != -1);
    assert(contacts[2].address != -1);
    printf("---------------------------  ✅\n");
}

void test_converting_to_close_contacts(){
    printf("[TEST] Converting to close contacts\n");
    init_contacts(contacts);
    create_dummy_contact_for_converting_to_close();
    
    for (int i=0;i<SIZE;i++){
        if (contacts[i].address != -1){
            convert_to_close_if_eligible(contacts, i);
        }
    }

    // Check that the first 2 contacts are converted to close
    assert(contacts[0].is_close == true);
    assert(contacts[1].is_close == true);
    assert(contacts[2].is_close == false);
    printf("---------------------------  ✅\n");
}


int main (int argc, char **argv){
    // test_deleting_non_close_contacts();
    // test_deleting_close_contacts();
    // test_converting_to_close_contacts();
    // return -1;

    init_contacts(contacts);
    struct timeval current_search_start, current_covid_start;
    current_search_start = tic();
    current_covid_start = tic();
    FILE *fh = fopen ("../data/scans.bin", "a+b");

    int c = 0;
    // while(1){
    while(c < 4){
        float elapsed_from_last_search = toc(current_search_start);
        float elapsed_from_last_covid_test = toc(current_covid_start);

        if (elapsed_from_last_search > SEARCH_INTERVAL){
            c++;
            current_search_start = tic();

            // --- ACTION: Scan for new BT contacts
            log_scan_time(fh);
            int scan_result = scan_bt();

            if (scan_result != 0){
                int search_index = search_contact_by_address(contacts, scan_result);

                if (search_index == -1){
                    create_new_contact(contacts, scan_result);
                }
                else{
                    // --- ACTION check if contact must be converted to close
                    convert_to_close_if_eligible(contacts, search_index);
                }
            }
            // --- ACTION: Delete non-close contacts after 20 mins
            delete_non_close_contacts(contacts);

            // --- ACTION: Delete close contact after 14 days
            delete_close_contacts(contacts);

            print_contacts(contacts);
        }

        if (elapsed_from_last_covid_test > COVID_TEST_INTERVAL && testCOVID()){
            current_covid_start = tic();
            uploadContacts(contacts);
        }
    }
    fclose(fh);
}
