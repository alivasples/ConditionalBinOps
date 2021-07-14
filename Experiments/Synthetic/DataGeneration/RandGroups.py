import random
import numpy as np
from CondSets import CondSet

def get_rand_grouping(nr_items, nr_groups, dist):
    ''' This function returns an array of <nr_items> items 
    grouped in <nr_grops> groups according to a distribution '''
    # Validating parameters
    valid_dists = {'uniform', 'normal', 'exp'}
    dist = dist.lower()
    assert(dist in valid_dists), 'Distribution must be one of ' + str(valid_dists)
    # Creating an empty list of groups counts
    groups = np.zeros(nr_groups, dtype = int)
    # filling the groups array
    curr_items = 0
    while curr_items < nr_items:
        # getting a value according to the distribution
        if dist == 'normal': val = int(random.gauss(nr_groups/2, nr_groups/4))
        elif dist == 'exp': val = int(random.expovariate(0.50))
        elif dist == 'uniform': val = int(random.uniform(0, nr_groups))
        else: raise ValueError('Distribution ' + dist + ' not supported/implemented.')
        # Adding the value to our groups array
        if val >= 0 and val < nr_groups:
            groups[val] += 1
            curr_items += 1
    # returning the grouped items
    return groups


import matplotlib.pyplot as plt
if __name__ == '__main__':
    groups = get_rand_grouping(1000, 10, 'normal')
    plt.plot(groups)
    plt.show()

    groups = get_rand_grouping(1000, 10, 'exp')
    plt.plot(groups)
    plt.show()

    groups = get_rand_grouping(1000, 10, 'uniform')
    plt.plot(groups)
    plt.show()