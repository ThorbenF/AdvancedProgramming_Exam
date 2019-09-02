import numpy as np
import matplotlib.pyplot as plt
from scipy import stats


def plot_times(node_number, log_N, map, BST_unbalanced, BST_balanced):
    plt.figure()
    x = np.arange(25, 10000000, 10)
    y = 20 * np.log(x)
    plt.plot(x, y, '-.', color='black', linewidth=2, label='log(N)')
    plt.plot(node_number, map, 'go', label='std::map', alpha=0.25)
    plt.plot(node_number, BST_unbalanced, 'ro',
             label='BST unbalanced', alpha=0.25)
    plt.plot(node_number, BST_balanced, 'bo',
             label='BST balanced', alpha=0.25)
    plt.xlabel('Number of nodes N')
    plt.ylabel('Average lookup-time in ns')
    plt.legend()
    plt.tight_layout()
    plt.savefig('./lookup_times_linear_scale.png', dpi=1000)
    plt.close()


def main():

    node_number = []
    log_N = []
    map = []
    BST_unbalanced = []
    BST_balanced = []
    stepsize = 10000

    f = open('./AverageLookupTimes.txt')
    for count, line in enumerate(f):
        nums = line.split()
        if count > 2:
            x = (count - 2) * stepsize
            node_number.append(x)
            log_N.append(20 * np.log2(x))
            map.append(float(nums[0]))
            BST_unbalanced.append(float(nums[1]))
            BST_balanced.append(float(nums[2]))

    plot_times(node_number, log_N, map, BST_unbalanced, BST_balanced)


main()
