/*
   generate - Version 10.
   Herramienta que crea los archivos de dependencias necesarios para crear de forma optima los ejecutables.

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
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>

#include <nemesis.defines.h>
#include <nemesis.Logger.h>
#include <nemesis.functions.h>

#include <smart.generate.Makefile.h>

#include <smart.description.functions.h>

#include <smart.analysis.filesystem.Path.h>


using namespace std;
using namespace nemesis;
using namespace smart;

SafeRecycler<generate::Makefile> generate::Makefile::st_pool;

void generate::Makefile::debug (const char* file, const int line) 
   throw (RuntimeException)
{
   write ("# ");
   write (file);
   write (" (");
   write (functions::asString (line));
   write (")\n");
}

generate::Makefile* generate::Makefile::create (const analysis::filesystem::Path* path) 
   throw (RuntimeException)
{
   static const int omode = O_WRONLY | O_CREAT | O_TRUNC;
   static const int mask =  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

   Makefile* result (NULL);
   
   result = st_pool.create ();
      
   result->a_path = path;      
   result->a_fullpath = path->getFullPath ();   
   result->a_fullpath += '/';
   result->a_fullpath += description::functions::getMakeName ();
   result->clear ();
   
   const std::string& filename (result->a_fullpath);
   
   LOGINFORMATION (
      string msg ("smart::generate::Makefile::create | Makefile: ");
      msg += filename;
      Logger::information (msg, FILE_LOCATION);
   );
   
   nemesis_signal_shield (result->a_fd, open (filename.c_str (), omode, mask));   
   nemesis_signal_shield (result->a_fd, open (filename.c_str (), omode));   

   if (result->a_fd == -1)
      throw RuntimeException (filename, errno, FILE_LOCATION);   
      
   int opts;
   
   nemesis_signal_shield (opts, fcntl (result->a_fd, F_GETFL));

   opts |= O_NONBLOCK;   

   nemesis_signal_shield (opts, fcntl (result->a_fd, F_SETFL, opts));
   
   return result;
}

generate::Makefile::Makefile () :
   a_fd (-1),
   a_path (NULL)
{
   for (int i = 0; i <= Section::Ending; i ++)
      a_sections [i] = new DataBlock (true);      
   a_activeSection = a_sections [0];
}

void generate::Makefile::clear ()
   throw ()
{
   for (int i = 0; i <= Section::Ending; i ++)
      a_sections [i]->clear ();  
   a_activeSection = a_sections [0];
}

void generate::Makefile::release (Makefile* makefile) 
   throw ()
{
   if (makefile == NULL)
      return;

   if (makefile->a_fd != -1) {
      try { 
         for (int i = 0; i <= Section::Ending; i ++) {
            makefile->write (makefile->a_sections [i]);
            makefile->a_sections [i]->clear ();
         }
      }
      catch (RuntimeException& ex) {
         ex.trace ();
      }
      close (makefile->a_fd);
      makefile->a_fd = -1;
   }      
   
   st_pool.release (makefile);
}

void generate::Makefile::write (const DataBlock* section) const
   throw (RuntimeException)
{
   const char* data = section->getData ();
   const int size = section->getSize ();

   int r (0);
   int nloop (0);
   int left (size);
   register int cx (0);
   bool retry = (left > 0);
   int xerrno;

   while (retry == true) {
      r = ::write (a_fd, data, left);

      if (r < 0) {
         if (errno == EINTR) {
            if (++ cx < 5)
               continue;
         }
         retry = false;
      }
      else if (r <= left) {
         nloop ++;
         data += r;
         if ((left -= r) == 0)                                    // (1)
            retry = false;
         else {
            pollfd waiting;
            waiting.fd = a_fd;
            waiting.events = POLLOUT;
            nemesis_signal_shield (r, poll (&waiting, 1, 100));
            if (r == 0)
               cx = 0;
         }
      }
   }

   if (r < 0)
      throw RuntimeException (a_fullpath, xerrno, FILE_LOCATION);
}

