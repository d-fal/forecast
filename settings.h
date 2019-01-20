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

#ifndef Settings_H_
#define Settings_H_

#include <gtkmm.h>
#include <string>
#include "db.h"

class Settings : public Gtk::ApplicationWindow
{
  public:
    Settings();
    ~Settings();

  private:
    Gtk::Grid g_grid;
    Gtk::CheckButton m_checkAutoUpdate;
    Gtk::Entry e_autoUpdateInterval, e_cacheUpdateInterval;
    Gtk::Label l_autoUpdateEnableLabel,l_autoUpdateLabel, l_catchUpdateIntervalLabel;
};

#endif /* SETTINGS_H */
