#include "friendslist.hpp"
#include "../abaddon.hpp"
#include "lazyimage.hpp"

FriendsList::FriendsList()
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL)
    , m_filter_mode(FILTER_FRIENDS) {
    auto &discord = Abaddon::Get().GetDiscordClient();

    for (const auto &[id, type] : discord.GetRelationships()) {
        const auto user = discord.GetUser(id);
        if (!user.has_value()) continue;
        auto *row = Gtk::manage(new FriendsListFriendRow(type, *user));
        m_list.add(*row);
        row->show();
    }

    constexpr static std::array<const char *, 4> strs = {
        "Friends",
        "Online",
        "Pending",
        "Blocked",
    };
    for (const auto &x : strs) {
        auto *btn = Gtk::manage(new Gtk::RadioButton(m_group, x));
        m_buttons.add(*btn);
        btn->show();
        btn->signal_toggled().connect([this, btn, str = x] {
            if (!btn->get_active()) return;
            switch (str[0]) { // hehe
                case 'F':
                    m_filter_mode = FILTER_FRIENDS;
                    break;
                case 'O':
                    m_filter_mode = FILTER_ONLINE;
                    break;
                case 'P':
                    m_filter_mode = FILTER_PENDING;
                    break;
                case 'B':
                    m_filter_mode = FILTER_BLOCKED;
                    break;
            }
            m_list.invalidate_filter();
        });
    }
    m_buttons.set_homogeneous(true);
    m_buttons.set_halign(Gtk::ALIGN_CENTER);

    m_add.set_halign(Gtk::ALIGN_CENTER);
    m_add.set_margin_top(5);
    m_add.set_margin_bottom(5);

    m_scroll.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    m_list.set_sort_func(sigc::mem_fun(*this, &FriendsList::ListSortFunc));
    m_list.set_filter_func(sigc::mem_fun(*this, &FriendsList::ListFilterFunc));
    m_list.set_selection_mode(Gtk::SELECTION_NONE);
    m_list.set_hexpand(true);
    m_list.set_vexpand(true);
    m_scroll.add(m_list);
    add(m_add);
    add(m_buttons);
    add(m_scroll);

    m_add.show();
    m_scroll.show();
    m_buttons.show();
    m_list.show();
}

int FriendsList::ListSortFunc(Gtk::ListBoxRow *a_, Gtk::ListBoxRow *b_) {
    auto *a = dynamic_cast<FriendsListFriendRow *>(a_);
    auto *b = dynamic_cast<FriendsListFriendRow *>(b_);
    if (a == nullptr || b == nullptr) return 0;
    return a->Name.compare(b->Name);
}

bool FriendsList::ListFilterFunc(Gtk::ListBoxRow *row_) {
    auto *row = dynamic_cast<FriendsListFriendRow *>(row_);
    if (row == nullptr) return false;
    switch (m_filter_mode) {
        case FILTER_FRIENDS:
            return row->Type == RelationshipType::Friend;
        case FILTER_ONLINE:
            return row->Type == RelationshipType::Friend && row->Status != PresenceStatus::Offline;
        case FILTER_PENDING:
            return row->Type == RelationshipType::PendingIncoming || row->Type == RelationshipType::PendingOutgoing;
        case FILTER_BLOCKED:
            return row->Type == RelationshipType::Blocked;
        default:
            return false;
    }
}

FriendsListAddComponent::FriendsListAddComponent()
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL)
    , m_label("Add a Friend", Gtk::ALIGN_START)
    , m_box(Gtk::ORIENTATION_HORIZONTAL)
    , m_add("Add")
    , m_status("", Gtk::ALIGN_START) {
    m_box.add(m_entry);
    m_box.add(m_add);
    m_box.add(m_status);

    m_label.set_halign(Gtk::ALIGN_CENTER);

    m_entry.set_placeholder_text("Enter a Username#1234");

    add(m_label);
    add(m_box);

    show_all_children();
}

FriendsListFriendRow::FriendsListFriendRow(RelationshipType type, const UserData &data)
    : Name(data.Username + "#" + data.Discriminator)
    , Type(type)
    , ID(data.ID)
    , Status(Abaddon::Get().GetDiscordClient().GetUserStatus(data.ID))
    , m_accept("Accept") {
    auto *ev = Gtk::manage(new Gtk::EventBox);
    auto *box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
    auto *img = Gtk::manage(new LazyImage(32, 32, true));
    auto *namelbl = Gtk::manage(new Gtk::Label("", Gtk::ALIGN_START));
    m_status_lbl = Gtk::manage(new Gtk::Label("", Gtk::ALIGN_START));
    auto *lblbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));

    auto &discord = Abaddon::Get().GetDiscordClient();
    discord.signal_presence_update().connect(sigc::mem_fun(*this, &FriendsListFriendRow::OnPresenceUpdate));

    if (data.HasAnimatedAvatar()) {
        img->SetAnimated(true);
        img->SetURL(data.GetAvatarURL("gif", "32"));
    } else {
        img->SetURL(data.GetAvatarURL("png", "32"));
    }

    namelbl->set_markup(data.GetEscapedBoldName());

    UpdatePresenceLabel();

    AddWidgetMenuHandler(ev, m_menu, [this] {
        m_accept.set_visible(Type == RelationshipType::PendingIncoming);

        switch (Type) {
            case RelationshipType::Blocked:
            case RelationshipType::Friend:
                m_remove.set_label("Remove");
                break;
            case RelationshipType::PendingIncoming:
                m_remove.set_label("Ignore");
                break;
            case RelationshipType::PendingOutgoing:
                m_remove.set_label("Cancel");
                break;
            default:
                break;
        }
    });

    m_remove.signal_activate().connect([this] {
        m_signal_remove.emit();
    });

    m_accept.signal_activate().connect([this] {
        m_signal_accept.emit();
    });

    m_menu.append(m_accept);
    m_menu.append(m_remove);
    m_menu.show_all();

    lblbox->set_valign(Gtk::ALIGN_CENTER);

    img->set_margin_end(5);

    lblbox->add(*namelbl);
    lblbox->add(*m_status_lbl);

    box->add(*img);
    box->add(*lblbox);

    ev->add(*box);
    add(*ev);
    show_all_children();
}

void FriendsListFriendRow::UpdatePresenceLabel() {
    switch (Type) {
        case RelationshipType::PendingIncoming:
            m_status_lbl->set_text("Incoming Friend Request");
            break;
        case RelationshipType::PendingOutgoing:
            m_status_lbl->set_text("Outgoing Friend Request");
            break;
        default:
            m_status_lbl->set_text(GetPresenceDisplayString(Status));
            break;
    }
}

void FriendsListFriendRow::OnPresenceUpdate(const UserData &user, PresenceStatus status) {
    if (user.ID != ID) return;
    Status = status;
    UpdatePresenceLabel();
    changed();
}

FriendsListFriendRow::type_signal_remove FriendsListFriendRow::signal_action_remove() {
    return m_signal_remove;
}

FriendsListFriendRow::type_signal_accept FriendsListFriendRow::signal_action_accept() {
    return type_signal_accept();
}

FriendsListWindow::FriendsListWindow() {
    add(m_friends);
    set_default_size(500, 500);
    get_style_context()->add_class("app-window");
    get_style_context()->add_class("app-popup");
    m_friends.show();
}
