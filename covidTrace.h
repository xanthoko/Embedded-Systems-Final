#include <stdio.h>

#define SIZE 121
#define SCAN_SUCCESS_PROBABILITY 50
#define DELETE_CONTACT_THRESH 1200
#define DELETE_CLOSE_THRESH 14*24*60 
#define LOWER_CLOSE_LIMIT 240
#define UPPER_CLOSE_LIMIT 1200

int get_seconds_of_tod(){
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec;
}

int get_useconds_of_tod(){
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_usec;
}

typedef struct contact_details{
    int address;
    int time_found;
    bool is_close;
} contact_details;

// ------------------ CONTACTS ARRAY ------------------
void init_contacts(contact_details* contacts){
    for(int i=0;i<SIZE;i++){
        contact_details cd;
        cd.address = -1;
        contacts[i] = cd;
    }
}

void insert_contact(contact_details* contacts, contact_details contact){
    int contact_index = 0;

    while(contacts[contact_index].address != -1 && contact_index < SIZE){
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
        if (contacts[i].address != -1){
            printf("(%d, %d, %d), ", contacts[i].address, contacts[i].time_found, contacts[i].is_close);
        }
        ++i;
    }
    printf("\n");
}

int search_contact_by_address(contact_details* contacts, int address){
    int i=0;
    while(i < SIZE){
        if (contacts[i].address == address){
            return i;
        }
        i++;
    }
    return -1;
}

void delete_contact_in_index(contact_details* contacts, int index){
    contact_details deleted_contact;
    deleted_contact.address = -1;
    contacts[index] = deleted_contact;
}

// ------------------ ACTIONS ------------------
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