/**@author Gav Guo
 * @date 2016
 */

#include <boost/python.hpp>
#include "PyTree.h"
using namespace boost::python;

void SecureTrie::setRoot(h256 const& _r)
{
    m_tree.setRoot(_r);
}

BOOST_PYTHON_MODULE(PyTree)
{
    class_<SecureTrie>("SecureTrie")
        .def("setRoot", &SecureTrie::setRoot);
}
