#include <iostream>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "game.grpc.pb.h"
#include "client.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using game::EventType;
using game::Void;
using game::OrderInfo;
using game::Event;
using game::Card;
using game::Dice;
using game::Market;
using game::Build;
using game::Player;
using game::Network;


GameClient::GameClient(std::shared_ptr<Channel> channel)
    : stub_(Network::NewStub(channel)), player() {
}

OrderInfo GameClient::Register() {
    ClientContext context;
    Void empty;
    OrderInfo info;

    Status status = stub_->Register(&context, empty, &info);
    if (!status.ok()) {
        throw NetworkException("Can't Register");
    }

    player.set_playerid(info.id());

    return info;
}

void GameClient::SendEvent(const Event& event) {
    ClientContext context;
    Void empty;

    Status status = stub_->SendEvent(&context, event, &empty);
    if (!status.ok()) {
        throw NetworkException("Can't SendEvent");
    }
}


Event GameClient::GetEvent() {
    ClientContext context;
    Event event;

    Status status = stub_->GetEvent(&context, player, &event);
    if (!status.ok()) {
        throw NetworkException("Can't GetEvent");
    }

    return event;
}


using std::cin;
using std::cout;

int main() {
    GameClient game(grpc::CreateChannel("68.183.30.230:50051",
                          grpc::InsecureChannelCredentials()));
    OrderInfo i = game.Register();
    cout << "My id is: " << i.id() << std::endl;

    int c = 0;
    int type = 0;
    while (cin >> c) {
        if (c == 1) {
            Event event;
            cout << "Type: ";
            cin >> type;
            event.set_type((EventType)type);
            event.set_playerid(i.id());
            game.SendEvent(event);
        } else if (c == 2) {
            Event event = game.GetEvent();
            cout << "Get type: " << event.type() << std::endl;
        }
    }


    return 0;
}