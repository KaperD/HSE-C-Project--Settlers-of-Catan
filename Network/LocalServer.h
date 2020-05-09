#include <iostream>
#include <exception>
#include <chrono>
#include <thread>
#include <mutex>
#include <set>
#include <queue>
#include <string>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include "game.grpc.pb.h"
#include "EventQueue.h"
#include "spinlock.h"

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
using game::GameId;
using game::NumberOfPlayers;
using game::Bool;


namespace {

constexpr int MaximumNumberOfGames = 100;
constexpr int MaximumNumberOfPlayers = 4;

} // namespace


struct Game {
    Game() : events_(MaximumNumberOfPlayers) { }
    Game(const Game&) = delete;
    Game& operator= (const Game&) = delete;

    void restart() {
        for (int k = 0; k < MaximumNumberOfPlayers; ++k) {
            events_[k].clear();
        }
        numberOfPlayers = 0;
        activePlayers.store(0);
    }


    std::vector<utility::EventQueue> events_;
    int numberOfPlayers = 0;
    std::atomic_int activePlayers { 0 };
    utility::spinlock spin;
};


class GameServerImpl final : public Network::Service {
public:
    GameServerImpl() : games(MaximumNumberOfGames) {
        for (int k = 0; k < MaximumNumberOfGames; ++k) {
            availableIds.insert(k);
        }
    }

private:
    Status StartNewGame(::grpc::ServerContext* context, const NumberOfPlayers* request, OrderInfo* response) override {
        std::cout << "New game " << request->numberofplayers() << std::endl;
        std::lock_guard<utility::spinlock> lock(spin);

        ++numberOfMadeGames;
        if (numberOfMadeGames >= 90) {
            numberOfMadeGames = 0;
            for (int k = 0; k < MaximumNumberOfGames; ++k) {
                if (games[k].activePlayers.load() == 0) {
                    games[k].restart();
                    availableIds.insert(k);
                }
            }
        }

        int newid = *availableIds.begin();
        availableIds.erase(availableIds.begin());

        games[newid].activePlayers.store(1);
        std::cout << "New game active: " << games[newid].activePlayers.load() << ' ' << games[newid].activePlayers.load() << std::endl;
        games[newid].numberOfPlayers = request->numberofplayers();

        response->set_numberofplayers(request->numberofplayers());
        response->set_id(0);
        std::cout << "ID " << newid << std::endl;
        response->set_gameid(newid);

        return Status::OK;
    }


    Status JoinGame(::grpc::ServerContext* context, const GameId* request, OrderInfo* response) override {
        Game& game = games.at(request->gameid());
        std::lock_guard<utility::spinlock> lock(game.spin);


        if (game.activePlayers.load() == game.numberOfPlayers || game.activePlayers.load() == 0) {
            std::cout << "Bad join" << std::endl;
            return Status::CANCELLED;
        }
        std::cout << "Active: " << game.activePlayers.load() << std::endl;

        response->set_id(game.activePlayers++);
        response->set_gameid(request->gameid());
        response->set_numberofplayers(game.numberOfPlayers);

        return Status::OK;
    }


    Status SendEvent(::grpc::ServerContext* context, const Event* request, Void* response) override {
        Event event = *request;
        int playerid = event.playerid();
        std::cout << "SendEvent gameid " <<  request->gameid() << std::endl;
        Game& game = games.at(event.gameid());

        for (int k = 0; k < 3; ++k) {
            if (k == playerid) continue;
            game.events_[k].push(event);
        }
        if (event.type() == EventType::ENDGAME) {
            std::cout << event.gameid() << std::endl;
            std::cout << "SendEvent ENDGAME " <<  game.activePlayers.load() << ' ';
            --game.activePlayers;
            std::cout << game.activePlayers.load() << std::endl;
        }
        std::cout << playerid << ' ' << event.type() << std::endl;
        return Status::OK;
    }


    Status GetEvent(ServerContext* context, const Player* request, Event* response) override {
        int playerid = request->playerid();
        std::cout << "GetEvent gameid " <<  request->gameid() << std::endl;
        std::cout << "GetEvent playerid " <<  playerid << std::endl;
        Game& game = games.at(request->gameid());

        Event event = game.events_[playerid].front();

        if (event.type() == EventType::ENDGAME) {
            std::cout << "GetEvent ENDGAME " <<  game.activePlayers.load() << ' ';
            --game.activePlayers;
            std::cout << game.activePlayers.load() << std::endl;
        }

        *response = std::move(event);

        return Status::OK;
    }


    Status HasEvent(ServerContext* context, const Player* request, Bool* response) override {
        int playerid = request->playerid();
        std::cout << "HasEvent gameid " <<  request->gameid() << std::endl;
        std::cout << "HasEvent playerid " <<  playerid << std::endl;
        Game& game = games.at(request->gameid());

        response->set_hasevent(!game.events_[playerid].empty());

        return Status::OK;
    }

private:
    std::vector<Game> games;

    utility::spinlock spin;
    std::set<int> availableIds;
    int numberOfMadeGames = 0;
};


class LocalServer {
public:
    LocalServer() {
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        server = builder.BuildAndStart();
    }

    void runServer() {
        server->Wait();
    }

    void terminate() {
        server->Shutdown();
    }

private:
    std::unique_ptr<Server> server;
    GameServerImpl service;
    std::string server_address = "0.0.0.0:50051";
    ServerBuilder builder;
};

void RunServer(LocalServer* serv, bool needRun) {
    if (needRun) {
        serv->runServer();
    }
}


