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

#ifndef GTKMM_MainApp_H_
#define GTKMM_MainApp_H_

#define SEC_IN_MILLISECONDS 1000
#define ITEM_NOT_SELECTED -1

#include <gtkmm/stack.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/box.h>
#include <gtkmm/applicationwindow.h>
#include "canvas.h"
#include <thread>
#include <mutex>
#include <string>
#include "forecastapi.h"
#include <mutex>
#include "db.h"

class MainApp : public Gtk::ApplicationWindow
{
public:
  MainApp(BaseObjectType *cobject,
          const Glib::RefPtr<Gtk::Builder> &refBuilder);
  ~MainApp();
  static MainApp *create();

  void open_file_view(const Glib::RefPtr<Gio::File> &file);
  void notify_update_map(std::map<std::string, std::vector<std::string>> map_result);
  bool on_timeout();
  void update_cities_list();
  void combo_selection_changed();
  bool auto_update_map();
  void call_forecast_api(const std::string &name, const int &id);
  bool release_cache();
  void disable_animation(bool stat);
  void store_temperature_sample(const int &id, const std::map<std::string, std::vector<std::string>> &map_result);

  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:
    ModelColumns()
    {
      add(m_col_id);
      add(m_col_name);
      add(m_col_ref_id);
    }

    Gtk::TreeModelColumn<int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    Gtk::TreeModelColumn<int> m_col_ref_id;
  };

protected:
  Glib::RefPtr<Gtk::Builder> m_refBuilder;
  Glib::RefPtr<Gio::Settings> m_settings;
  Gtk::Stack *m_stack;
  Canvas *m_Drawing = nullptr;
  Gtk::Box m_VBox;
  std::thread *m_WorkerThread;
  ForecastAPI *api = nullptr;
  int offset, direction;
  int cityId;
  std::string cityName;

private:
  std::map<std::string, std::vector<std::string>> map_receivedWeatherConditions;
  std::map<int, std::map<std::string, std::vector<std::string>>> map_allCitiesConditions;
  ModelColumns m_Columns;
  Gtk::ComboBox m_Combo;
  Gtk::CellRendererText m_cell;
  Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
  Db *dbInstance;
  Gtk::InfoBar serverConnectionInfobar;
  Gtk::Label connectionMessage;
  bool continueAnimate;
  int autoUpdateInterval;
  std::map<int, std::map<int, std::string>> citiesList;
  int selectedIndex;
};

#endif /* GTKMM_MainApp_H */
