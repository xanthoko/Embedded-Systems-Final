from random import randint, shuffle

output_file = '../data/addresses.txt'

NUMBER_OF_DIFFERENT_ADDRESSES = 70


def generate_mac_addresses():
    addresses = [randint(1, 100) for _ in range(NUMBER_OF_DIFFERENT_ADDRESSES)]
    shuffle(addresses)

    high_chance = addresses[:5]
    mid_chance = addresses[5:15]
    low_chance = addresses[10:]

    final_addresses = high_chance * 10 + mid_chance * 5 + low_chance

    with open(output_file, 'w') as f:
        for final_address in final_addresses:
            f.write(f'{final_address}\n')

    print(f'[INFO]: Generated {len(final_addresses)} addresses')


generate_mac_addresses()
