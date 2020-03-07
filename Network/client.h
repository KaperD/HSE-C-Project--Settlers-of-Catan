#ifndef CLIENT_H
#define CLIENT_H

#include <grpcpp/create_channel.h>
#include "game.grpc.pb.h"

class GameClient {
public:
    GameClient(std::shared_ptr<Channel> channel);

    game::OrderInfo Register();

    void SendEvent(const ::game::Event& event);

    void GetEvents(const ::game::Player& player);


private:
    std::unique_ptr<::game::Network::Stub> stub_;
};

#endif // CLIENT_H