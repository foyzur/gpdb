import datetime

import tinctest
import unittest2 as unittest

from mpp.lib.PSQL import PSQL
from tinctest.lib import Gpdiff
from mpp.models import SQLConcurrencyTestCase

class Setup(SQLConcurrencyTestCase):
    pass
