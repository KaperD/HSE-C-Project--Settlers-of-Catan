#ifndef CLIENT_H
#define CLIENT_H

#include <exception>
#include <string>

#include <grpcpp/create_channel.h>

#include "game.grpc.pb.h"

class NetworkException : public std::exception {
public:
    NetworkException(const std::string& what) : whatStr_(what) { }
    NetworkException(std::string&& what) noexcept : whatStr_(std::move(what)) { }
    const char* what() const noexcept override {
        return whatStr_.c_str();
    }

private:
    std::string whatStr_;
};

class GameClient {
public:
    GameClient(std::shared_ptr<::grpc::Channel> channel);

    game::OrderInfo Register();

    void SendEvent(const ::game::Event& event);

    ::game::Event GetEvent();


private:
    std::unique_ptr<::game::Network::Stub> stub_;
    ::game::Player player;
};

#endif // CLIENT_H