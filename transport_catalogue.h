#pragma once
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

#include "geo.h"

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

class TransportCatalogue
{
  public:
  private:
    std::deque<Stop> stops_;
    std::deque<Bus> buses_;
    std::unordered_map<std::string_view, Stop*> stopname_to_stop_;
    std::unordered_map<std::string_view, Bus*> busname_to_bus;
};