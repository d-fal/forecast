/* gtkmm example Copyright (C) 2016 gtkmm development team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 1019965016313
 * 117670954
 * 1176
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "skeleteonapplication.h"

#include <iostream>
#include <exception>

#include "cities.h"

SkeletonApplication::SkeletonApplication()
    : Gtk::Application("ir.youconnect.forecast.application", Gio::APPLICATION_HANDLES_OPEN)
{
}

Glib::RefPtr<SkeletonApplication> SkeletonApplication::create()
{

  return Glib::RefPtr<SkeletonApplication>(new SkeletonApplication());
}

MainApp *SkeletonApplication::create_appwindow()
{
  auto appwindow = MainApp::create();
  add_window(*appwindow);

  /**
   * Initialize class instances
   * */
  if (c_Cities == nullptr)
    c_Cities = new Cities();
  if (settings == nullptr)
    settings = new Settings();

  c_Cities->signal_hide().connect(sigc::bind<Gtk::Window *>(sigc::mem_fun(*this,
                                                                          &SkeletonApplication::cities_closed),
                                                            c_Cities));
  appwindow->signal_hide().connect(sigc::bind<Gtk::Window *>(sigc::mem_fun(*this,
                                                                           &SkeletonApplication::on_hide_window),
                                                             appwindow));
  appwindow->signal_window_state_event().connect(sigc::mem_fun(*this,
                                                               &SkeletonApplication::on_focus_window));
  return appwindow;
}

void SkeletonApplication::on_startup()
{
  // Call the base class's implementation.
  Gtk::Application::on_startup();

  // Add actions and keyboard accelerators for the application menu.
  add_action("preferences", sigc::mem_fun(*this, &SkeletonApplication::on_action_preferences));
  add_action("add_city", sigc::mem_fun(*this, &SkeletonApplication::on_add_city));
  add_action("quit", sigc::mem_fun(*this, &SkeletonApplication::on_action_quit));
  set_accel_for_action("app.quit", "<Ctrl>Q");
  set_accel_for_action("app.add_city", "<Ctrl>A");

  auto refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_resource("/ir/youconnect/app_menu.ui");
  }
  catch (const Glib::Error &ex)
  {
    std::cerr << "SkeletonApplication::on_startup(): " << ex.what() << std::endl;
    return;
  }

  auto object = refBuilder->get_object("appmenu");
  auto app_menu = Glib::RefPtr<Gio::MenuModel>::cast_dynamic(object);
  if (app_menu)
    set_app_menu(app_menu);
  else
    std::cerr << "SkeletonApplication::on_startup(): No \"appmenu\" object in app_menu.ui"
              << std::endl;
}

void SkeletonApplication::on_activate()
{
  try
  {
    // The application has been started, so let's show a window.
    auto appwindow = create_appwindow();
    appwindow->present();
  }
  // If create_appwindow() throws an exception (perhaps from Gtk::Builder),
  // no window has been created, no window has been added to the application,
  // and therefore the application will stop running.
  catch (const Glib::Error &ex)
  {
    std::cerr << "SkeletonApplication::on_activate(): " << ex.what() << std::endl;
  }
  catch (const std::exception &ex)
  {
    std::cerr << "SkeletonApplication::on_activate(): " << ex.what() << std::endl;
  }
}

void SkeletonApplication::on_open(const Gio::Application::type_vec_files &files,
                                  const Glib::ustring & /* hint */)
{
  // The application has been asked to open some files,
  // so let's open a new view for each one.
  MainApp *appwindow = nullptr;

  auto windows = get_windows();
  if (windows.size() > 0)
    appwindow = dynamic_cast<MainApp *>(windows[0]);

  try
  {
    if (!appwindow)
      appwindow = create_appwindow();

    for (const auto &file : files)
      appwindow->open_file_view(file);

    appwindow->present();
  }
  catch (const Glib::Error &ex)
  {
    std::cerr << "SkeletonApplication::on_open(): " << ex.what() << std::endl;
  }
  catch (const std::exception &ex)
  {
    std::cerr << "SkeletonApplication::on_open(): " << ex.what() << std::endl;
  }
}

void SkeletonApplication::on_hide_window(Gtk::Window *window)
{
  delete window;
}

void SkeletonApplication::on_action_preferences()
{
  settings->show();
}

void SkeletonApplication::on_action_quit()
{

  auto windows = get_windows();
  for (auto window : windows)
    window->hide();

  quit();
}

void SkeletonApplication::on_add_city()
{
  c_Cities->show();
}

void SkeletonApplication::cities_closed(Gtk::Window *window)
{
  auto windows = get_windows();
  if (windows.size() > 0)
  {
    auto appwindow = dynamic_cast<MainApp *>(windows[0]);
    appwindow->update_cities_list();
  }
}

bool SkeletonApplication::on_focus_window(GdkEventWindowState *event)
{
  auto windows = get_windows();
  if (windows.size() > 0)
  {
    auto appwindow = dynamic_cast<MainApp *>(windows[0]);

    if (event->new_window_state & GDK_WINDOW_STATE_FOCUSED)
    {
      appwindow->disable_animation(false);
      return false;
    }
    else
    {
      appwindow->disable_animation(true);

      return false;
    }
  }
  return false;
}