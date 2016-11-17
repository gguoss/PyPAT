/**@author Gav Guo
 * @date 2016
 */
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/filesystem.hpp>
#include "PyTree.h"
using namespace boost::python;

static OverlayDB openDB(h256 const& _genesisHash)
{
	std::string path = "./cppleveldb";

	//path += "/" + toHex(_genesisHash.ref().cropped(0, 4)) + "/" + toString(0.1);
	boost::filesystem::create_directories(path);

	ldb::Options o;
	o.max_open_files = 256;
	o.create_if_missing = true;
	ldb::DB* db = nullptr;
	ldb::Status status = ldb::DB::Open(o, path + "/state", &db);
	if (!status.ok() || !db)
	{
		if (boost::filesystem::space(path + "/state").available < 1024)
		{
			cwarn << "Not enough available space found on hard drive. Please free some up and then re-run. Bailing.";
		}
		else
		{
			cwarn << status.ToString();
			cwarn <<
				"Database " <<
				(path + "/state") <<
				"already open. You appear to have another instance of ethereum running. Bailing.";
		}
	}

	ctrace << "Opened state DB.";
	return OverlayDB(db);
}


BOOST_PYTHON_MODULE(PyTree)
{
    class_<bytes>("bytes")
        .def(vector_indexing_suite<bytes>());
    class_<SecureTrie>("SecureTrie", init<PyObject *, std::string>())
        .def(init<PyLevelDB *>())
        .def(init<std::string>())
        .def("getRoot", &SecureTrie::getRoot)
        .def("get", &SecureTrie::get)
        .def("update", &SecureTrie::update)
        .def("remove", &SecureTrie::remove);
}
