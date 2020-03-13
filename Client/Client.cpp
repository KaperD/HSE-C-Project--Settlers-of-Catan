#include <iostream>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "Client.h"
#include "game.grpc.pb.h"


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
    : stub_(Network::NewStub(channel)), player_() {
}

GameClient::GameClient() : stub_(Network::NewStub(grpc::CreateChannel("68.183.30.230:50051",
                          grpc::InsecureChannelCredentials()))), player_() {

}

OrderInfo GameClient::Register() {
    ClientContext context;
    Void empty;
    OrderInfo info;

    Status status = stub_->Register(&context, empty, &info);
    if (!status.ok()) {
        throw NetworkException("Can't Register");
    }

    player_.set_playerid(info.id());

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

    Status status = stub_->GetEvent(&context, player_, &event);
    if (!status.ok()) {
        throw NetworkException("Can't GetEvent");
    }

    return event;
}

// int main() {
//     GameClient game;

//     OrderInfo info = game.Register();

//     int id = info.id();
//     int command = 0;

//     while (std::cin >> command) {
//         if (command == 1) {
//             Event e;
//             e.set_type(EventType::BUILD);
//             e.set_playerid(id);
//             Build* bi = e.mutable_buildinfo();
//             bi->set_x(id);
//             bi->set_y(id + 1);
//             game.SendEvent(e);
//         } else if (command == 2) {
//             Event e = game.GetEvent();
//             std::cout << "Player " << e.playerid() << ":\n";
//             std::cout << "Event type " << e.type() << ":\n";
//             Build* turn = e.mutable_buildinfo();
//             std::cout << turn->x() << ' ' << turn->y() << '\n';
//         } 

//     }

//     return 0;
// }