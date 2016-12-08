import os

from mpp.models import MPPTestCase
from tinctest.lib import Gpdiff

class check_files(MPPTestCase):
    def verify_results(self, test_name):
        files = []
        isEmpty = False
        isOOM = False
        result = True
        testPass = False
        oom_message = "DETAIL:  VM Protect failed to allocate"

        folder = self._get_absolute_filename('../' + test_name + '/output')
        ans_file = self._get_absolute_filename('../' + test_name + '/' + test_name + '.ans')
        files += [each for each in os.listdir(folder) if each.endswith('.out') and each.startswith('test_')]

        for f in files:
            out_file = self._get_absolute_filename('../' + test_name + '/output/' + f)
            if os.stat(out_file).st_size == 0:
                isEmpty = True
            else:
                output = open(out_file, 'r')
                for line in output:
                    if oom_message in line:
                        isOOM = True 
                        break 
            if isEmpty == False and isOOM == False:
                result = Gpdiff.are_files_equal(out_file, ans_file)
                if result == False:
                    self.test_artifacts.append(out_file.replace('.out', '.diff'))

        # Verify that at least one request fails with OOM and no diffs are generated for queries that returns results
        if (isEmpty == True or isOOM == True) and result == True:
            testPass = True
        else:
            testPass = False      
       
        self.failUnless( testPass == True, 'Test either failed because queries did not fail with OOM or because diff files were found')
