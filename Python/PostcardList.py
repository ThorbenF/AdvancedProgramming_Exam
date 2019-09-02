import unittest
import datetime  # use this module to deal with dates:  https://docs.python.org/3/library/datetime.html


class PostcardList:
    def __init__(self):
        self._file = ''
        self._postcards = list()
        self._date = dict()
        self._from = dict()
        self._to = dict()

    def readFile(self, filename):
        self._file = filename
        f = open(self._file, 'r')
        self._postcards = f.readlines()
        self.parsePostcards()
        f.close()

    def parsePostcards(self):
        for l, line in enumerate(self._postcards):
            date, sender, receiver = line.split(';', 2)
            date = datetime.datetime.strptime(date[5:], "%Y-%m-%d")
            sender = sender[6:]
            receiver = receiver[4:-2]

            if date not in self._date:
                self._date[date] = [l]
            else:
                self._date[date].append(l)
            if sender not in self._from:
                self._from[sender] = [l]
            else:
                self._from[sender].append(l)
            if receiver not in self._to:
                self._to[receiver] = [l]
            else:
                self._to[receiver].append(l)

    def updateLists(self, filename):
        self._file = filename
        f = open(filename, 'r')
        for l, line in enumerate(f):
            self._postcards.append(line)
        self.parsePostcards()

    def writeFile(self, filename):
        f = open(filename, 'w')
        for elem in self._postcards:
            f.write(elem)
        f.close()

    def updateFile(self, filename):
        f = open(filename, 'a')
        for elem in self._postcards:
            f.write(elem)
        f.close()
        return 0

    def getNumberOfPostcards(self):
        return len(self._postcards)

    def getPostcardsByDateRange(self, date_range):
        postcards_withinrange = list()
        for date in self._date:
            if date_range[0] <= date <= date_range[1]:
                for index in self._date[date]:
                    postcards_withinrange.append(self._postcards[index])
        return postcards_withinrange

    def getPostcardsBySender(self, sender):
        postcards_fromSender = list()
        if sender in self._from:
            for index in self._from[sender]:
                postcards_fromSender.append(self._postcards[index])
        return postcards_fromSender

    def getPostcardsByReceiver(self, receiver):
        postcards_fromReceiver = list()
        if receiver in self._to:
            for index in self._to[receiver]:
                postcards_fromReceiver.append(self._postcards[index])
        return postcards_fromReceiver


class Test(unittest.TestCase):

    @classmethod
    def setUpClass(self):
        self._files = ['./exam_postcard_list{}.txt'.format(i)
                       for i in range(10)]
        self._PstLst = [PostcardList() for f in self._files]
        [p.readFile(f) for f, p in zip(self._files, self._PstLst)]

    def test_missing_attributes(self):
        attributes_to_define = ['_file', '_postcards', '_date', '_from', '_to',
                                'writeFile', 'readFile', 'parsePostcards',
                                'updateFile', 'updateLists',
                                'getNumberOfPostcards',
                                'getPostcardsByDateRange',
                                'getPostcardsBySender',
                                'getPostcardsByReceiver']
        for attr in attributes_to_define:
            if attr not in dir(self._PstLst[0]):
                raise Exception(attr + ' is missing')

    def test_check_getPostcardByDateRange(self):
        dr_test = []
        for i in [0, 1, 4, 6]:
            dr_test.append(
                self._PstLst[i].getPostcardsByDateRange(date_range=(datetime.datetime.strptime('2008-1-1', "%Y-%m-%d"),
                                                                    datetime.datetime.strptime('2010-12-31', "%Y-%m-%d"))))
        self.assertListEqual(sorted(dr_test[0]), sorted(['date:2010-06-23; from:Sneezy; to:Alice;\n', 'date:2009-12-12; from:Dopey; to:Peter;\n',
                                                         'date:2008-03-23; from:Sneezy; to:Pluto;\n', 'date:2008-06-03; from:Goofy; to:Pluto;\n']))
        self.assertEqual(len(dr_test[1]), 1)
        self.assertListEqual(sorted(dr_test[2]), sorted(['date:2010-03-30; from:Louie; to:Sneezy;\n', 'date:2010-03-05; from:Goofy; to:Dopey;\n', 'date:2009-11-08; from:Daisy; to:Minnie;\n',
                                                         'date:2010-07-13; from:Bashful; to:Louie;\n', 'date:2008-06-29; from:Huey; to:Dopey;\n', 'date:2009-01-04; from:Alice; to:Hook;\n', 'date:2009-04-28; from:Bashful; to:Mickey;\n']))
        self.assertEqual(len(dr_test[3]), 7)

    def test_check_getPostcardBySender(self):
        s_test = []
        for i in [2, 3, 5, 7]:
            for sender in ['Daisy', 'Bashful', 'Peter']:
                s_test.append(
                    self._PstLst[i].getPostcardsBySender(sender=sender))
        self.assertIn('date:2011-12-11; from:Daisy; to:Goofy;\n', s_test[0])
        self.assertListEqual(s_test[1], [])
        self.assertEqual(len(s_test[2]), 1)
        self.assertEqual(len(s_test[5]), 0)
        self.assertGreater(len(s_test[8]), len(s_test[9]))

    def test_check_getPostcardByReceiver(self):
        r_test = []
        for i in [0, 3, 8, 9]:
            for receiver in ['Peter', 'Hook', 'Alice', 'SnowWhite']:
                r_test.append(
                    self._PstLst[i].getPostcardsByReceiver(receiver=receiver))
        self.assertIn('date:2009-12-12; from:Dopey; to:Peter;\n', r_test[0])
        self.assertListEqual(
            r_test[1], ['date:2016-10-23; from:Sneezy; to:Hook;\n'])
        self.assertEqual(len(r_test[2]), 2)
        self.assertEqual(len(r_test[6]), 3)

    def test_check_sent_received_when(self):
        srw_test = []
        for ii in range(10):
            for jj in ["Mickey", "Minnie", "Goofy", "Pluto", "Donald", "Daisy", "$crooge", "Huey", "Dewey", "Louie", "Peter", "Hook", "Alice", "SnowWhite", "Doc", "Sleepy", "Sneezy", "Bashful", "Dopey", "Happy", "Grumpy"]:
                try:
                    srw_test.append(self._PstLst[ii]._from[jj])
                    srw_test.append(self._PstLst[ii]._to[jj])
                    srw_test.append(self._PstLst[ii]._date[jj])
                except:
                    pass
        self.assertListEqual(srw_test[0], [1])
        self.assertListEqual(srw_test[130], [14, 25])
        self.assertListEqual(srw_test[138], [10])
        self.assertListEqual(srw_test[140], [15])
        self.assertListEqual(srw_test[192], [13, 26])
        self.assertListEqual(srw_test[203], [6, 9, 11, 12, 24, 31, 42])


if __name__ == '__main__':

    unittest.main()
