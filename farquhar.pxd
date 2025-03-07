# distutils language = c++

cdef extern from "Farquhar.cc":
     pass

cdef extern from "Farquhar.h" namespace "LignumTLS":
     cdef cppclass Farquhar:
          Farquhar() except +
          double Al(double T, double Q)const
