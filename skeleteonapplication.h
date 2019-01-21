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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

/**
 * As suggested in documentations, it's better to override basic functions inherited from the Gtk::Application
 * */
#ifndef SKELETON_APPLICATION_H
#define SKELETON_APPLICATION_H


#include <gtkmm/application.h>
#include "mainapp.h"
#include "db.h"
#include "settings.h"

class MainApp;
class Cities;

class SkeletonApplication : public Gtk::Application
{

public:
  SkeletonApplication();
  static Glib::RefPtr<SkeletonApplication> create();

protected:
  // Override default signal handlers:
  void on_startup() override;
  void on_activate() override;
  void on_open(const Gio::Application::type_vec_files &files,
               const Glib::ustring &hint) override;

private:
  void cities_closed(Gtk::Window *window);
  MainApp *create_appwindow();
  void on_hide_window(Gtk::Window *window);
  void on_action_preferences();
  void on_action_quit();
  void on_add_city();
  bool on_focus_window(GdkEventWindowState *event);
  Cities *c_Cities = nullptr;
  Db *dbInstance = nullptr;
  Settings *settings = nullptr;
};

#endif /* SKELETON_APPLICATION_H */