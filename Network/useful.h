class OrderInfo : public Message {
public:
    // получить свой номер
    int32 id() const;
    // установить значение
    void set_id(int32 value);

    // получить число игроков
    int32 numberofplayers() const;
    // установить значение 
    void set_numberofplayers(int32 value);

private:
    int32 id_;
    int32 numberofplayers_;
};

class Event : public Message {
public:
    // Чтобы получить доступ с полю
    const ::game::Dice& diceinfo() const;
    // Чтобы изметить, нужно 

private:
    union EventInfoUnion {
    EventInfoUnion() {}
        ::game::Card* cardinfo_;
        ::game::Dice* diceinfo_;
        ::game::Market* marketinfo_;
        ::game::Build* buildinfo_;
    } EventInfo_;

};