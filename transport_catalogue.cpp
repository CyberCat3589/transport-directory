#include "transport_catalogue.h"

void TransportCatalogue::AddStop(Stop&& stop)
{
    stops_.push_back(std::move(stop));
    Stop* ptr_stop = &stops_[stops_.size() - 1];
    stopname_to_stop_[ptr_stop->name] = ptr_stop;
}

void TransportCatalogue::AddBus(Bus&& bus)
{
    buses_.push_back(std::move(bus));
    Bus* ptr_bus = &buses_[buses_.size() - 1];
    busname_to_bus[ptr_bus->name] = ptr_bus;
}

Stop* TransportCatalogue::GetStopByName(std::string_view stop_name) const
{
    return stopname_to_stop_.at(stop_name);
}

Bus* TransportCatalogue::GetBusByName(std::string_view) const
{
    //отсюда
}