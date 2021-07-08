#include "covidTrace.h"

contact_details contacts[SIZE];


void create_dummy_contacts(){
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


int main (int argc, char **argv){
    printf("[TEST] Converting to close contacts\n");
    init_contacts(contacts);
    create_dummy_contacts();
    
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

    return 0;
}
