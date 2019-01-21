#ifndef GTKMM_CITIES_H
#define GTKMM_CITIES_H

#include <gtkmm/grid.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/label.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/box.h>
#include <gtkmm/applicationwindow.h>

#include <vector>
#include "jsonparser.h"
#include <thread>
#include "db.h"
#include "treeview_withpopup.h"

class Cities : public Gtk::Window
{
public:
  Cities();
  virtual ~Cities();

  void notify();
  void on_progress(const char *message);

protected:
  //Signal handlers:
  void on_button_quit();
  void on_button_toggled();
  void load_list_cities();
  void on_infobar_response(int response);
  bool on_key_press(GdkEventKey *);
  bool match_selected(const Gtk::TreeModel::iterator &iter);
  
  void add_button_clicked();
 
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:
    ModelColumns()
    {
      add(m_col_id);
      add(m_col_name);
      add(m_col_itemchosen), add(m_col_choices);
    }
    Gtk::TreeModelColumn<int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    Gtk::TreeModelColumn<Glib::ustring> m_col_itemchosen;
    Gtk::TreeModelColumn<Glib::RefPtr<Gtk::TreeModel>> m_col_choices;
  };


private:
  double progressFraction;
  ModelColumns m_Columns;
  typedef std::map<int, Glib::ustring> type_actions_map;
  type_actions_map m_CompletionActions;
  int offset;
  JsonParser *c_jsonParser = nullptr;

  Glib::Dispatcher m_Dispatcher;
  std::thread *m_WorkerThread = nullptr;

  void on_notification_from_worker_thread();
  Gtk::Box m_VBox, m_HBox;
  Gtk::InfoBar m_InfoBar;
  Gtk::Label m_Message_Label;

  
  Gtk::Entry m_Entry;
  Db *dbInstance = nullptr;
  Gtk::ProgressBar m_ProgressBar;

  Glib::RefPtr<Gtk::EntryCompletion> completion;
  Glib::RefPtr<Gtk::ListStore> refCompletionModel;
  Glib::ustring lastItemSearched;

  Gtk::ScrolledWindow m_ScrolledWindow;
  
  Glib::RefPtr<Gtk::ListStore> m_refTreeModel;


  Gtk::ButtonBox m_ButtonBox;
  Gtk::Button m_ButtonAdd,m_Button_Quit;

  int selected_id;

  TreeView_WithPopup m_TreeView;

};

#endif //GTKMM_CITIES_H