/**@author Gav Guo
 * @date 2016
 */
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "PyTree.h"
using namespace boost::python;

BOOST_PYTHON_MODULE(PyTree)
{
    class_<bytes>("bytes")
        .def(vector_indexing_suite<bytes>());
    class_<SecureTrie>("SecureTrie", init<PyLevelDB *>())
        .def("getRoot", &SecureTrie::getRoot)
        .def("get", &SecureTrie::get)
        .def("update", &SecureTrie::update)
        .def("remove", &SecureTrie::remove);
}
