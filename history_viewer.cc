#include "history_viewer.h"
#include "log_chart.h"

HistoryViewver::HistoryViewver()
{

    add(grid);

    dbInstance = Db::getInstance();

    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    m_Combo.set_model(m_refTreeModel);

    m_Combo.pack_start(m_Columns.m_col_name);

    m_Combo.signal_changed().connect(sigc::mem_fun(*this, &HistoryViewver::combo_selection_changed));

    logChart = new LogChart();

    grid.attach(*logChart, 0, 1, 1, 1);
    grid.attach(m_Combo, 0, 0, 1, 1);

    update_cities_list();
    show_all_children();
}

HistoryViewver::~HistoryViewver()
{
}

void HistoryViewver::combo_selection_changed()
{
    Gtk::TreeModel::iterator iter = m_Combo.get_active();
    if (iter)
    {
        Gtk::TreeModel::Row row = *iter;
        if (row)
        {
            cityId = row[m_Columns.m_col_id];
            cityName = Glib::ustring(row[m_Columns.m_col_name]);
            prepare_logger_data(cityId, cityName);
        }
    }
}
void HistoryViewver::prepare_logger_data(int &cityId, std::string &cityName)
{
    std::map<std::string, std::string> loggedTemperatureMap;
    loggedTemperatureMap = dbInstance->get_temperature_history(cityId, 0);
    logChart->set_logger_data(loggedTemperatureMap, cityName);
    logChart->show();
}

void HistoryViewver::update_cities_list()
{
    Gtk::TreeModel::Row row;
    m_refTreeModel->clear();
    std::map<std::string, std::string> selectedCities =
        dbInstance->select_picked_cities_from_db(
            "Selected_Cities", "", "");

    std::map<int, std::string> _pair;
    for (const auto &pairs : selectedCities)
    {

        row = *(m_refTreeModel->append());
        row[m_Columns.m_col_id] = std::stoi(pairs.first);
        row[m_Columns.m_col_name] = pairs.second;
    }
    // m_Combo.show();
}