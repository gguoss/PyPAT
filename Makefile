# location of the Python header files
#  
PYTHON_VERSION = 2.7
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
LIBS = -L./libdevcore/build/ -lpython$(PYTHON_VERSION) -ldevcore -L./usr/lib/x86_64-linux-gnu/ -lleveldb
 
BOOST_INC = -I/usr/include
BOOST_LIB = -L/usr/lib -L/usr/local/lib/stage/lib/ -lboost_thread -lboost_random -lboost_filesystem -lboost_system -lboost_python -lpthread
INCLUDE = -I./libdevcore/ -I.
CFLAG = -std=c++11 -fPIC -c
  
TARGET = PyTree
$(TARGET).so: $(TARGET).o
	g++ -shared -Wl,--export-dynamic $(TARGET).o $(LIBS) $(BOOST_LIB) -o $(TARGET).so

$(TARGET).o: $(TARGET).cpp
	g++ -I$(PYTHON_INCLUDE) $(BOOST_INC) $(INCLUDE) $(CFLAG) $(TARGET).cpp

test:
	g++ $(INCLUDE) -std=c++11 test.cpp $(LIBS) $(BOOST_LIB)
clean:
	rm *\.so *\.o
