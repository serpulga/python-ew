"""
Testing module for tracebuf2 functionality.
Currently only works for Rings with no previous
TRACEBUF2_TYPE data in it.
"""

import unittest

from tracebuf2ring import Tracebuf2Ring, Tracebuf2Message


class TestTracebuf2(unittest.TestCase):
    def setUp(self):
        self.tracebuf2 = Tracebuf2Ring('WAVE', 'MOD_WILDCARD')

        dict_trace = dict(
                  pinno=0, nsamp=100, starttime=0.0, endtime=0.0, samprate=100,
                  sta='BRU2', net='PA', chan='HHZ', loc='--', version='20', 
                  datatype='i4', quality='00', pad='', samples=[0] * 100)

        self.test_traces = []
        for i in range(100):
            dict_trace.update(dict(starttime=i, endtime=i+1))
            self.test_traces.append(Tracebuf2Message.fromDict(dict_trace))
         
    def test_read_write(self):
        for trace in self.test_traces:
            self.tracebuf2.write(trace) 
        
        read_traces = self.tracebuf2.read()

        # Using dicts to make it easier to evaluate equality.        
        test_dict = [Tracebuf2Message.toDict(t) for t in self.test_traces]
        read_dict = [Tracebuf2Message.toDict(t) for t in read_traces]
        self.assertTrue(test_dict == read_dict)


if __name__ == '__main__':
    tracebuf2suite = unittest.TestLoader().loadTestsFromTestCase(TestTracebuf2)
    
    alltests = unittest.TestSuite([tracebuf2suite])
    unittest.TextTestRunner(verbosity=3).run(alltests)
