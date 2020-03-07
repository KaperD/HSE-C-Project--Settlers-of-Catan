#ifndef CLIENT_H
#define CLIENT_H

#include <grpcpp/create_channel.h>
#include "catan.grpc.pb.h"

class GameClient {
public:
    GameClient(std::shared_ptr<Channel> channel);

    catan::OrderInfo Register();

    void SendEvent(const ::catan::Event& event);

    void GetEvents(const ::catan::Player& player);


private:
    std::unique_ptr<Catan::Stub> stub_;
};

#endif // CLIENT_H