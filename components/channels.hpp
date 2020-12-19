// #pragma once
// #include <gtkmm.h>
// #include <string>
// #include <queue>
// #include <mutex>
// #include <unordered_set>
// #include <unordered_map>
// #include <sigc++/sigc++.h>
// #include "../discord/discord.hpp"

// static const constexpr int ChannelEmojiSize = 16;

// class ChannelListRow : public Gtk::ListBoxRow {
// public:
//     bool IsUserCollapsed;
//     Snowflake ID;
//     std::unordered_set<ChannelListRow *> Children;
//     ChannelListRow *Parent = nullptr;

//     virtual void Collapse();
//     virtual void Expand();

//     static void MakeReadOnly(Gtk::TextView *tv);
// };

// class ChannelListRowDMHeader : public ChannelListRow {
// public:
//     ChannelListRowDMHeader();

// protected:
//     Gtk::EventBox *m_ev;
//     Gtk::Box *m_box;
//     Gtk::Label *m_lbl;
// };

// class ChannelListRowDMChannel : public ChannelListRow {
// public:
//     ChannelListRowDMChannel(const Channel *data);

// protected:
//     void OnImageLoad(Glib::RefPtr<Gdk::Pixbuf> buf);

//     Gtk::EventBox *m_ev;
//     Gtk::Box *m_box;
//     Gtk::TextView *m_lbl;
//     Gtk::Image *m_icon = nullptr;
// };

// class ChannelListRowGuild : public ChannelListRow {
// public:
//     ChannelListRowGuild(const Guild *data);

//     int GuildIndex;

// protected:
//     void OnImageLoad(Glib::RefPtr<Gdk::Pixbuf> buf);

//     Gtk::EventBox *m_ev;
//     Gtk::Box *m_box;
//     Gtk::TextView *m_lbl;
//     Gtk::Image *m_icon;

//     Gtk::Menu m_menu;
//     Gtk::MenuItem *m_menu_copyid;
//     Gtk::MenuItem *m_menu_leave;

// private:
//     typedef sigc::signal<void> type_signal_copy_id;
//     typedef sigc::signal<void> type_signal_leave;

//     type_signal_copy_id m_signal_copy_id;
//     type_signal_leave m_signal_leave;

// public:
//     type_signal_copy_id signal_copy_id();
//     type_signal_leave signal_leave();
// };

// class ChannelListRowCategory : public ChannelListRow {
// public:
//     ChannelListRowCategory(const Channel *data);

//     virtual void Collapse();
//     virtual void Expand();

// protected:
//     Gtk::EventBox *m_ev;
//     Gtk::Box *m_box;
//     Gtk::TextView *m_lbl;
//     Gtk::Arrow *m_arrow;

//     Gtk::Menu m_menu;
//     Gtk::MenuItem *m_menu_copyid;

// private:
//     typedef sigc::signal<void> type_signal_copy_id;

//     type_signal_copy_id m_signal_copy_id;

// public:
//     type_signal_copy_id signal_copy_id();
// };

// class ChannelListRowChannel : public ChannelListRow {
// public:
//     ChannelListRowChannel(const Channel *data);

// protected:
//     Gtk::EventBox *m_ev;
//     Gtk::Box *m_box;
//     Gtk::TextView *m_lbl;

//     Gtk::Menu m_menu;
//     Gtk::MenuItem *m_menu_copyid;

// private:
//     typedef sigc::signal<void> type_signal_copy_id;

//     type_signal_copy_id m_signal_copy_id;

// public:
//     type_signal_copy_id signal_copy_id();
// };

// class ChannelList {
// public:
//     ChannelList();
//     Gtk::Widget *GetRoot() const;
//     void UpdateListing();
//     void UpdateNewGuild(Snowflake id);
//     void UpdateRemoveGuild(Snowflake id);
//     void UpdateRemoveChannel(Snowflake id);
//     void UpdateChannel(Snowflake id);
//     void UpdateCreateDMChannel(Snowflake id);
//     void UpdateCreateChannel(Snowflake id);
//     void UpdateGuild(Snowflake id);
//     void Clear();

//     void SetActiveChannel(Snowflake id);

// protected:
//     Gtk::ListBox *m_list;
//     Gtk::ScrolledWindow *m_main;

//     ChannelListRowDMHeader *m_dm_header_row = nullptr;

//     void CollapseRow(ChannelListRow *row);
//     void ExpandRow(ChannelListRow *row);
//     void DeleteRow(ChannelListRow *row);

//     void UpdateChannelCategory(Snowflake id);

//     void on_row_activated(Gtk::ListBoxRow *row);

//     int m_guild_count;
//     void OnMenuCopyID(Snowflake id);
//     void OnGuildMenuLeave(Snowflake id);

//     Gtk::Menu m_channel_menu;
//     Gtk::MenuItem *m_channel_menu_copyid;

//     Glib::Dispatcher m_update_dispatcher;
//     //mutable std::mutex m_update_mutex;
//     //std::queue<std::unordered_set<Snowflake>> m_update_queue;

//     // i would use one map but in really old guilds there can be a channel w/ same id as the guild so this hacky shit has to do
//     std::unordered_map<Snowflake, ChannelListRow *> m_guild_id_to_row;
//     std::unordered_map<Snowflake, ChannelListRow *> m_id_to_row;
//     std::unordered_map<Snowflake, ChannelListRow *> m_dm_id_to_row;

//     void InsertGuildAt(Snowflake id, int pos);

//     void AddPrivateChannels(); // retard moment
//     void UpdateListingInternal();

//     void CheckBumpDM(Snowflake channel_id);

// public:
//     typedef sigc::signal<void, Snowflake> type_signal_action_channel_item_select;
//     typedef sigc::signal<void, Snowflake> type_signal_action_guild_leave;

//     type_signal_action_channel_item_select signal_action_channel_item_select();
//     type_signal_action_guild_leave signal_action_guild_leave();

// protected:
//     type_signal_action_channel_item_select m_signal_action_channel_item_select;
//     type_signal_action_guild_leave m_signal_action_guild_leave;
// };
