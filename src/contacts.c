#include "covidTrace.h"


void init_contacts(contact_details* contacts){
    for(int i=0;i<SIZE;i++){
        contact_details cd;
        cd.address.x = -1;
        contacts[i] = cd;
    }
}


void insert_contact(contact_details* contacts, contact_details contact){
    int contact_index = 0;

    while(contacts[contact_index].address.x != -1 && contact_index < SIZE){
        ++contact_index;
    }
    if(contact_index == SIZE){
        printf("[ERROR] Contacts array is full\n");  // hope this does not happen
    }
    else{
        contacts[contact_index] = contact;
    }
}


void print_contacts(contact_details* contacts){
    int i=0;
    printf("Contacts: ");
    while(i < SIZE){
        if (contacts[i].address.x != -1){
            printf("(%llu, %f, %d), ", contacts[i].address.x, contacts[i].time_found, contacts[i].is_close);
        }
        ++i;
    }
    printf("\n");
}


int search_contact_by_address(contact_details* contacts, mac_address address){
    int i=0;
    while(i < SIZE){
        if (contacts[i].address.x == address.x){
            return i;
        }
        i++;
    }
    return -1;
}


void delete_contact_in_index(contact_details* contacts, int index){
    contact_details deleted_contact;
    deleted_contact.address.x = -1;
    contacts[index] = deleted_contact;
}