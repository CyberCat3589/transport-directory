#pragma once
#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
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

struct BusStatistics
{
    std::string_view name;
    size_t stops_count;
    size_t unique_stops_count;
    double route_length;
};

class TransportCatalogue
{
  public:
    void AddStop(const Stop& stop);
    Bus* AddBus(const Bus& bus);
    Stop* GetStopByName(std::string_view stop_name) const;
    Bus* GetBusByName(std::string_view bus_name) const;

    BusStatistics GetBusStatistics(std::string_view bus_name) const;
    std::vector<Bus*> GetStopBuses(std::string_view stop_name) const;

  private:
    double CalculateDistance(std::string_view bus_name) const;

    std::deque<Stop> stops_;
    std::deque<Bus> buses_;
    std::unordered_map<std::string_view, Stop*> stopname_to_stop_;
    std::unordered_map<std::string_view, Bus*> busname_to_bus_;
};
}