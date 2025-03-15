cdef class PyFarquhar:
     cdef Farquhar* f
     def __cinit__(self):
         self.f = new Farquhar()
         
     def Al(self,T:float,Q:float):
         return self.f.Al(T,Q)

     def GrossCO2assimilation(self,T:float,Q:float):
         return self.f.GrossCO2assimilation(T,Q)
     
     def __dealloc__(self):
         del self.f
                   
