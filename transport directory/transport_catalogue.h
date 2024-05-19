#pragma once
#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>

#include "geo.h"

namespace transport_catalogue
{
struct Bus;

struct Stop
{
  std::string name;
  Coordinates coordinates;
};

struct Bus
{
  std::string name;
  std::vector<Stop*> stops;
};

struct BusInfo
{
  std::string_view name;
  size_t stops_count;
  size_t unique_stops_count;
  int route_length;
  double curvature;
};

struct DistanceHasher
{
    std::hash<const void*> hasher;
    size_t operator()(std::pair<const Stop*, const Stop*> pair_stop) const noexcept
    {
      auto hash1 = static_cast<const void*>(pair_stop.first);
      auto hash2 = static_cast<const void*>(pair_stop.second);
      return hasher(hash1) * 15 + hasher(hash2);
    }
};

struct RouteDistance
{
  std::string_view name;
  int fact_distance;
  double geo_distance;
};

class TransportCatalogue
{
  public:
    void AddStop(const Stop& stop);
    Bus* AddBus(const Bus& bus, const std::vector<std::string_view>& stops);
    Stop* GetStopByName(std::string_view stop_name) const;
    Bus* GetBusByName(std::string_view bus_name) const;
    void AddDistance(std::string from, std::string to, int distance);

    BusInfo GetBusStatistics(std::string_view bus_name) const;
    std::unordered_set<Bus*> GetStopBuses(std::string_view stop_name) const;

  private:
    void AddBusToStop(std::string_view stop_name, Bus* bus);
    RouteDistance CalculateRouteDistance(std::string_view bus_name) const;
    int CalculateFactDistance(Stop* from, Stop* to) const;

    std::deque<Stop> stops_;
    std::deque<Bus> buses_;
    std::unordered_map<std::string_view, Stop*> stopname_to_stop_;
    std::unordered_map<std::string_view, Bus*> busname_to_bus_;
    std::unordered_map<std::string_view, std::unordered_set<Bus*>> stop_to_buses_;
    std::unordered_map<std::pair<Stop*, Stop*>, int, DistanceHasher> distances_;
};
}