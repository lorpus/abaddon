#pragma once
#include "../components/channels.hpp"
#include "../components/chatwindow.hpp"
#include "../components/memberlist.hpp"
#include <gtkmm.h>

class MainWindow : public Gtk::Window {
public:
    MainWindow();

    void UpdateComponents();
    void UpdateMembers();
    void UpdateChannelListing();
    void UpdateChatWindowContents();
    void UpdateChatActiveChannel(Snowflake id);
    Snowflake GetChatActiveChannel() const;
    void UpdateChatNewMessage(Snowflake id);
    void UpdateChatMessageDeleted(Snowflake id, Snowflake channel_id);
    void UpdateChatMessageEditContent(Snowflake id, Snowflake channel_id);
    void UpdateChatPrependHistory(const std::vector<Snowflake> &msgs);
    void InsertChatInput(std::string text);

    ChannelList *GetChannelList();
    ChatWindow *GetChatWindow();
    MemberList *GetMemberList();

public:
    typedef sigc::signal<void> type_signal_action_connect;
    typedef sigc::signal<void> type_signal_action_disconnect;
    typedef sigc::signal<void> type_signal_action_set_token;
    typedef sigc::signal<void> type_signal_action_reload_css;

    type_signal_action_connect signal_action_connect();
    type_signal_action_disconnect signal_action_disconnect();
    type_signal_action_set_token signal_action_set_token();
    type_signal_action_reload_css signal_action_reload_css();

protected:
    type_signal_action_connect m_signal_action_connect;
    type_signal_action_disconnect m_signal_action_disconnect;
    type_signal_action_set_token m_signal_action_set_token;
    type_signal_action_reload_css m_signal_action_reload_css;

protected:
    Gtk::Box m_main_box;
    Gtk::Box m_content_box;
    Gtk::Paned m_chan_chat_paned;
    Gtk::Paned m_chat_members_paned;

    ChannelList m_channel_list;
    ChatWindow m_chat;
    MemberList m_members;

    Gtk::MenuBar m_menu_bar;
    Gtk::MenuItem m_menu_discord;
    Gtk::Menu m_menu_discord_sub;
    Gtk::MenuItem m_menu_discord_connect;
    Gtk::MenuItem m_menu_discord_disconnect;
    Gtk::MenuItem m_menu_discord_set_token;

    Gtk::MenuItem m_menu_file;
    Gtk::Menu m_menu_file_sub;
    Gtk::MenuItem m_menu_file_reload_css;
};
