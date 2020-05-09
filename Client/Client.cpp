#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <game.pb.h>
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
using game::NumberOfPlayers;
using game::GameId;
using game::Bool;


GameClient::GameClient(const std::shared_ptr<Channel>& channel)
    : stub_(Network::NewStub(channel)), player_() {
}

//35.193.120.72:80

GameClient::GameClient(bool isLocal) : player_() {
    if (isLocal) {
        stub_ = Network::NewStub(grpc::CreateChannel("localhost:50051",
                                                     grpc::InsecureChannelCredentials()));
    } else {
        stub_ = Network::NewStub(grpc::CreateChannel("209.97.148.147:80",
                                                     grpc::InsecureChannelCredentials()));
    }
}

OrderInfo GameClient::StartNewGame(int numberOfPlayers) {
    ClientContext context;
    NumberOfPlayers players;
    players.set_numberofplayers(numberOfPlayers);
    OrderInfo info;

    Status status = stub_->StartNewGame(&context, players, &info);
    if (!status.ok()) {
        throw NetworkException("Can't start new game");
    }

    player_.set_playerid(info.id());
    player_.set_gameid(info.gameid());

    return info;
}


game::OrderInfo GameClient::JoinGame(int gameId) {
    ClientContext context;
    GameId id;
    id.set_gameid(gameId);
    OrderInfo info;

    Status status = stub_->JoinGame(&context, id, &info);
    if (!status.ok()) {
        throw NetworkException("Can't join game");
    }

    player_.set_playerid(info.id());
    player_.set_gameid(info.gameid());

    return info;
}


void GameClient::SendEvent(Event event) {
    ClientContext context;
    Void empty;
    event.set_playerid(player_.playerid());
    event.set_gameid(player_.gameid());

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

bool GameClient::HasEvent() {
    ClientContext context;
    Bool hasEvent;

    Status status = stub_->HasEvent(&context, player_, &hasEvent);
    if (!status.ok()) {
        throw NetworkException("Can't HasEvent");
    }

    return hasEvent.hasevent();
}

OrderInfo GameClient::ConnectToGame(int type, int val) {
    if (type == 1) {
        OrderInfo info = StartNewGame(val);
        std::cout << "Game was created. Game id is: " << info.gameid() << std::endl;
        return info;
    } else if (type == 2) {
        OrderInfo info = JoinGame(val);
        std::cout << "Your turn is: " << (info.id() + 1) << std::endl;
        return info;
    } else {
        throw NetworkException("Wrong params");
    }
}

