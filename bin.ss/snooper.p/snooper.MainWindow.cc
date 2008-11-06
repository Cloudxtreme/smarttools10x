#include <QMessageBox>

#include <nemesis.Logger.h>

#include <nemesis.io.TextReader.h>

#include <nemesis.app.functions.h>

#include <smart.Variable.h>

#include <smart.snooper.MainWindow.h>
#include <smart.snooper.EnvironVariable.h>

#include <smart.structure.Agent.h>
#include <smart.structure.Section.h>

/**
 * MainWindow_ui generado a partir del FORM MainWindow.ui
 */
#include <MainWindow_ui.h>

using namespace std;
using namespace nemesis;
using namespace smart;

/**
 * LA Ui::MainWindow esta generada a partir de un FORM del QT Designer
 */
snooper::MainWindow::MainWindow (QWidget* parent) :
   QMainWindow (parent)
{
   a_gui = new Ui::MainWindow;
   a_gui->setupUi (this);
   
   QObject::connect(
      a_gui->listEnvironVariable, 
      SIGNAL(currentItemChanged (QListWidgetItem*,QListWidgetItem*)), 
      this, 
      SLOT(changeEnvironVariable (QListWidgetItem*,QListWidgetItem*))
   );

   QObject::connect(
      a_gui->q_listMakeVariable, 
      SIGNAL(currentItemChanged (QListWidgetItem*,QListWidgetItem*)), 
      this, 
      SLOT(changeMakefileVariable (QListWidgetItem*,QListWidgetItem*))
   );
   
   QObject::connect(a_gui->action_About, SIGNAL(triggered ()), this, SLOT(about ()));  
}

void snooper::MainWindow::initialize ()  
   throw (RuntimeException)
{
   app::Application& app = app::functions::getApp ();   
   
   QString text = app.getShortName ();
   text += ' ';
   text += app.getVersion ().c_str ();
   setWindowTitle(text);
   
   structure::Agent& structureAgent = structure::Agent::instance ();
   const structure::Section* root = structureAgent.uploadEnvironment ();
   
   forward (root, root);
   
   show ();
}

void snooper::MainWindow::forward (const structure::Section* root, const structure::Section* section)
   throw (RuntimeException)
{
   EnvironVariable* environVariable;
   string vfix;
   string rfix;
   string vpath;
   string fullpath;
   
   vfix = "$";
   vfix += root->getVariable ()->getName ();
   vfix += "/";
   
   rfix = root->getVariable ()->getValue ();
   rfix += '/';
         
   if (root != section) {
      vfix += section->getName ();
      vfix += "/$";
      vfix += section->getVariable ()->getName ();
      vfix += "/";
      
      rfix += section->getName ();
      rfix += '/';
      rfix += section->getVariable ()->getValue ();
      rfix += '/';
   }
      
   vpath = vfix;     
   fullpath = rfix;
   vpath += "this.include";
   fullpath += "this.include";
   environVariable = new EnvironVariable (a_gui->listEnvironVariable, section->getVariable ());
   environVariable->initialize (vpath, fullpath);

   interpret (fullpath);
   
   structure::Section::const_resource_iterator ii, maxii;
   const structure::Resource* resource;   
   
   for (ii = section->resource_begin (), maxii = section->resource_end (); ii != maxii; ii ++) {
      resource = structure::Section::resource (ii);

      if (resource->isASection () == false) {      
         vpath = vfix;
         fullpath = rfix;
         
         vpath += resource->getName ();
         vpath += "/$";
         vpath += resource->getVariable ()->getName ();
         vpath += ".include";   

         fullpath += resource->getName ();
         fullpath += '/';
         fullpath += resource->getVariable ()->getValue ();
         fullpath += ".include";
         
         environVariable = new EnvironVariable (a_gui->listEnvironVariable, resource->getVariable ());
         environVariable->initialize (vpath, fullpath);
         
         interpret (fullpath);
      }
      else
         forward (root, static_cast <const structure::Section*> (resource));
   }
}

void snooper::MainWindow::interpret (const string& fullpath) 
   throw (RuntimeException)
{
   io::TextReader reader;
   char* line;
   char* w;
   char* name;
   char* value;
   MakefileVariable* makefileVariable;
      
   reader.open (fullpath);
      
   while ((line = (char*) reader.fetch ()) != NULL) {
      if ((w = nemesis_strchr (line, '#')) != NULL)
         *w = 0;
      
      if (*line == 0)
         continue;
      
      if ((w = nemesis_strchr (line, '=')) == NULL)
         continue;
      
      *w = 0;
      name = line;
      value = w + 1;
      
      for (w = name; *w != 0; w ++) {
         if (*w == ' ') {
            *w = 0;
            break;
         }
      }

      LOGDEBUG (
         string msg ("smart::snooper::MainWindow::interpret | Name: ");
         msg += name;
         msg += " | Value: ";
         msg += value;
         Logger::debug (msg, FILE_LOCATION);
      );
      
      if ((makefileVariable = find (name)) == NULL) {
         makefileVariable = new MakefileVariable (a_gui->q_listMakeVariable, name);
         a_makefileVariables.add (makefileVariable);
         
         LOGINFORMATION (
            string msg ("smart::snooper::MainWindow::interpret | ");
            msg += makefileVariable->asString ();
            Logger::information (msg, FILE_LOCATION);
         );
      }
      
      makefileVariable->add (value);
   }   
}

// slots
void snooper::MainWindow::changeEnvironVariable (QListWidgetItem* current,QListWidgetItem* previous)
{
   EnvironVariable* environVariable = static_cast <EnvironVariable*> (current);
   
   a_gui->q_lineValue->setText (environVariable->getValue ());
   a_gui->q_lineFileName->setText (environVariable->getFileName ());
   a_gui->q_textContain->setPlainText (environVariable->getContain ());
}

void snooper::MainWindow::changeMakefileVariable (QListWidgetItem* current,QListWidgetItem* previous)
{
   MakefileVariable* makefileVariable = static_cast <MakefileVariable*> (current);
   
   a_gui->q_textMakefileValue->setText (makefileVariable->getValue ());
}

void snooper::MainWindow::about ()
{
   app::Application& app = app::functions::getApp ();   
   
   QString text = app.getShortName ();
   text += ' ';
   text += app.getVersion ().c_str ();
   
   QMessageBox::aboutQt (this, text);
}
