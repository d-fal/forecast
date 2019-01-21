#include "cities.h"
#include <iostream>

Cities::Cities() : m_VBox(Gtk::ORIENTATION_VERTICAL),
                   m_HBox(Gtk::ORIENTATION_HORIZONTAL),
                   m_ButtonAdd("Add"),
                   m_Button_Quit("Quit", 1)
{
    set_title("Select city");
    set_border_width(10);
    set_default_size(600, 400);

    add(m_VBox);

    auto infoBarContainer = dynamic_cast<Gtk::Container *>(m_InfoBar.get_content_area());
    if (infoBarContainer)
        infoBarContainer->add(m_Message_Label);

    m_InfoBar.add_button("Understood", 0);
    m_VBox.pack_start(m_InfoBar, Gtk::PACK_SHRINK);
    m_VBox.pack_start(m_HBox, Gtk::PACK_SHRINK);
    /**
     * HBOX - Attach search box
     * */
    m_HBox.pack_start(m_Entry, Gtk::PACK_SHRINK);
    m_HBox.pack_start(m_ButtonAdd, Gtk::PACK_SHRINK);

    m_ScrolledWindow.add(m_TreeView);
    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    m_VBox.pack_start(m_ScrolledWindow);
    m_refTreeModel = Gtk::ListStore::create(m_Columns);

    m_VBox.pack_end(m_ProgressBar, Gtk::PACK_SHRINK, 3);
    m_ProgressBar.set_halign(Gtk::ALIGN_FILL);
    m_ProgressBar.set_valign(Gtk::ALIGN_CENTER);
    m_ProgressBar.set_text("Loading cities");
    m_ProgressBar.set_show_text(true);
    m_VBox.pack_end(m_Button_Quit, Gtk::PACK_SHRINK);
    /**
     * Instantiate Database instance
     * */
    dbInstance = Db::getInstance();

    completion = Gtk::EntryCompletion::create();
    m_Entry.set_completion(completion);

    completion->set_text_column(m_Columns.m_col_name);
    refCompletionModel = Gtk::ListStore::create(m_Columns);
    completion->set_model(refCompletionModel);
    completion->signal_match_selected().connect(sigc::mem_fun(*this, &Cities::match_selected), false);
    m_Entry.signal_key_release_event().connect(sigc::mem_fun(*this, &Cities::on_key_press));
    m_ButtonAdd.signal_clicked().connect(sigc::mem_fun(*this, &Cities::add_button_clicked));

    m_Button_Quit.signal_clicked().connect(sigc::mem_fun(*this,
                                                         &Cities::on_button_quit));
    m_Dispatcher.connect(sigc::mem_fun(*this, &Cities::on_notification_from_worker_thread));
    m_InfoBar.signal_response().connect(sigc::mem_fun(*this, &Cities::on_infobar_response));

    show_all_children();
    /**
     * Hide progressbar
     * */
    m_ProgressBar.hide();
    m_InfoBar.hide();

    // Should load cities from json?
    if (!dbInstance->should_update_cities())
        load_list_cities();
}

Cities::~Cities()
{
}

bool Cities::match_selected(const Gtk::TreeModel::iterator &iter)
{
    Glib::ustring name((*iter)[m_Columns.m_col_name]);
    int id((*iter)[m_Columns.m_col_id]);
    selected_id = id;

    m_Entry.set_text(name);
    return true;
}

void Cities::on_infobar_response(int respones)
{
    m_Message_Label.set_text("");
    m_InfoBar.hide();
}

void Cities::load_list_cities()
{
    m_InfoBar.show();
    m_ProgressBar.show();
    m_Message_Label.set_text("Loading city names, it may take few seconds");
    m_InfoBar.show();
    if (m_WorkerThread)
    {
        std::cout << "Can't start a worker thread while another one is running." << std::endl;
    }
    else
    {
        if (c_jsonParser == nullptr)
            c_jsonParser = new JsonParser();
        // Start a new worker thread.
        m_WorkerThread = new std::thread(
            [this]() {
                c_jsonParser->load_city_names(this);
            });
    }
    c_jsonParser->stop_work();
}
void Cities::on_button_quit()
{

    hide();
}

void Cities::on_button_toggled()
{
}

void Cities::notify()
{

    m_ProgressBar.set_fraction(0.0);
    m_ProgressBar.hide();
    m_Dispatcher.emit();
    dbInstance->update_in_db("Settings", "status=1", "name='cities_loaded'");
    on_infobar_response(3);
}

void Cities::on_notification_from_worker_thread()
{
    if (m_WorkerThread && c_jsonParser->has_stopped())
    {
        if (m_WorkerThread->joinable())
            m_WorkerThread->join();
        delete m_WorkerThread;
        m_WorkerThread = nullptr;
    }
}
void Cities::on_progress(const char *message)
{

    m_ProgressBar.pulse();
    dbInstance->insert_in_db("Cities", message);
}

bool Cities::on_key_press(GdkEventKey *event)
{
    if (!lastItemSearched.compare(m_Entry.get_text()))
    {
        return false;
    }
    refCompletionModel->clear();
    Gtk::TreeModel::Row row;

    std::map<std::string, std::string> map_suggestions;
    map_suggestions = dbInstance->select_from_db("Cities", "id , name, country", m_Entry.get_text().c_str());

    for (const auto &pairs : map_suggestions)
    {
        row = *(refCompletionModel->append());
        row[m_Columns.m_col_id] = std::stoi(pairs.first);
        row[m_Columns.m_col_name] = pairs.second;
    }

    lastItemSearched = m_Entry.get_text();
    return false;
}

void Cities::add_button_clicked()
{
    std::string query;
    query = "(city_id) VALUES (" + std::to_string(selected_id) + ")";
    dbInstance->insert_in_db("Selected_Cities", query.c_str());
    m_TreeView.update_tree_view();
}
