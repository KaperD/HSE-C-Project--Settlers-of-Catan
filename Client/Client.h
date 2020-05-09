#ifndef CLIENT_H
#define CLIENT_H

#include <exception>
#include <string>
#include <utility>

#include <grpcpp/create_channel.h>

#include "game.grpc.pb.h"

class NetworkException : public std::exception {
public:
    explicit NetworkException(std::string  what) : whatStr_(std::move(what)) { }
    [[nodiscard]] const char* what() const noexcept override {
        return whatStr_.c_str();
    }

private:
    std::string whatStr_;
};

class GameClient {
public:
    GameClient(bool isLocal);
    explicit GameClient(const std::shared_ptr<::grpc::Channel>& channel);

    game::OrderInfo StartNewGame(int numberOfPlayers);

    game::OrderInfo JoinGame(int gameId);

    void SendEvent(::game::Event event);

    ::game::Event GetEvent();

    bool HasEvent();


private:
    std::unique_ptr<::game::Network::Stub> stub_;
    ::game::Player player_;
};

#endif // CLIENT_H