''' This module creates all synthetic test cases for validating our proposal '''

import DataGenerator as dg
import os
import subprocess as sp
import random as rd

# Global variables
gbl_path_index_gen = os.getenv("HOME")+'/Documentos/PROJECTS/ConditionalBinOps/SourceCodes/IndexGenerator/Debug/IndexGenerator'
gbl_path_relCondForAllAny = os.getenv("HOME")+'/Documentos/PROJECTS/ConditionalBinOps/SourceCodes/CondBinOps/Debug/CondBinOps'
gbl_want_generate_index = False
gbl_want_execute_queries = False
gbl_nr_repetitions = 10

# Global Default Features for Datasets
default_size_pred = 3
default_size_T1 = 1000
default_size_T2 = 10
default_size_TG = 10
default_dist_groups = 'normal'
default_percentage_TR = None
default_cond_op = 'ForAll'
# tests directory
work_path = 'TESTS/TEST 1/'

def generate_index(dir, nr_atts, file_name = 'T1.data'):
    ''' This function creates the indexes for the data in the current folder
    Note: This one calls an external function named IndexGenerator. Please make 
    sure you haved written the path of it into the global variable'''

    print(f'Generating Indexes in {dir}')
    for i in range(nr_atts):
        command = f'cd "{dir}"'
        command += f' && {gbl_path_index_gen} {file_name} {i} simple'
        sp.run(command, shell = True)

def execute_query(dir, cond_op, file_T1 = 'T1.data', file_T2 = 'T2.data', file_groups_name = 'TG.data'):
    ''' This function execute the queries for the dataset in the current folder
    Note: This one calls an external function named RelationalCondition. Please make 
    sure you haved written the path of it into the global variable'''

    command = f'cd "{dir}"'
    command += f' && {gbl_path_relCondForAllAny} {file_T1} {file_T2} query{cond_op} -groups {file_groups_name} -index T1'
    command += f' && {gbl_path_relCondForAllAny} {file_T1} {file_T2} query{cond_op} -groups {file_groups_name}'
    for iRep in range(gbl_nr_repetitions):
        sp.run(command, shell = True)

def generate_var_T1(end = 10000, nr_steps = 50):
    # Preparing variables for this variation
    step = int(end/nr_steps)
    start = step
    var_dir = 'VarT1/'
    # Generate all datasets
    iCase = 1
    curr_size_T1 = start
    while curr_size_T1 <= end:
        # create the directory
        curr_var_directory = work_path + var_dir + str(iCase) + '/'
        os.makedirs(curr_var_directory, exist_ok = True)
        # generate and save the dataset
        predicate, T1, T2 = dg.generate_dataset(default_size_pred, curr_size_T1, default_size_T2, 
            default_size_TG, default_dist_groups, default_percentage_TR, default_cond_op)
        dg.save_dataset(predicate, T1, T2, curr_var_directory)
        # generate the indexes for all attributes if user wants to
        # TODO: remove the +2 by removing the mandatority of need all attributes with indexes 
        if gbl_want_generate_index: generate_index(curr_var_directory, predicate.size + 2)
        # execute the queries if the user wants to
        if gbl_want_execute_queries: execute_query(curr_var_directory, 'ForAll')
        if gbl_want_execute_queries: execute_query(curr_var_directory, 'ForAny')
        # continue with the next case
        curr_size_T1 += step
        iCase += 1


def generate_var_T2(end = 100, nr_steps = 50):
    # Preparing variables for this variation
    step = int(end/nr_steps)
    start = step
    var_dir = 'VarT2/'
    # Generate all datasets
    iCase = 1
    curr_size_T2 = start
    while curr_size_T2 <= end:
        # create the directory
        curr_var_directory = work_path + var_dir + str(iCase) + '/'
        os.makedirs(curr_var_directory, exist_ok = True)
        # generate and save the dataset
        predicate, T1, T2 = dg.generate_dataset(default_size_pred, default_size_T1, curr_size_T2, 
            default_size_TG, default_dist_groups, default_percentage_TR, default_cond_op)
        dg.save_dataset(predicate, T1, T2, curr_var_directory)
        # generate the indexes for all attributes if user wants to
        # TODO: remove the +2 by removing the mandatority of need all attributes with indexes 
        if gbl_want_generate_index: generate_index(curr_var_directory, predicate.size + 2)
        # execute the queries if the user wants to
        if gbl_want_execute_queries: execute_query(curr_var_directory, 'ForAll')
        if gbl_want_execute_queries: execute_query(curr_var_directory, 'ForAny')
        # continue with the next case
        curr_size_T2 += step
        iCase += 1


def generate_var_TG(end = 100, nr_steps = 50):
    # Preparing variables for this variation
    step = int(end/nr_steps)
    start = step
    var_dir = 'VarTG/'
    # Generate all datasets
    iCase = 1
    curr_size_TG = start
    while curr_size_TG <= end:
        # create the directory
        curr_var_directory = work_path + var_dir + str(iCase) + '/'
        os.makedirs(curr_var_directory, exist_ok = True)
        # generate and save the dataset
        predicate, T1, T2 = dg.generate_dataset(default_size_pred, default_size_T1, default_size_T2, 
            curr_size_TG, default_dist_groups, default_percentage_TR, default_cond_op)
        dg.save_dataset(predicate, T1, T2, curr_var_directory)
        # generate the indexes for all attributes if user wants to
        # TODO: remove the +2 by removing the mandatority of need all attributes with indexes 
        if gbl_want_generate_index: generate_index(curr_var_directory, predicate.size + 2)
        # execute the queries if the user wants to
        if gbl_want_execute_queries: execute_query(curr_var_directory, 'ForAll')
        if gbl_want_execute_queries: execute_query(curr_var_directory, 'ForAny')
        # continue with the next case
        curr_size_TG += step
        iCase += 1


def generate_var_atts(end = 100, nr_steps = 50):
    # Preparing variables for this variation
    step = int(end/nr_steps)
    start = step
    var_dir = 'VarL1L2/'
    # Generate all datasets
    iCase = 1
    curr_size_pred = start
    while curr_size_pred <= end:
        # create the directory
        curr_var_directory = work_path + var_dir + str(iCase) + '/'
        os.makedirs(curr_var_directory, exist_ok = True)
        # generate and save the dataset
        predicate, T1, T2 = dg.generate_dataset(curr_size_pred, default_size_T1, default_size_T2, 
            default_size_TG, default_dist_groups, default_percentage_TR, default_cond_op)
        dg.save_dataset(predicate, T1, T2, curr_var_directory)
        # generate the indexes for all attributes if user wants to
        # TODO: remove the +2 by removing the mandatority of need all attributes with indexes 
        if gbl_want_generate_index: generate_index(curr_var_directory, predicate.size + 2)
        # execute the queries if the user wants to
        if gbl_want_execute_queries: execute_query(curr_var_directory, 'ForAll')
        if gbl_want_execute_queries: execute_query(curr_var_directory, 'ForAny')
        # continue with the next case
        curr_size_pred += step
        iCase += 1


def generate_var_valid_groups(cond_op, end = 100, nr_steps = 50):
    # Preparing variables for this variation
    step = int(end/nr_steps)
    start = 0
    var_dir = f'VarValidGroups{cond_op}/'
    # Generate all datasets
    iCase = 1
    curr_default_percentage_TR = start
    while curr_default_percentage_TR <= end:
        # create the directory
        curr_var_directory = work_path + var_dir + str(iCase) + '/'
        os.makedirs(curr_var_directory, exist_ok = True)
        # generate and save the dataset
        predicate, T1, T2 = dg.generate_dataset(default_size_pred, default_size_T1, default_size_T2, 
            default_size_TG, default_dist_groups, curr_default_percentage_TR, cond_op)
        dg.save_dataset(predicate, T1, T2, curr_var_directory)
        # generate the indexes for all attributes if user wants to
        # TODO: remove the +2 by removing the mandatority of need all attributes with indexes 
        if gbl_want_generate_index: generate_index(curr_var_directory, predicate.size + 2)
        # execute the queries if the user wants to
        if gbl_want_execute_queries: execute_query(curr_var_directory, cond_op)
        # continue with the next case
        curr_default_percentage_TR += step
        iCase += 1


def generate_var_distribution(nr_steps):
    # Preparing variables for this variation
    var_dir = 'VarDistribution/'
    # Generate all datasets
    dists = ['normal', 'exp', 'uniform']
    iCase = 1
    for dist in dists:
        for iRep in range(nr_steps):
            # create the directory
            curr_var_directory = work_path + var_dir + str(iCase) + '/'
            os.makedirs(curr_var_directory, exist_ok = True)
            # generate and save the dataset
            predicate, T1, T2 = dg.generate_dataset(default_size_pred, default_size_T1, default_size_T2, 
                default_size_TG, default_dist_groups, default_percentage_TR, default_cond_op)
            dg.save_dataset(predicate, T1, T2, curr_var_directory)
            # generate the indexes for all attributes if user wants to
            # TODO: remove the +2 by removing the mandatority of need all attributes with indexes 
            if gbl_want_generate_index: generate_index(curr_var_directory, predicate.size + 2)
            # execute the queries if the user wants to
            if gbl_want_execute_queries: execute_query(curr_var_directory, 'ForAll')
            if gbl_want_execute_queries: execute_query(curr_var_directory, 'ForAny')
            # continue with the next case
            iCase += 1


if __name__ == '__main__':
    rd.seed(500)
    
    ans = input('Do you want to generate indexes? (y/n): ')
    if ans == 'y': gbl_want_generate_index = True
    
    ans = input('Do you want to execute the queries? (y/n): ')
    if ans == 'y': 
        gbl_want_execute_queries = True
        gbl_nr_repetitions = int(input('How many times do you want execute each query?: '))
    
    nr_steps = 10
    # for i in range(1,2):
        # work_path = f'TESTS/TEST {i}/'
    # generate_var_T1(10000, nr_steps)
    generate_var_T2(100, nr_steps)
    generate_var_TG(100, nr_steps)
    generate_var_atts(100, nr_steps)
    generate_var_valid_groups('ForAll', 100, nr_steps)
    generate_var_valid_groups('ForAny', 100, nr_steps)
    generate_var_distribution(nr_steps)