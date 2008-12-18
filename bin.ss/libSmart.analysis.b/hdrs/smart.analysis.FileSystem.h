/*
   Smart.analysis - Libreria para el analisis de depedencias entre componentes software.
   Desarrollado por la division de Nuevos conceptos de Plataformas.
   (c) Copyright 2005-2007 Telefonica Investigacion y Desarrollo. S.A. Unipersonal. 

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/
#ifndef _smart_analysis_FileSystem_h_
#define _smart_analysis_FileSystem_h_

#include <vector>
#include <string>

#include <nemesis.Singleton.h>
#include <nemesis.Mutex.h>
#include <nemesis.Semaphore.h>
#include <nemesis.SortedVector.h>

#include <smart.naming.File.h>

namespace smart {

class Variable;

namespace analysis {

namespace filesystem {
   class Path;
   class Node;
   class FlatEntry;
}

class Scanner;

using namespace nemesis;

class FileSystem : public Singleton <FileSystem> {
   typedef SortedVector <naming::File, naming::File::SortByName, std::string> notfound_container;
   typedef notfound_container::iterator notfound_iterator;
   
public:
   void add (const char* varname) throw (RuntimeException);
   void exclude_add (const char* fullpath) throw ();
   
   void load () throw (RuntimeException);
   filesystem::Path* seek (const char* pathname) throw (RuntimeException); 
   
   void notFound (const naming::File& file) throw () { a_notFound.add (new naming::File (file)); }
   bool wasNotFound (const naming::File& file) const throw () { return a_notFound.contains (&file); }

   const Variable* findVariable (const char* varname) const throw (RuntimeException);   
      
   filesystem::FlatEntry* find (const naming::File&, const filesystem::Path* done) throw (RuntimeException);

   bool exclude (const filesystem::Path*) throw ();
   
   void scannerInitialize () throw (RuntimeException);
   void scannerTerminate () throw (RuntimeException);
   
private:
   typedef std::vector<Variable*> variable_container;
   typedef variable_container::iterator variable_iterator;
   typedef variable_container::const_iterator const_variable_iterator;
   
   typedef std::vector<Scanner*> scanner_container;
   typedef scanner_container::iterator scanner_iterator;
   
   typedef std::vector <std::string> exclude_container;
   typedef exclude_container::iterator exclude_iterator;
   
   scanner_container a_scanners;
   variable_container a_variables;
   notfound_container a_notFound;
   Mutex a_mutex;
   Semaphore a_semaphore;
   int a_counterInitializes;
   int a_counterTerminates;
   bool a_hasExclude;
   exclude_container a_excludes;
      
   scanner_iterator scanner_begin () throw () { return a_scanners.begin (); }
   scanner_iterator scanner_end () throw () { return a_scanners.end (); }   
   int scanner_size () const throw () { return a_scanners.size (); }
   static Scanner* scanner (scanner_iterator& ii) throw () { return *ii; }

   variable_iterator variable_begin () throw () { return a_variables.begin (); }
   variable_iterator variable_end () throw () { return a_variables.end (); }   
   static Variable* variable (variable_iterator& ii) throw () { return *ii; }

   exclude_iterator exclude_begin () throw () { return a_excludes.begin (); }
   exclude_iterator exclude_end () throw () { return a_excludes.end (); }   
   static const std::string& exclude (exclude_iterator& ii) throw () { return *ii; }
   
   const_variable_iterator variable_begin () const throw () { return a_variables.begin (); }
   const_variable_iterator variable_end () const throw () { return a_variables.end (); }   
   static const Variable* variable (const_variable_iterator& ii) throw () { return *ii; }
   
   FileSystem ();
   FileSystem (const FileSystem&);
   void disableDuplicates () throw (RuntimeException);

   friend class Singleton <FileSystem>;
};

}

}

#endif
