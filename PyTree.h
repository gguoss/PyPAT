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
};

#endif
