#include "skeleteonapplication.h"
#include <gtkmm/application.h>

int main (int argc, char *argv[])
{
  // Since this example is running uninstalled, we have to help it find its
  // schema. This is *not* necessary in a properly installed application.
  Glib::setenv ("GSETTINGS_SCHEMA_DIR", ".", false);

  auto application = SkeletonApplication::create();

  // Start the application, showing the initial window,
  // and opening extra views for any files that it is asked to open,
  // for instance as a command-line parameter.
  // run() will return when the last window has been closed.
  return application->run(argc, argv);
}