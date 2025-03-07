from setuptools import Extension, setup
from Cython.Build import cythonize

sourcels=["farquhar.pyx"]

setup(name="farquhar",
      ext_modules = cythonize(
          Extension(
              name="farquhar",
              include_dirs=["include","src/"],
              sources=sourcels,
              language="c++",
       ),
          compiler_directives={"language_level":"3str"}
      ))
