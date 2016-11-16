/** @author Gav Guo
 * @date 2016
 */
#ifndef _PY_TREE_
#define _PY_TREE_

#include "libdevcore/TrieDB.h"
#include "libdevcore/OverlayDB.h"

typedef struct {
	PyObject_HEAD

	// object is open if all of these are non-null,
	// once an object has been closed, it can not be re-opened
	leveldb::DB* _db;
	leveldb::Options* _options;
	leveldb::Cache* _cache;
	const leveldb::Comparator* _comparator;

	// number of open snapshots, associated with LevelDB object
	int n_snapshots;

	// number of open iterators, associated with LevelDB object
	int n_iterators;
} PyLevelDB;

using namespace dev;
using Address = h160;
class SecureTrie {
    OverlayDB m_db;
    SpecificTrieDB<HashedGenericTrieDB<OverlayDB>, Address> m_tree;
public:
    SecureTrie(PyLevelDB *db) : m_db(OverlayDB(db->_db)), m_tree(&m_db) {}
    bytes getRoot() const { return m_tree.root().asBytes(); };
    std::string get(bytes k) const { return m_tree.at(Address(k)); }
    void update(bytes k, bytes value) { m_tree.insert(Address(k), value); }
    void remove(bytes k) { m_tree.remove(Address(k)); }
};

#endif
