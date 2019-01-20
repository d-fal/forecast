#include "settings.h"

Settings::Settings() : m_checkAutoUpdate("Turn on/off"),
                       l_autoUpdateEnableLabel("Enable automatic update"),
                       l_autoUpdateLabel("Update interval"),
                       l_catchUpdateIntervalLabel("Delete cache after")
{
    set_title("Settings");
    set_border_width(10);

    add(g_grid);
    g_grid.set_row_homogeneous(true);
    g_grid.set_column_homogeneous(true);

    g_grid.attach(m_checkAutoUpdate, 1, 0, 1, 1);
    g_grid.attach(l_autoUpdateEnableLabel, 0, 0, 1, 1);
    g_grid.attach(l_autoUpdateLabel, 0, 1, 1, 1);
    g_grid.attach(l_catchUpdateIntervalLabel, 0, 2, 1, 1);
    g_grid.attach(e_autoUpdateInterval, 1, 1, 1, 1);
    g_grid.attach(e_cacheUpdateInterval, 1, 2, 1, 1);

    show_all_children();
}

Settings::~Settings()
{
}