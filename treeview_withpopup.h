//$Id: treeview_withpopup.h 705 2006-07-19 02:55:32Z jjongsma $ -*- c++ -*-

/* gtkmm example Copyright (C) 2002 gtkmm development team
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef GTKMM_EXAMPLE_TREEVIEW_WITHPOPUP_H
#define GTKMM_EXAMPLE_TREEVIEW_WITHPOPUP_H

#include <gtkmm/treeview.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/menu.h>
#include <gtkmm/liststore.h>
#include "db.h"

class TreeView_WithPopup : public Gtk::TreeView
{
public:
  TreeView_WithPopup();
  virtual ~TreeView_WithPopup();
  void update_tree_view();

protected:
  // Override Signal handler:
  // Alternatively, use signal_button_press_event().connect_notify()
  bool on_button_press_event(GdkEventButton *button_event) override;

  //Signal handler for popup menu items:
  void on_menu_file_popup_generic();

  //Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:
    ModelColumns()
    {
      add(m_col_id);
      add(m_col_name);
    }

    Gtk::TreeModelColumn<unsigned int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
  };

  ModelColumns m_Columns;

  //The Tree model:
  Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

  Gtk::Menu m_Menu_Popup;
  Db* dbInstance;
};

#endif //GTKMM_EXAMPLE_TREEVIEW_WITHPOPUP_H
