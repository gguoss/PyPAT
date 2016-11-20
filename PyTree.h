/** @author Gav Guo
 * @date 2016
 */
#ifndef _PY_TREE_
#define _PY_TREE_

#include "libdevcore/TrieDB.h"
#include "libdevcore/OverlayDB.h"

using namespace dev;
using Address = h160;

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


static OverlayDB openDB(h256 const& _genesisHash);

class SecureTrie {
    OverlayDB m_db;
    GenericTrieDB<OverlayDB> m_tree;
public:
    SecureTrie(PyObject *db, std::string root_hash) : m_db(OverlayDB(((PyLevelDB *)db)->_db)), m_tree(&m_db) {m_tree.setRoot(h256(root_hash, h256::FromBinary));}
    SecureTrie(PyLevelDB *db) : m_db(OverlayDB(db->_db)) {}
    SecureTrie(std::string genesis_hash) : m_db(openDB(h256(genesis_hash, h256::FromBinary))), m_tree(&m_db) {m_tree.setRoot(h256(genesis_hash, h256::FromBinary));}
    std::string getRoot() const { return m_tree.root().hex(); };
    std::string get(std::string k) const { return m_tree.at(k); }
    void update(std::string k, std::string value) { m_tree.insert(k, value);  std::cout<<"---cpp key:"<<k<<"---cpp value:"<<value<<std::endl;}
    void remove(std::string k) { m_tree.remove(k); }
    bool root_hash_valid() {return !m_tree.isNull();}
    void commit() { m_db.commit();}
};

#endif
