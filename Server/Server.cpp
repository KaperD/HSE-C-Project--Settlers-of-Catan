#include <iostream>
#include <mutex>
#include <queue>
#include <exception>
#include <condition_variable>
#include <atomic>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include "game.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
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

class EventQueue final {
public:
    EventQueue() : hasElement_(false) { }

    void push(Event event) {
        std::unique_lock<std::mutex> locker(lockqueue_);
        events_.push(std::move(event));
        hasElement_.store(true);
        queuecheck_.notify_all();
    }

    Event front() {
        std::unique_lock<std::mutex> locker(lockqueue_);
        while (!hasElement_.load()) {
            queuecheck_.wait(locker);
        }
        if (events_.size() > 0) {
            Event event = events_.front();
            events_.pop();
            if (events_.size() == 0) {
                hasElement_.store(false);
            }
            return event;
        } else {
            std::cerr << "front()" << std::endl;
            throw (std::logic_error("front()"));
        }
    }

    void clear() {
        std::unique_lock<std::mutex> locker(lockqueue_);
        while (events_.size() > 0) {
            events_.pop();
        }
    }




private:
    std::queue<Event> events_;
    std::mutex lockqueue_;
    std::condition_variable queuecheck_;
    std::atomic_bool hasElement_;
};
 

class GameServerImpl final : public Network::Service {
public:
    GameServerImpl() : events_(4), currentPlayerNumber_(0) { }
private:
    Status Register(::grpc::ServerContext* context, const Void* request, OrderInfo* response) override {
        int id = currentPlayerNumber_++;
        if (id > 2) {
            id %= 3;
            events_[id].clear();
        }
        id %= 3;
        response->set_id(id);
        response->set_numberofplayers(3);

        return Status::OK;
    }

    Status SendEvent(::grpc::ServerContext* context, const Event* request, Void* response) override {
        Event event = *request;
        int playerid = event.playerid();
        for (int k = 0; k < 3; ++k) {
            if (k == playerid) continue;
            events_[k].push(event);
        }
    
        return Status::OK;
    }

    Status GetEvent(ServerContext* context, const Player* request, Event* response) override { 
        int playerid = request->playerid();
        Event event = events_[playerid].front();
        *response = std::move(event);

        return Status::OK;
    }
  
private:
    std::vector<EventQueue> events_;
    std::atomic_int currentPlayerNumber_;
};

 
void RunServer() {
  std::string server_address("68.183.30.230:50051");
  GameServerImpl service;

  ServerBuilder builder;

  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  server->Wait();
}

int main() {
    RunServer();
    
     
    return 0;
}