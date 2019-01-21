#include "settings.h"
#include "constants.h"

Settings::Settings() : l_autoUpdateEnableLabel("Enable automatic update"),
                       l_degreeSystem("Degrees shown as"),
                       r_fahrenheit("Fahrenheit"),
                       r_celcius("Celcius")
{
    set_title("Settings");
    set_border_width(10);

    add(g_grid);
    g_grid.set_row_homogeneous(true);
    g_grid.set_column_homogeneous(true);

    g_grid.attach(m_checkAutoUpdate, 1, 0, 1, 1);
    g_grid.attach(l_autoUpdateEnableLabel, 0, 0, 1, 1);

    g_grid.attach(l_degreeSystem, 0, 3, 1, 1);

    r_fahrenheit.join_group(r_celcius);
    g_radioGroup.pack_start(r_celcius);
    g_radioGroup.pack_start(r_fahrenheit);

    g_grid.attach(g_radioGroup, 1, 3, 1, 1);
    m_checkAutoUpdate.set_active(true);
    r_celcius.signal_clicked().connect(sigc::mem_fun(*this, &Settings::on_check_auto_update_clicked));

    m_checkAutoUpdate.signal_clicked().connect(sigc::mem_fun(*this, &Settings::on_check_auto_update_clicked));
    show_all_children();
}

Settings::~Settings()
{
}

void Settings::on_check_auto_update_clicked()
{

    Constants::getInstance()->set_dynamic_update_status(m_checkAutoUpdate.get_active());

    if (r_celcius.get_active())
        Constants::getInstance()->set_temprature(CELCIUS);
    if (r_fahrenheit.get_active())
        Constants::getInstance()->set_temprature(FAHRENHEIT);
}