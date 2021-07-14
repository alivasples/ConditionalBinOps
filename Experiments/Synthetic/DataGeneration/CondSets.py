class LogicOp:
    ''' This class stores a list of logical operators and has a method to 
    evaluate the logic operation between two values '''
    valids = ['=', '<', '>', '<=', '>=']

    def test_isupper(self):
        self.assertTrue('FOO'.isupper())
        self.assertFalse('Foo'.isupper())

    def test_split(self):
        s = 'hello world'
        self.assertEqual(s.split(), ['hello', 'world'])
        # check that s.split fails when the separator is not a string
        with self.assertRaises(TypeError):
            s.split(2)
    
    @staticmethod
    def eval(a, op, b):
        assert(op == '=' or type(a) == int or type(a) == float), 'Error with 1st parameter "' + a +'". Should be numeric or must be compared by identity.'
        assert(op == '=' or type(b) == int or type(b) == float), 'Error with 2nd parameter "' + b +'". Should be numeric or must be compared by identity.'
        assert(op in LogicOp.valids), 'Invalid operator "' + op + '". Should be one of ' + str(LogicOp.valids)
        if op == '=': return a == b
        if op == '<': return a < b
        if op == '<=': return a <= b
        if op == '>': return a > b
        if op == '>=': return a >= b
        raise ValueError('Operator ' + op + ' not supported.')

class Connector:
    ''' This class stores a list of logical connector and has a method to 
    evaluate the logic operation between two values '''
    valids = ['and', 'or']
    
    @staticmethod
    def eval(a, op, b):
        op = op.lower()
        assert(type(a) == bool), 'Error with 1st parameter "' + a +'". Should be bool.'
        assert(type(b) == bool), 'Error with 2nd parameter "' + b +'". Should be bool.'
        assert(op in Connector.valids), 'Invalid connector "' + op + '". Should be one of ' + str(Connector.valids)
        if op == 'and': return a and b
        if op == 'or': return a or b
        raise ValueError('Connector ' + op + ' not supported.')

class Predicate:
    ''' This class defines a predicate as a list of N comparisons {=,<,<=,>,>=} 
        and N-1 connectors {and, or}. Thus, the predicate between tuples t1 and t2
        is formed as (...(t1[0] comp[0] (t2[0] conn[0] (t1[1] comp[1] t2[1])))...)'''
    
    # CONSTRUCTOR
    def __init__(self, ops, conns):
        # Validating that parameters are valid
        assert(len(ops) == len(conns)+1), 'Number of connectors must be number logic operators - 1'
        for x in ops:
            assert(x in LogicOp.valids), 'Invalid operator "' + x + '". Should be one of ' + str(LogicOp.valids)
            
        conns = list(map(lambda x : x.lower(), conns))
        
        for x in conns:
            assert(x in Connector.valids), 'Invalid connector "' + x + '". Should be one of ' + str(Connector.valids) 
        
        # Initializing class attributes
        self._ops = ops
        self._conns = conns
        self._size = len(ops)
        
    # GETTERS AND SETTERS
    @property
    def size(self):
        ''' We assume as size of a predicate as the number of logical operations '''
        return self._size
    
    @property
    def operators(self):
        return self._ops
    
    @property
    def connectors(self):
        return self._conns

    @property
    def header(self):
        lst = []
        for i, operator in enumerate(self._ops):
            if operator == '=':
                lst.append(f'ATT{i}(string,10)')
            else:
                lst.append(f'ATT{i}(float,1)')
        return lst
    
    @property
    def query(self):
        connections = '1'
        operations = ''
        for i, connector in enumerate(self._conns):
            connections += f',{connector},'
            if i < len(self._conns)-1: connections += '('
            connections += f'{i+2}'
        connections += (len(self._conns)-1) * ')'
        for i, operator in enumerate(self._ops):
            operations += f'ATT{i};ATT{i};{operator};0\n'
        return connections + '\n' + operations

    # @property
    def query_sql(self, cond_op, left_table = 'T1', right_table = 'T2'):
        q = f'{left_table} {cond_op} {right_table} ON \n'
        for i, operator in enumerate(self._ops):
            q += f'{left_table}.ATT{i} {operator} {right_table}.ATT{i} '
            if i < len(self._conns):
                q += self._conns[i] + ' ('
        q += (len(self._ops) - 1) * ')'
        return q

    # SPECIFIC METHODS
    def eval(self, t1, t2):
        ''' evaluates the predicate for two tuples '''
        
        # validating the size of both tuples are equal to our predicate size
        assert(len(t1) == self._size and len(t2) == self._size), 'Tuples must have same predicate size'
        
        # evaluate every pair of columns
        # remember that 1&x=x, 0&x=0, 1|x=1, and 0|x=x
        # that's why we start iterating with the last operation (.(..).) op y
        # because if "y" is 1 and the operation is "or" we can assume the result is 1
        # if "y" is 0 we will keep iterating... similar if ioperation is "and"
        idx_last = self._size - 1
        for i in range(idx_last):
            value = LogicOp.eval(t1[i], self._ops[i], t2[i])
            if value == True and self._conns[i] == 'or': return True
            if value == False and self._conns[i] == 'and': return False
        # if for ended, the result is the only missing operation
        return LogicOp.eval(t1[idx_last], self._ops[idx_last], t2[idx_last])
        

class CondSet:
    ''' A conditional set is a set of tuples where not exists any pair of 
    tuples that satisfy the conditional Predicate. To improve our performance we assume 
    that the given predicate starts comparing by = and the first connector is AND. 
    Then, we group tuples in dictionaries, having as key the first attribute
    '''
    
    # CONSTRUCTOR
    def __init__(self, pred):
        self._size = 0
        self._tuples = dict()
        self._predicate = pred
        assert(pred.operators[0] == '='), 'In order to improve performance, please make first operator an equal (=)'
        assert(pred.connectors[0] == 'and'), 'In order to improve performance, please make first connector an "and"'
        
    # GETTERS AND SETTERS
    @property
    def size(self):
        return self._size
    
    @property
    def tuples(self):
        lst = []
        for aTuple in self._tuples.values():
            lst.extend(aTuple)
        return lst
    
    @property
    def predicate(self):
        return self._predicate
    
    # SPECIFIC METHODS
    def is_cond_member(self, newTuple):
        '''' Verify if a tuple satisfy the predicate with any of our intern tuples '''
        assert(len(newTuple) == self._predicate.size), 'Tuple must be a list of ' + str(self._predicate.size) + ' values'
        # If there is no key in our dictionary, return False
        key = newTuple[0]
        if key not in self._tuples: return False
        # Search for all tuples in respective dictionary
        for iTuple in self._tuples[key]:
            # if an intern tuple satisfies the predicate, return true
            if self._predicate.eval(newTuple, iTuple):
                return True
        # If there wasn't a tuple that satisfies our predicate, return false
        return False

    def insert(self, newTuple):
        ''' Try to insert a new tuple in the conditional set if there is no another 
        tuple that satisfies the predicate with the new one'''
        assert(len(newTuple) == self._predicate.size), 'Tuple must be a list of ' + str(self._predicate.size) + ' values'
        # If there is no key in our dictionary, just inserted
        key = newTuple[0]
        if newTuple[0] not in self._tuples:
            self._tuples[key] = [newTuple]
            self._size += 1
            return True
        # If there exists a tuple that satisfies the predicate, return false
        for iTuple in self._tuples[key]:
            # if an intern tuple satisfies the predicate, return false
            if self._predicate.eval(iTuple, newTuple) or self._predicate.eval(newTuple, iTuple):
                return False
        # If there was no tuple to satisfiy the predicate, the new tuple is inserted
        self._tuples[key].append(newTuple)
        self._size += 1
        return True
            

import unittest
class TestCondSets(unittest.TestCase):

    def test_logic_operations(self):
        self.assertTrue(LogicOp.eval(1,'=',1))
        self.assertTrue(LogicOp.eval(1,'<=',1))
        self.assertTrue(LogicOp.eval(1,'>=',1))
        self.assertTrue(LogicOp.eval(1,'<',2))
        self.assertTrue(LogicOp.eval(2,'>',1))
        self.assertTrue(LogicOp.eval(1,'<=',2))
        self.assertTrue(LogicOp.eval(2,'>=',1))

    def test_connectors(self):
        self.assertEqual(Connector.eval(True,'and',True), True)
        self.assertEqual(Connector.eval(True,'AnD',False), False)
        self.assertEqual(Connector.eval(False,'and',True), False)
        self.assertEqual(Connector.eval(False,'AND',False), False)
        self.assertEqual(Connector.eval(True,'oR',True), True)
        self.assertEqual(Connector.eval(True,'or',False), True)
        self.assertEqual(Connector.eval(False,'Or',True), True)
        self.assertEqual(Connector.eval(False,'or',False), False)
        
    def test_predicates_small(self):
        p = Predicate(['=', '='], ['and'])
        self.assertTrue(p.eval(['aaa','aaa'],['aaa','aaa']))
        self.assertTrue(p.eval(['bb','bb'],['bb','bb']))
        self.assertTrue(p.eval([1,'a'],[1,'a']))
        
        p = Predicate(['=', '=', '='], ['and', 'or'])
        self.assertTrue(p.eval(['aaa','aaa','aaa'], ['aaa','aaa','aaa']))
        self.assertFalse(p.eval(['aaa','aaa','aaa'], ['---','aaa','aaa']))
        self.assertTrue(p.eval(['aaa','aaa','aaa'], ['aaa','---','aaa']))
        self.assertFalse(p.eval(['aaa','aaa','aaa'], ['aaa','---','---']))
        
        p = Predicate(['<', '>'], ['and'])
        self.assertTrue(p.eval([1,10],[10,1]))
        self.assertTrue(p.eval([100,1000],[500,500]))
        self.assertTrue(p.eval([10,11],[11,10]))
        self.assertTrue(p.eval([1,2],[2,1]))
        
    def test_predicates_big(self):
        p = Predicate(['=', '=', '<', '>'], ['and', 'or', 'and'])
        self.assertFalse(p.eval(['aaa','aaa', 10, 10],['---','aaa', 999, 0]))
        self.assertTrue(p.eval(['aaa','aaa', 10, 10],['aaa','aaa', 10, 10]))
        self.assertFalse(p.eval(['aaa','aaa', 10, 10],['aaa','---', 10, 0]))
        self.assertFalse(p.eval(['aaa','aaa', 10, 10],['aaa','---', 999, 10]))
        self.assertTrue(p.eval(['aaa','aaa', 10, 10],['aaa','---', 999, 0]))
        
    def test_cond_member_small(self):
        cond_set = CondSet(Predicate(['=', '='], ['and']))
        cond_set.insert(['aaa', 'aaa'])
        self.assertTrue(cond_set.is_cond_member(['aaa', 'aaa']))
        self.assertFalse(cond_set.is_cond_member(['aaa', 'bbb']))
        self.assertFalse(cond_set.is_cond_member(['bbb', 'aaa']))
        self.assertFalse(cond_set.is_cond_member(['bbb', 'bbb']))

        cond_set = CondSet(Predicate(['=', '=', '='], ['and', 'or']))
        cond_set.insert(['aaa', 'aaa', 'aaa'])
        self.assertTrue(cond_set.is_cond_member(['aaa', 'aaa', 'aaa']))
        self.assertFalse(cond_set.is_cond_member(['---','aaa','aaa']))
        self.assertTrue(cond_set.is_cond_member(['aaa','---','aaa']))
        self.assertFalse(cond_set.is_cond_member(['aaa','---','---']))

    def test_cond_member_big(self):
        cond_set = CondSet(Predicate(['=', '=', '<', '>'], ['and', 'or', 'and']))
        cond_set.insert(['aaa', 'aaa', 10, 10])
        self.assertTrue(cond_set.is_cond_member(['aaa','aaa', 0, 999]))
        self.assertFalse(cond_set.is_cond_member(['---','aaa', 0, 999]))
        self.assertTrue(cond_set.is_cond_member(['aaa','aaa', 10, 10]))
        self.assertFalse(cond_set.is_cond_member(['aaa','---', 0, 10]))
        self.assertFalse(cond_set.is_cond_member(['aaa','---', 10, 999]))
        self.assertTrue(cond_set.is_cond_member(['aaa','---', 0, 999]))
        

    def test_cond_sets_small(self):
        cond_set = CondSet(Predicate(['=', '='], ['and']))
        self.assertTrue(cond_set.insert(['aaa', 'aaa']))
        self.assertTrue(cond_set.insert(['aaa', 'bbb']))
        self.assertTrue(cond_set.insert(['bbb', 'aaa']))
        self.assertTrue(cond_set.insert(['bbb', 'bbb']))
        self.assertFalse(cond_set.insert(['aaa', 'aaa']))
        self.assertFalse(cond_set.insert(['aaa', 'bbb']))
        self.assertFalse(cond_set.insert(['bbb', 'aaa']))
        self.assertFalse(cond_set.insert(['bbb', 'bbb']))
        self.assertEqual(cond_set.size, 4)
        
        cond_set = CondSet(Predicate(['=', '=', '='], ['and', 'or']))
        self.assertTrue(cond_set.insert(['aaa', 'aaa', 'aaa']))
        self.assertFalse(cond_set.insert(['aaa', 'aaa', '---']))
        self.assertFalse(cond_set.insert(['aaa', '---', 'aaa']))
        self.assertTrue(cond_set.insert(['aaa', 'bbb', 'bbb']))
        self.assertTrue(cond_set.insert(['bbb', 'aaa', 'aaa']))
        self.assertEqual(cond_set.size, 3)
        
    def test_cond_sets_big(self):
        cond_set = CondSet(Predicate(['=', '=', '<', '>'], ['and', 'or', 'and']))
        self.assertTrue(cond_set.insert(['aaa','aaa', 10, 10]))
        self.assertFalse(cond_set.insert(['aaa','aaa', 0, 999]))
        self.assertTrue(cond_set.insert(['bbb','aaa', 0, 999]))
        self.assertFalse(cond_set.insert(['aaa','aaa', 10, 10]))
        self.assertTrue(cond_set.insert(['aaa','bbb', 0, 10]))
        self.assertTrue(cond_set.insert(['aaa','ccc', 10, 999]))
        self.assertFalse(cond_set.insert(['aaa','ddd', 0, 999]))
        self.assertEqual(cond_set.size, 4)


if __name__ == '__main__':
    unittest.main()