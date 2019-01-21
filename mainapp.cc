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

#include "mainapp.h"
#include <iostream>
#include <stdexcept>

using namespace std;

MainApp::MainApp(BaseObjectType *cobject,
                 const Glib::RefPtr<Gtk::Builder> &refBuilder) : Gtk::ApplicationWindow(cobject),
                                                                 m_refBuilder(refBuilder),
                                                                 m_settings(),
                                                                 m_stack(nullptr),
                                                                 m_VBox(Gtk::ORIENTATION_VERTICAL, 0),
                                                                 offset(0),
                                                                 direction(1),
                                                                 continueAnimate(true),
                                                                 autoUpdateInterval(10)
{
  m_refBuilder->get_widget("stack", m_stack);
  if (!m_stack)
    throw std::runtime_error("No \"stack\" object in window.ui");

  dbInstance = Db::getInstance();
  m_Drawing = new Canvas();

  m_refTreeModel = Gtk::ListStore::create(m_Columns);
  m_Combo.set_model(m_refTreeModel);

  m_Combo.pack_start(m_Columns.m_col_name);

  m_Combo.signal_changed().connect(sigc::mem_fun(*this, &MainApp::combo_selection_changed));

  Glib::signal_timeout().connect(sigc::mem_fun(*this, &MainApp::on_timeout), 300);
  Glib::signal_timeout().connect(sigc::mem_fun(*this, &MainApp::auto_update_map), autoUpdateInterval * SEC_IN_MILLISECONDS);
  /**
   * Releasing the cache every 1 hour
   * */
  Glib::signal_timeout().connect(sigc::mem_fun(*this, &MainApp::release_cache), 3600 * SEC_IN_MILLISECONDS);
  m_settings = Gio::Settings::create("ir.youconnect.forecast");
  m_settings->bind("transition", m_stack->property_transition_type());

  m_stack->add(m_VBox);
  /**
   * Adding infobar to show server connection
   * */

  auto infoBarContainer = dynamic_cast<Gtk::Container *>(serverConnectionInfobar.get_content_area());
  if (infoBarContainer)
    infoBarContainer->add(connectionMessage);
  connectionMessage.set_text("Fetching data from server");
  //
  m_VBox.pack_start(m_Combo, Gtk::PACK_SHRINK);
  m_VBox.pack_end(serverConnectionInfobar, Gtk::PACK_SHRINK);

  m_VBox.pack_start(*m_Drawing, Gtk::PACK_SHRINK);
  update_cities_list();
  auto_update_map();
  show_all_children();
}
MainApp::~MainApp()
{
  delete m_Drawing;
  delete dbInstance;
}
//static
MainApp *MainApp::create()
{
  // Load the Builder file and instantiate its widgets.
  auto refBuilder = Gtk::Builder::create_from_resource("/ir/youconnect/window.ui");

  MainApp *window = nullptr;
  refBuilder->get_widget_derived("app_window", window);
  if (!window)
    throw std::runtime_error("No \"app_window\" object in window.ui");

  return window;
}

void MainApp::open_file_view(const Glib::RefPtr<Gio::File> &file)
{
  const auto basename = file->get_basename();

  auto scrolled = Gtk::manage(new Gtk::ScrolledWindow());
  scrolled->set_hexpand(true);
  scrolled->set_vexpand(true);
  scrolled->show();
  auto view = Gtk::manage(new Gtk::TextView());
  view->set_editable(false);
  view->set_cursor_visible(false);
  view->show();
  scrolled->add(*view);
  m_stack->add(*scrolled, basename, basename);

  auto buffer = view->get_buffer();
  try
  {
    char *contents = nullptr;
    gsize length = 0;

    file->load_contents(contents, length);
    buffer->set_text(contents, contents + length);
    g_free(contents);
  }
  catch (const Glib::Error &ex)
  {
    std::cout << "MainApp::open_file_view(\"" << file->get_parse_name()
              << "\"):\n  " << ex.what() << std::endl;
  }

  auto tag = buffer->create_tag();
  m_settings->bind("font", tag->property_font());
  buffer->apply_tag(tag, buffer->begin(), buffer->end());
}
void MainApp::notify_update_map(std::map<std::string, std::vector<std::string>> map_result)
{
  map_receivedWeatherConditions = map_result;
  if (map_allCitiesConditions.find(cityId) == map_allCitiesConditions.end())
  {
    map_allCitiesConditions.insert(std::pair<int, std::map<std::string, std::vector<std::string>>>(cityId, map_result));
  }
  if (map_result.size() > 0)
  {

    m_Drawing->set_input_param(map_result, cityId, cityName);
    m_Drawing->show();

    connectionMessage.set_text("Selected city : " + cityName);
    continueAnimate = true;
  }
  else
  {
    connectionMessage.set_text("Please check your internet connection");
  }
}
bool MainApp::on_timeout()
{
  if (map_receivedWeatherConditions.size() > 0 && continueAnimate)
  {

    if (offset > 10)
      direction = -1;
    else if (offset < 0)
      direction = 1;

    offset += direction;
    m_Drawing->set_input_param(map_receivedWeatherConditions, cityId, cityName);
    m_Drawing->set_animation_param(offset);
    m_Drawing->show();
  }
  return true;
}

void MainApp::update_cities_list()
{
  Gtk::TreeModel::Row row;
  m_refTreeModel->clear();
  std::map<std::string, std::string> selectedCities =
      dbInstance->select_picked_cities_from_db(
          "Selected_Cities", "", "");
  int id = 0;

  citiesList.clear();
  std::map<int, std::string> _pair;
  for (const auto &pairs : selectedCities)
  {
    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_id] = std::stoi(pairs.first);
    row[m_Columns.m_col_name] = pairs.second;
    _pair.insert(std::pair<int, std::string>(std::stoi(pairs.first), pairs.second));
    citiesList.insert(
        std::pair<int, std::map<int, std::string>>(id, _pair));
    _pair.clear();
    id++;
  }
  m_Combo.show();
  if (selectedCities.size() == 0)
    connectionMessage.set_text("Ooops, no cities selected. Please select at least one city from the menu or press Cntrl+A");
}

void MainApp::combo_selection_changed()
{

  Gtk::TreeModel::iterator iter = m_Combo.get_active();

  if (iter)
  {
    Gtk::TreeModel::Row row = *iter;
    if (row)
    {

      cityId = row[m_Columns.m_col_id];
      cityName = Glib::ustring(row[m_Columns.m_col_name]);
      call_forecast_api(cityName, cityId);
    }
  }
}

bool MainApp::auto_update_map()
{
  if (Constants::getInstance()->get_dynamic_update_status())
  {
    if (selectedIndex >= int(citiesList.size()) - 1)
      selectedIndex = ITEM_NOT_SELECTED;

    for (auto const &it : citiesList)
    {

      if (selectedIndex == ITEM_NOT_SELECTED || selectedIndex < it.first)
      {
        cityId = it.second.begin()->first;
        cityName = it.second.begin()->second;
        call_forecast_api(cityName, cityId);
        selectedIndex = it.first;
        break;
      }
    }
  }
  return true;
}

void MainApp::call_forecast_api(const std::string &name, const int &id)
{
  if (map_allCitiesConditions.find(id) != map_allCitiesConditions.end())
  {
    /**
     * In order not to fetch data everytime from server, caching is used
     * The fetched data being stored in map_allCitiesConditions and it will be used everytime 
     * it is needed.
     * */
    notify_update_map(map_allCitiesConditions.at(id));
  }
  else
  {
    std::string url;

    continueAnimate = false;
    Constants::getInstance()->get_url_by_city_id(url, id);
    serverConnectionInfobar.show();
    connectionMessage.set_text("Please wait, we are connecting to get data from server");

    m_WorkerThread = new std::thread(
        [this, url]() {
          api->make_post_request(this, url.c_str());
        });
  }
}
/**
 * Cache will be cleared through calling this method
 * */
bool MainApp::release_cache()
{
  map_allCitiesConditions.clear();
  return true;
}

void MainApp::disable_animation(bool stat)
{
  continueAnimate = !stat;
}