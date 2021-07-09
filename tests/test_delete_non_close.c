#include "covidTrace.h"

contact_details contacts[SIZE];


void create_dummy_contacts(){
    contact_details cd1, cd2, cd3;
    mac_address ma1, ma2, ma3;
    // case 1: non close contact, time threshold has passed, must be deleted
    ma1.x = 1;
    cd1.address = ma1;
    cd1.is_close = false;
    cd1.time_found = get_tod_in_mill() - DELETE_CONTACT_THRESH * 1000 - 2;
    insert_contact(contacts, cd1);
    // case 2: close contact, must NOT be deleted
    ma2.x = 2;
    cd2.address = ma2; 
    cd2.is_close = true;
    cd2.time_found = get_tod_in_mill() - DELETE_CONTACT_THRESH * 1000 - 2;
    insert_contact(contacts, cd2);
    // case 3: non close contact, time threshold not passed, must NOT be deleted
    ma3.x = 1;
    cd3.address = ma3; 
    cd3.is_close = false;
    cd3.time_found = get_tod_in_mill();
    insert_contact(contacts, cd3);
}


int main (int argc, char **argv){
    printf("[TEST] Deleting non close contacts\n");
    init_contacts(contacts);
    create_dummy_contacts();

    delete_non_close_contacts(contacts);

    // check that only the first contact is deleted
    assert(contacts[0].address.x == -1);
    assert(contacts[1].address.x != -1);
    assert(contacts[2].address.x != -1);
    printf("---------------------------  ✅\n");

    return 0;
}