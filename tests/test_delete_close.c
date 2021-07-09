#include "covidTrace.h"

contact_details contacts[SIZE];


void create_dummy_contacts(){
    contact_details cd1, cd2, cd3;
    mac_address ma1, ma2, ma3;
    // case 1, close contact, time threshold passed, must be deleted
    ma1.x = 1;
    cd1.address = ma1;
    cd1.is_close = true;
    cd1.time_found = get_tod_in_mill() - DELETE_CLOSE_THRESH * 1000 - 5;
    insert_contact(contacts, cd1);
    // case 2, non close contact, must NOT be deleted
    ma2.x = 1;
    cd2.address = ma2; 
    cd2.is_close = false;
    cd2.time_found = get_tod_in_mill() - DELETE_CLOSE_THRESH * 1000 - 5;
    insert_contact(contacts, cd2);
    // case 3, close contact, time threshold not passed, must NOT be deleted
    ma3.x = 3;
    cd3.address = ma3; 
    cd3.is_close = true;
    cd3.time_found = get_tod_in_mill();
    insert_contact(contacts, cd3);
}


int main (int argc, char **argv){
    printf("[TEST] Deleting close contacts\n");
    init_contacts(contacts);
    create_dummy_contacts();

    delete_close_contacts(contacts);

    // check that only the first contact is deleted
    assert(contacts[0].address.x == -1);
    assert(contacts[1].address.x != -1);
    assert(contacts[2].address.x != -1);
    printf("---------------------------  ✅\n");

    return 0;
}