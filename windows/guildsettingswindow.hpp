#pragma once
#include <gtkmm.h>
#include "../discord/snowflake.hpp"
#include "guildsettings/infopane.hpp"

class GuildSettingsWindow : public Gtk::Window {
public:
    GuildSettingsWindow(Snowflake id);

private:
    Gtk::Box m_main;
    Gtk::Stack m_stack;
    Gtk::StackSwitcher m_switcher;

    GuildSettingsInfoPane m_pane_info;

    Snowflake GuildID;
};
