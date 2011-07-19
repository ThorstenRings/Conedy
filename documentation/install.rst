Installation
=======

Using Conedy’s ubuntu/debian repository
-------------------------------------------

Open /etc/apt/sources.list with an editor of your choice and add a line which is similar to the following::

   deb http://conedy.org/ubuntu/maverick maverick main

If your not using maverick, just replace maverick by your distribution codename in this line. For debian, replace ubuntu by debian, and maverick by your distribution codename (e.g. squeeze).


Add Conedy’s gpg-key to apt with the following command::

#wget http://conedy.org/download/conedy_gpg.pub -O - | sudo apt-key add -


And finally install Conedy via::
#sudo aptitude install conedy python-conedy


Downloading ubuntu/debian packages
----------------------------------

Download the package(s) for your distribution from :ref:`Download`. The deb file (i.e. conedy_0.18_amd64.deb ) can then be installed by::

#dpkg -i conedy_0.18-1_amd64.deb
#apt-get -f install 





Building from source
--------------------




Conedy uses a simple Makefile, which handles compilation. Before building, a file named config.h should be created and filled with some basic configuration variables like installation directories. The file config.h.tmpl defines a template for this configuration file. Conedy does not need to be configured. The configure.sh, which ships with Conedy just print a reminder to the console, that the config.h-file needs to be edited.

The file should define the following variables

- todo

A whitespace seperated list of the targets you want to build. Should be an arbitrary subset of: python-conedy conedy conedy-src documentation
Whenever the makefile targets build install or uninstall are evoqued, all targets in this subset are considered.

- dirinstall

The installation directory for Conedy’s script-interpreter and recompilation scripts. Normally you would want to choose a directory which is in your path.

- dirsrc

A directory for the installation of the Conedy sources.

- noUserSpace

In case you install Conedy's sources and executables into a directory, which is not in userspace, recompilation will require root-priviledges. Choose the tool, which allows to acquire root-priviledges. sudo or kdesudo will work fine. For an installation in userspace noUserSpace kann be left empty. 

- globalConfig

The filename for the config file of Conedy. During installation config.h is copied to this location, which allows you to change variables later.

- addedDir

A directory which should be monitored by Conedy for files which specify new nodes with user-added dynamics.

- numberCores

The number of threads that should be used for recompilation of Conedy’s script interpreter. (similar to make -j).



Dependencies
------------


To install Conedy from source, you will have to satisfy the following dependencies on your system:


-  the gnu scientific library                    
 
   http://www.gnu.org/software/gsl/

-   boost-build, the build sytem of the boost library          
-   boost-iostreams used for automatic compression of file output 

   http://www.boost.org/

-  make
-  c++ compiler


The script interpreter of Conedy  is created with bisonc++ and flex. Therefore these tools need also be installed on your system.
 
-   bisonc++, source code generation for a parser

  http://bisoncpp.sourceforge.net/

-   flex, source code generation for a scanner

  http://flex.sourceforge.net/

   
The python bindings of Conedy additionally depend on

-    boost-python

    http://www.boost.org/

If all dependencies are satisfied, building and installing can be issued by::

   $make; sudo make install

