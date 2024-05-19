#include "transport_catalogue.h"
#include <algorithm>

using namespace transport_catalogue;

void TransportCatalogue::AddStop(const Stop& stop)
{
    stops_.push_back(stop);
    Stop* ptr_stop = &stops_.back();
    stopname_to_stop_[ptr_stop->name] = ptr_stop;
}

Bus* TransportCatalogue::AddBus(const Bus& bus, const std::vector<std::string_view>& stops)
{
    buses_.push_back(bus);
    Bus* bus_ptr = &buses_.back();
    busname_to_bus_[bus_ptr->name] = bus_ptr;
    
    Stop* stop;
    for (const auto stop_name : stops)
    {
        stop = GetStopByName(stop_name);
        bus_ptr->stops.push_back(stop);

        AddBusToStop(stop->name, bus_ptr);
    }
    return bus_ptr;
}

void TransportCatalogue::AddDistance(std::string from, std::string to, int distance)
{
    Stop* from_ptr = GetStopByName(from);
    Stop* to_ptr = GetStopByName(to);
    auto stops_pair = std::make_pair(from_ptr, to_ptr);
    distances_.insert(std::unordered_map<std::pair<Stop*, Stop*>, int, DistanceHasher>::value_type(stops_pair, distance));
}

void TransportCatalogue::AddBusToStop(std::string_view stop_name, Bus* bus)
{
    stop_to_buses_[stop_name].insert(bus);
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

int TransportCatalogue::CalculateFactDistance(Stop* from, Stop* to) const
{
    double length = 0;

    auto forward_pair = std::make_pair(from, to);
    auto reverce_pair = std::make_pair(to, from);

    if(auto it = distances_.find(forward_pair); it != distances_.end())
    {
        length = distances_.at(forward_pair);
    }
    else if(auto it = distances_.find(reverce_pair); it != distances_.end())
    {
        length = distances_.at(reverce_pair);
    }

    return length;
}

RouteDistance TransportCatalogue::CalculateRouteDistance(std::string_view bus_name) const
{
    Bus* bus = busname_to_bus_.at(bus_name);
    
    int fact_route_length = 0;
    double geo_route_length = 0;

    std::vector<Stop*>& stops = bus->stops;
    for (auto it = stops.begin(); it != std::prev(stops.end(), 1); ++it)
    {
        auto next_stop = std::next(it);
        Stop* stop1 = *it;
        Stop* stop2 = *next_stop;

        fact_route_length += CalculateFactDistance(stop1, stop2);
        geo_route_length += ComputeDistance(stop1->coordinates, stop2->coordinates);
    }

    return {bus->name, fact_route_length, geo_route_length};
}

BusInfo TransportCatalogue::GetBusStatistics(std::string_view bus_name) const
{
    Bus* bus = busname_to_bus_.at(bus_name);
    std::vector<Stop*>& bus_stops = bus->stops;

    size_t stops_count = bus_stops.size();  // кол-во остановок в маршруте

    std::unordered_set unique_stops(bus_stops.cbegin(), bus_stops.cend());
    size_t unique_stops_count = unique_stops.size();  // кол-во уникальных остановок

    RouteDistance route_distance = CalculateRouteDistance(bus->name);

    //извилистость пути
    double curvature = route_distance.fact_distance / route_distance.geo_distance;

    return BusInfo{bus_name, stops_count, unique_stops_count, route_distance.fact_distance, curvature};
}

std::unordered_set<Bus*> TransportCatalogue::GetStopBuses(std::string_view stop_name) const
{
    std::unordered_set<Bus*> buses;
    if(auto it = stop_to_buses_.find(stop_name); it != stop_to_buses_.end())
    {
        buses = stop_to_buses_.at(stop_name);
    }
    return buses;
}