from typing import List

SCAN_INTERVAL = 1


def _read_scan_file():
    """Reads the data"""
    scans_file_path = '../data/scans.bin'

    reads = []
    with open(scans_file_path, 'rb') as f:
        while (byte := f.read(8)):  # double -> 8 bytes
            reads.append(int.from_bytes(byte, "little"))
    return reads


def _get_scan_times(scan_file_reads: List[int]):
    """Merges the seconds and the milliseconds"""
    scan_times = []
    for read_index in range(0, len(scan_file_reads), 2):
        seconds = scan_file_reads[read_index]
        useconds = scan_file_reads[read_index + 1]
        str_time = f'{seconds}.{useconds}'
        scan_time = float(str_time)
        scan_times.append(scan_time)
    return scan_times


def get_diffs_of_scan_times():
    scan_file_reads = _read_scan_file()
    scan_times = _get_scan_times(scan_file_reads)

    base_scan_time = scan_times[0]
    relative_scan_times = [x - base_scan_time for x in scan_times]
    baseline_scan_times = [x * SCAN_INTERVAL for x in range(len(scan_times))]
    diffs = [
        rst - bst for rst, bst in zip(relative_scan_times, baseline_scan_times)
    ]
    print(diffs)


print(_read_scan_file())
