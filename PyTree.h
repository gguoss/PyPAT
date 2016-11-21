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
    SecureTrie(PyObject *db, PyObject *root_hash) : m_db(OverlayDB(((PyLevelDB *)db)->_db)), m_tree(&m_db) {m_tree.setRoot(h256(PyString_AsString(root_hash), h256::FromBinary));}
    SecureTrie(PyLevelDB *db) : m_db(OverlayDB(db->_db)) {}
    SecureTrie(std::string genesis_hash) : m_db(openDB(h256(genesis_hash, h256::FromBinary))), m_tree(&m_db) {m_tree.setRoot(h256(genesis_hash, h256::FromBinary));}
    PyObject* getRoot() const { return PyString_FromStringAndSize((const char *)(m_tree.root().data()), 32); }
    PyObject* get(PyObject *k) const
    { 
        char *data = PyString_AsString(k);
        Py_ssize_t len = PyString_GET_SIZE(k);
        return PyString_FromString(m_tree.at(bytes(data, data + len)).data());
    }
    void update(PyObject *k, PyObject *value)
    { 
        char *k_data = PyString_AsString(k);
        Py_ssize_t k_len = PyString_GET_SIZE(k);
        char *v_data = PyString_AsString(value);
        Py_ssize_t v_len = PyString_GET_SIZE(value);
        m_tree.insert(bytes(k_data, k_data + k_len), bytes(v_data, v_data + v_len));
    }
    void remove(PyObject *k) {
        char *data = PyString_AsString(k);
        Py_ssize_t len = PyString_GET_SIZE(k);
        m_tree.remove(bytes(data, data + len));
    }
    bool root_hash_valid() {return !m_tree.isNull();}
    void commit() { m_db.commit();}
};

#endif
