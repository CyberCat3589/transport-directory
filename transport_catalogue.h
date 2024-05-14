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

struct Distance
{
  Stop* from;
  Stop* to;
  int distance;
};

struct DistanceHasher
{
    std::hash<const void*> hasher;
    size_t operator()(std::pair<const Stop*, const Stop*> pair_stop)
    {
      auto hash1 = static_cast<const void*>(pair_stop.first);
      auto hash2 = static_cast<const void*>(pair_stop.second);
      return hasher(hash1) * 15 + hasher(hash2);
    }
};

using StopMap = std::unordered_map<std::string_view, Stop*>;
using BusMap = std::unordered_map<std::string_view, Bus*>;
using DistanceMap = std::unordered_map<std::pair<Stop*, Stop*>, int, DistanceHasher>;

class TransportCatalogue
{
  public:
    void AddStop(const Stop& stop);
    Bus* AddBus(const Bus& bus);
    Stop* GetStopByName(std::string_view stop_name) const;
    Bus* GetBusByName(std::string_view bus_name) const;
    void AddDistance(std::vector<Distance> distances);

    BusStatistics GetBusStatistics(std::string_view bus_name) const;
    std::vector<Bus*> GetStopBuses(std::string_view stop_name) const;

  private:
    double CalculateDistance(std::string_view bus_name) const;

    std::deque<Stop> stops_;
    std::deque<Bus> buses_;
    StopMap stopname_to_stop_;
    BusMap busname_to_bus_;
    DistanceMap distances_;
};
}