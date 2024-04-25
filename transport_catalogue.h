#pragma once
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

#include "geo.h"

namespace transport_catalogue
{
struct Bus;

struct Stop
{
    std::string name;
    Coordinates coordinates;
    std::vector<Bus*> buses;
};

struct Bus
{
    std::string name;
    std::vector<Stop*> stops;
};

class TransportCatalogue
{
  public:
    void AddStop(Stop&& stop);
    Bus* AddBus(Bus&& bus);
    Stop* GetStopByName(std::string_view stop_name) const;
    Bus* GetBusByName(std::string_view bus_name) const;

  private:
    std::deque<Stop> stops_;
    std::deque<Bus> buses_;
    std::unordered_map<std::string_view, Stop*> stopname_to_stop_;
    std::unordered_map<std::string_view, Bus*> busname_to_bus_;
};
}