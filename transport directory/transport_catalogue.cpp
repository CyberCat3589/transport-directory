#include "transport_catalogue.h"
#include <algorithm>

using namespace transport_catalogue;

void TransportCatalogue::AddStop(const Stop& stop)
{
    stops_.push_back(stop);
    Stop* ptr_stop = &stops_[stops_.size() - 1];
    stopname_to_stop_[ptr_stop->name] = ptr_stop;
}

Bus* TransportCatalogue::AddBus(const Bus& bus)
{
    buses_.push_back(bus);
    Bus* ptr_bus = &buses_[buses_.size() - 1];
    busname_to_bus_[ptr_bus->name] = ptr_bus;
    return ptr_bus;
}

void TransportCatalogue::AddDistances(std::vector<Distance> distances)
{
    for(auto distance : distances)
    {
        Stop* from = GetStopByName(distance.from);
        Stop* to = GetStopByName(distance.to);
        auto stops_pair = std::make_pair(from, to);
        distances_.insert(DistanceMap::value_type(stops_pair, distance.distance));
    }
}

Stop* TransportCatalogue::GetStopByName(std::string_view stop_name) const
{
    Stop* stop_ptr = nullptr;
    if(auto iter = stopname_to_stop_.find(stop_name); iter != stopname_to_stop_.end())
    {
        stop_ptr = stopname_to_stop_.at(stop_name);
    }
    return stop_ptr;
}

Bus* TransportCatalogue::GetBusByName(std::string_view bus_name) const
{
    Bus* bus_ptr = nullptr;
    if(auto iter = busname_to_bus_.find(bus_name); iter != busname_to_bus_.end())
    {
        bus_ptr = busname_to_bus_.at(bus_name);
    }
    return bus_ptr;
}

double TransportCatalogue::CalculateDistance(std::string_view bus_name) const
{
    Bus* bus = busname_to_bus_.at(bus_name);
    double route_length = 0;
    std::vector<Stop*>& stops = bus->stops;
    for (auto it = stops.begin(); it != std::prev(stops.end(), 1); ++it)
    {
        auto next_stop = std::next(it);
        Stop* stop1 = *it;
        Stop* stop2 = *next_stop;

        route_length += ComputeDistance(stop1->coordinates, stop2->coordinates);
    }
    return route_length;
}

BusStatistics TransportCatalogue::GetBusStatistics(std::string_view bus_name) const
{
    Bus* bus = busname_to_bus_.at(bus_name);
    std::vector<Stop*>& bus_stops = bus->stops;

    size_t stops_count = bus_stops.size();  // кол-во остановок в маршруте

    std::unordered_set unique_stops(bus_stops.cbegin(), bus_stops.cend());
    size_t unique_stops_count = unique_stops.size();  // кол-во уникальных остановок

    // длина маршрута
    double route_length = CalculateDistance(bus->name);

    return BusStatistics{bus_name, stops_count, unique_stops_count, route_length};
}

std::vector<Bus*> TransportCatalogue::GetStopBuses(std::string_view stop_name) const
{
    Stop* stop = stopname_to_stop_.at(stop_name);
    std::vector<Bus*>& buses = stop->buses;
    if(!buses.empty())
    {
        std::sort(buses.begin(), buses.end(), [](Bus* lhs, Bus* rhs) { return lhs->name < rhs->name; });
    }
    
    return buses;
}