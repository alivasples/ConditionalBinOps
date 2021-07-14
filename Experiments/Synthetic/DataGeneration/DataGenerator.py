''' This module will generate datasets for testing the quantifiers operations '''

import sys
import random as rd 
from CondSets import * 
import RandGroups as rg

def generate_predicate(size = 3):
    ''' This function creates a predicate of size <size> '''
    operators = ['=']
    connectors = []
    for i in range(1, size):
        operators.append(LogicOp.valids[i%len(LogicOp.valids)])
        connectors.append(Connector.valids[(i+1)%len(Connector.valids)])
    return Predicate(operators, connectors)


def generate_tuple(predicate, dif_limit = 100, first = None, num_variability = 100):
    ''' This function just generates a list of <size> values
    <first> is the first element we want to put in the tuple, just send None if you want a random value
    <dif_limit> is the minimum number of different strings values
    <number_variability> is the end of the range of a random number [1;num_variability] 
    '''
    assert(dif_limit <= 25**3 - 1), 'String combinatory cannot be greater than ' + str(25**3 - 1)
    str_limit = int(dif_limit ** (1./3.))
    tuple = []
    for i in range(predicate.size):
        # if first element is given, append it
        if first != None and i == 0:
            tuple.append(first)
        # if current attribute will be compared by identity, we can generate a string
        elif predicate.operators[i % predicate.size] == '=':
            str_id = "".join([chr(rd.randint(ord('a'),ord('a')+str_limit)) for i in range(3)])
            tuple.append(str_id)
        # if not, only append a number
        else: tuple.append(rd.randint(1, num_variability))
    return tuple


def generate_cond_set(predicate, size, dif_limit):
    ''' This function receives a predicate and a size and returns a 
    conditional with condition <predicate> of size <size>. It also receives a 
    <dif_limit> parameter that indicates the desired number of different strings
    '''
    # empty conditional set
    cond_set = CondSet(predicate)
    # inserting <size> random tuples
    while cond_set.size < size:
        cond_set.insert(generate_tuple(predicate, dif_limit))
    # returning our cond set
    return cond_set


def generate_relation_requirements(predicate, size, dif_limit = 100):
    return generate_cond_set(predicate, size, dif_limit)


def generate_relation_candidates(predicate, groups, dif_limit, requirements, percentage_result, cond_op):
    # creating an empty list of groups
    candidates_groups = []

    # CASE 1: If we don't mind about the number of resulting valid groups
    if percentage_result == None:
        # creating our different groups' sets
        for group_size in groups:    
            curr_group = generate_cond_set(predicate, group_size, dif_limit)
            candidates_groups.append(curr_group)
        # returning all candidates' groups
        return candidates_groups
        
    # CASE 2: If we want to get some certain number of valid groups in the result
    assert(percentage_result >= 0 and percentage_result <= 100), 'Percentage of valid groups must be in range [0;100]'
    assert(cond_op in {'ForAll', 'ForAny'}), 'conditional operation should be either "ForAll" or "ForAny", ' + str(cond_op) + ' is not valid'

    # getting the number of valid groups in the result
    nr_valid_groups = int(len(groups) * percentage_result / 100)
    curr_valid_groups = 0
    # creating our different groups' sets
    for id_group, group_size in enumerate(groups):    
        # Empty conditional set
        curr_group = CondSet(predicate)
        # filling our current group to satisfy all or only first requirements (depends on ForAll or ForAny)
        if curr_valid_groups < nr_valid_groups: 
            for id_req in range(requirements.size):
                # create a tuple with the first value of current requirement
                # and insert it into our conditional set (repeat until a valid tuple is found)
                for iteration in range(1001):
                    first = requirements.tuples[id_req][0]
                    tuple = generate_tuple(predicate, dif_limit, first)
                    # validate tuple is conditional member of requirements and not candidates group
                    if requirements.is_cond_member(tuple) and curr_group.insert(tuple): break
                    # overtime iterations
                    if iteration == 1000: raise Exception('We could not find a tuple for ' + first + ' to be inserted into candidates')
                # if we're in a For Any query, break the loop after the first requirement was inserted
                if cond_op == 'ForAny': break
            # At the end of the loop, our current group satisfies all or first requirement(s) (Depends of ForAll or ForAny type)
            curr_valid_groups += 1
        # Now, we need to fill the group with the rest of tuples
        while curr_group.size < group_size:
            # overtime validation
            for iteration in range(1001):
                if iteration == 1000: raise Exception('After 1000 iterations we could not find any tuple to insert into a group')
                # try to insert a tuple that is not cond member of requirements
                tuple = generate_tuple(predicate, dif_limit)
                if not requirements.is_cond_member(tuple) and curr_group.insert(tuple): break

        # Add group to candidates
        candidates_groups.append(curr_group)
            
    # returning the list of candidates' groups
    return candidates_groups


def generate_dataset(size_pred = 3, size_T1 = 1000, size_T2 = 10, size_TG = 10, dist_groups = 'exp', percentage_TR = None, cond_op = None):
    # generating a predicate
    predicate = generate_predicate(size_pred)
    # distributing our tuples into groups
    for i in range(1001): # avoid an empty group
        groups = rg.get_rand_grouping(size_T1, size_TG, dist_groups)
        if percentage_TR != None and min(groups) >= size_T2: break
        if percentage_TR == None and min(groups) > 0: break
        if i == 1000:
            print('Groups:', groups) 
            raise Exception('After 1000 iterations we could not generate a group with that distribution with size > ' + str(size_T2))
    # the maximum number of different string will be based on max groups size
    dif_limit = max(groups)
    # getting our requirements' relation
    T2 = generate_relation_requirements(predicate, size_T2, dif_limit)
    T1 = generate_relation_candidates(predicate, groups, dif_limit, T2, percentage_TR, cond_op)
    # returning the datasets
    return predicate, T1, T2


def save_query(predicate, dir, name_T1, name_T2, cond_op):
    assert(cond_op in {'ForAll', 'ForAny'}), 'Conditional Operation must be either a ForAll or ForAny operation'
    # TODO: change our Conditional Operator in c++ to accept 'ForAll' and 'ForAny' instead of 'all' and 'any', then remove the next 2 lines
    if cond_op == 'ForAll': query_type = 'all' 
    else: query_type = 'any'
    # open files
    file_query = open(dir + 'query' + cond_op, 'w')
    # save query
    # print(f'{name_T1};{name_T2};{predicate.size};{query_type}', file = file_query)
    file_query.write(predicate.query_sql(cond_op))
    # closing files
    file_query.close()


def save_dataset(predicate, T1, T2, dir, name_T1 = 'T1.data', name_T2 = 'T2.data', name_TG = 'TG.data'):
    ''' This function saves the data of both candidates and requirements relations in files '''
    assert(dir[-1:] == '/'), 'A valid directory should end with /'
    # Opening storing files
    file_T1 = open(dir + name_T1, 'w')
    file_T2 = open(dir + name_T2, 'w')
    file_TG = open(dir + name_TG, 'w')
    # Saving data into files
    file_T1.write(';'.join(predicate.header) + ';id(float,1);idGroup(float,1);\n')
    iTuple = 0
    for iSet, aSet in enumerate(T1):
        for aTuple in aSet.tuples:
            str_tuple = ';'.join(str(x) for x in aTuple) + f';{iTuple};{iSet};'
            file_T1.write(str_tuple + '\n')
            file_TG.write(f'{iTuple}\t{iSet}\n')
            iTuple += 1
    
    file_T2.write(';'.join(predicate.header) + ';\n')
    for aTuple in T2.tuples:
        str_tuple = ';'.join(str(x) for x in aTuple) + ';'
        file_T2.write(str_tuple + '\n')
    # Also save queries
    save_query(predicate, dir, name_T1, name_T2, 'ForAll')
    save_query(predicate, dir, name_T1, name_T2, 'ForAny')
    # Closing files
    file_T1.close()
    file_T2.close()
    file_TG.close()
    # Confirm message
    print(f'One dataset succesfully generated and saved into: {dir} including: \n\t{name_T1}\n\t{name_T2}\n\t{name_TG}\n\tqueryForAll\n\tqueryForAny\n')


import unittest
class TestCondSets(unittest.TestCase):

    def test_gen_predicate(self):
        self.assertEqual(generate_predicate(3).operators, ['=','<','>'])
        self.assertEqual(generate_predicate(3).connectors, ['and', 'or'])
        self.assertEqual(generate_predicate(5).operators, ['=','<','>', '<=', '>='])
        self.assertEqual(generate_predicate(5).connectors, ['and', 'or', 'and', 'or'])
        self.assertEqual(generate_predicate(7).operators, ['=','<','>', '<=', '>=', '=', '<'])
        self.assertEqual(generate_predicate(7).connectors, ['and', 'or', 'and', 'or', 'and', 'or'])

    def test_gen_tuple(self):
        rd.seed(100)
        self.assertEqual(generate_tuple(generate_predicate(3)), ['eoo', 99, 23])
        self.assertEqual(generate_tuple(generate_predicate(4)), ['wmx', 45, 56, 65])
        self.assertEqual(generate_tuple(generate_predicate(5)), ['zdr', 16, 11, 95, 59])

if __name__ == '__main__':
    rd.seed(100)
    # unittest.main()
    # Default Features
    size_pred = 3
    size_T1 = 1000
    size_T2 = 10
    size_TG = 10
    dist_groups = 'normal'
    percentage_TR = None
    cond_op = None
    # default saving directory
    dir = './'
    name_T1 = 'sampleT1.data'
    name_T2 = 'sampleT2.data'
    name_TG = 'sampleTG.data'
    # If user gives inputs, change the default configuration
    # print(sys.argv[1:])
    # Generate the dataset
    predicate, T1, T2 = generate_dataset(size_pred, size_T1, size_T2, size_TG, dist_groups, percentage_TR, cond_op)
    # Saving our dataset
    save_dataset(predicate, T1, T2, dir, name_T1, name_T2, name_TG)