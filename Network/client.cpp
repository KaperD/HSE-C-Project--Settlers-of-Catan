#include <memory>
#include <thread>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "catan.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using catan::EventType;
using catan::Void;
using catan::OrderInfo;
using catan::Event;
using catan::Card;
using catan::Dice;
using catan::Market;
using catan::Build;
using catan::Player;

class GameClient {
public:
    GameClient(std::shared_ptr<Channel> channel);


private:
    std::unique_ptr<Catan::Stub> stub_;
};

int main(int argc, char** argv) {

    return 0;
}