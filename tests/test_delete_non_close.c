#include "covidTrace.h"

contact_details contacts[SIZE];


void create_dummy_contacts(){
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


int main (int argc, char **argv){
    printf("[TEST] Deleting non close contacts\n");
    init_contacts(contacts);
    create_dummy_contacts();

    delete_non_close_contacts(contacts);

    // check that only the first contact is deleted
    assert(contacts[0].address == -1);
    assert(contacts[1].address != -1);
    assert(contacts[2].address != -1);
    printf("---------------------------  ✅\n");

    return 0;
}