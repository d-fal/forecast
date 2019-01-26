#ifndef HISTORY_VIEWER
#define HISTORY_VIEWER

#include "constants.h"
#include "db.h"

class LogChart;

class HistoryViewver : public Gtk::Window
{
  public:
    HistoryViewver();
    virtual ~HistoryViewver();
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

  private:
    void combo_selection_changed();
    void combo_scale_changed();
    void update_cities_list();
    void prepare_logger_data();
    void populate_scale_combo();

    LogChart *logChart = nullptr;
    Gtk::Grid grid;
    ModelColumns m_Columns;
    Gtk::ComboBox m_Combo;
    Gtk::ComboBox m_scaleCombo;
    Gtk::Label l_scaleTitle;
    Gtk::CellRendererText m_cell;
    Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
    Glib::RefPtr<Gtk::ListStore> m_refTreeModel2;
    Db *dbInstance = nullptr;
    int cityId;
    std::string cityName;
    int scale;
    std::vector<int> scaleVec = {1, 2, 4, 8};
};

#endif