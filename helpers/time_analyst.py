import struct
from matplotlib import pyplot as plt


def _read_scan_file():
    """Reads the data"""
    scans_file_path = '../data/scans.bin'

    reads = []
    with open(scans_file_path, 'rb') as f:
        byte = f.read(8)  # double -> 8 bytes
        while byte != b"":
            reads.append(struct.unpack('d', byte)[0])
            byte = f.read(8)  # double -> 8 bytes
    return reads


def get_relative_scan_times(scan_times):
    base_scan_time = scan_times[0]
    relative_scan_times = [(x - base_scan_time) / 1000 for x in scan_times]
    return relative_scan_times


def get_baseline_scan_times(number_of_scans):
    return [x * (10 / 100) for x in range(number_of_scans)]


def get_diffs_of_scan_times():
    scan_times = _read_scan_file()
    relative_scan_times = get_relative_scan_times(scan_times)
    baseline_scan_times = get_baseline_scan_times(len(scan_times))
    diffs = [
        rst - bst for rst, bst in zip(relative_scan_times, baseline_scan_times)
    ]
    return diffs


diffs = get_diffs_of_scan_times()

fig = plt.figure()
fig.suptitle("Diff between actual and baseline scan time")
plt.xlabel('scan index')
plt.ylabel('time (seconds)')
plt.plot(diffs)
fig.savefig('../data/diffs.jpg')
