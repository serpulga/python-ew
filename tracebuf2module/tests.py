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
        self.test_other_traces = []
        for i in range(100):
            # Preparing sets of messages to write.
            dict_trace.update(dict(starttime=i, endtime=i+1, sta='BRU2'))
            self.test_traces.append(Tracebuf2Message.fromDict(dict_trace))
            dict_trace.update(dict(sta='BRU3'))
            self.test_other_traces.append(Tracebuf2Message.fromDict(dict_trace))
         
    def test_read_write(self):
        for trace in self.test_traces:
            self.tracebuf2.write(trace) 
        
        read_traces = self.tracebuf2.read()

        # Using dicts to make it easier to evaluate equality.        
        test_dict = [Tracebuf2Message.toDict(t) for t in self.test_traces]
        read_dict = [Tracebuf2Message.toDict(t) for t in read_traces]
        
        # Asserting that what was read it the same as what was written.
        self.assertTrue(test_dict == read_dict)

    def test_read_max(self):
        max_traces = 10
        read_traces = self.tracebuf2.read(max_traces=max_traces)
        
        # Asserting that we didn't get more traces
        # than max_traces.
        self.assertTrue(len(read_traces) <= max_traces)

    def test_read_sta(self):
        for trace in self.test_traces:
            self.tracebuf2.write(trace)
        for other_trace in self.test_other_traces:
            self.tracebuf2.write(other_trace)

        read_traces = self.tracebuf2.read(sta='BRU3')
        read_sta = [t.sta for t in read_traces]

        # Asserting that read_sta only contains 'BRU3'.
        self.assertEqual(read_sta.count('BRU3'), len(read_sta))

if __name__ == '__main__':
    tracebuf2suite = unittest.TestLoader().loadTestsFromTestCase(TestTracebuf2)
    
    alltests = unittest.TestSuite([tracebuf2suite])
    unittest.TextTestRunner(verbosity=3).run(alltests)
