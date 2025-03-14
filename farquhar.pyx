cdef class PyFarquhar:
     cdef Farquhar* f
     def __cinit__(self):
         self.f = new Farquhar()
         
     def Al(self,T:float,Q:float):
         return self.f.Al(T,Q)

     def CO2assimilation(self,T:float,Q:float):
         return self.f.CO2assimilation(T,Q)
     
     def __dealloc__(self):
         del self.f
                   
