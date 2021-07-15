import struct

SCAN_INTERVAL = 0.1


def _read_scan_file():
    """Reads the data"""
    scans_file_path = '../data/scans.bin'

    reads = []
    with open(scans_file_path, 'rb') as f:
        byte = f.read(8) # double -> 8 bytes
        while byte != b"":
            reads.append(struct.unpack('d', byte)[0])
            byte = f.read(8) # double -> 8 bytes
    return reads


def get_diffs_of_scan_times():
    scan_times = _read_scan_file()
    if not scan_times:
        print("[ERROR] No scans found.")
        return

    base_scan_time = scan_times[0]

    relative_scan_times = [(x - base_scan_time) / 1000 for x in scan_times]
    baseline_scan_times = [x * SCAN_INTERVAL for x in range(len(scan_times))]
    diffs = [
        rst - bst for rst, bst in zip(relative_scan_times, baseline_scan_times)
    ]
    return diffs


print(get_diffs_of_scan_times())

