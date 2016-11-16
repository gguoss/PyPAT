/** @author Gav Guo
 * @date 2016
 */
#ifndef _PY_TREE_
#define _PY_TREE_

#include "libdevcore/TrieDB.h"
#include "libdevcore/OverlayDB.h"

using namespace dev;
using Address = h160;
class SecureTrie {
    OverlayDB m_db;
    SpecificTrieDB<HashedGenericTrieDB<OverlayDB>, Address> m_tree;
public:
    void setRoot(h256 const& _r);
    std::string get(bytes k) const { return m_tree.at(Address(k)); }
    void update(bytes k, bytes value) { m_tree.insert(Address(k), value); }
    void remove(bytes k) { m_tree.remove(Address(k)); }
};

#endif
