#include "covidTrace.h"

contact_details contacts[SIZE];


int main (int argc, char **argv){
    init_contacts(contacts);

    struct timeval current_search_start, current_covid_start;
    current_search_start = tic();
    current_covid_start = tic();
    FILE *fh = fopen ("../data/scans.bin", "wb");

    int c = 0;
    // while(1){
    while(c < 150){
        float elapsed_from_last_search = toc(current_search_start);
        float elapsed_from_last_covid_test = toc(current_covid_start);

        if (elapsed_from_last_search > SEARCH_INTERVAL){
            c++;
            current_search_start = tic();

            // --- ACTION: Scan for new BT contacts
            log_scan_time(fh);
            mac_address scan_result = scan_bt();

            if (scan_result.x != -1){
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
