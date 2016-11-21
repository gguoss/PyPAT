import os
import ethereum.testutils as testutils
import json
import ethereum.trie as trie
import ethereum.db as db
import ethereum.utils as utils
import itertools
from ethereum.slogging import get_logger
from rlp.utils import decode_hex, encode_hex
from ethereum.abi import is_string
from ethereum.testutils import fixture_to_bytes
logger = get_logger()
import leveldb
import PyTree
import rlp

# customize VM log output to your needs
# hint: use 'py.test' with the '-s' option to dump logs to the console
# configure_logging(':trace')


def check_testdata(data_keys, expected_keys):
    assert set(data_keys) == set(expected_keys), \
        "test data changed, please adjust tests"

fixture_path = os.path.join(os.path.dirname(__file__))


def load_tests():
    fixture = {}
    testdir = os.path.join(fixture_path, 'TrieTests')
    for f in os.listdir(testdir):
        if f != 'trietest.json':
            continue
        sub_fixture = json.load(open(os.path.join(testdir, f)))
        for k, v in sub_fixture.items():
            fixture[f + "_" + k] = v
    return fixture_to_bytes(fixture)


def run_test(name, pairs):

    logger.debug('testing %s' % name)

    ldb = leveldb.LevelDB("./db")
    def _dec(x):
        if is_string(x) and x.startswith(b'0x'):
            return decode_hex(x[2:])
        return x

    pairs['in'] = [(_dec(k), _dec(v)) for k, v in pairs['in']]
    deletes = [(k, v) for k, v in pairs['in'] if v is None]

    N_PERMUTATIONS = 1000
    for i, permut in enumerate(itertools.permutations(pairs['in'])):
        if i > N_PERMUTATIONS:
            break
        #t1 = trie.Trie(db.EphemDB())
        rlpnull = utils.sha3rlp(b'')
        t = PyTree.SecureTrie(ldb, rlpnull)
        print "SecureTrie root hash:" + t.getRoot()
        #print "python Trie root hash:" + encode_hex(t1.root_hash)
        for k, v in permut:
            #print 'updating with k:%s v:' %(encode_hex(k))
            if v is not None:
                #t1.update(str(k.encode('utf-8')), str(v.encode('utf-8')))
                #print "python Trie root hash:" + encode_hex(t1.root_hash)
                t.update(str(k.encode('utf-8')), str(v.encode('utf-8')))
                print "SecureTrie root hash:" + t.getRoot()
            else:
                t.delete(str(k.encode('utf-8')))
                #t1.delete(str(k.encode('utf-8')))
        # make sure we have deletes at the end
        for k, v in deletes:
            t.delete(str(k.encode('utf-8')))
        print "root_hash:" + t.getRoot()
        print "pairs['root']" + pairs['root']
        if pairs['root'] != b'0x' + t.getRoot():
            raise Exception("Mismatch: %r %r %r %r" % (
                name, pairs['root'], b'0x' + t.getRoot(), (i, list(permut) + deletes)))


if __name__ == '__main__':
    for name, pairs in load_tests().items():
        print "name:"
        print name
        print "pairs:"
        print pairs
        run_test(name, pairs)
else:
    for key, pairs in load_tests().items():
        globals()["test_" + key] = lambda: run_test(key, pairs)
