#include "input_reader.h"

#include <algorithm>
#include <cassert>
#include <iterator>

/**
 * Парсит строку вида "10.123,  -30.1837" и возвращает пару координат (широта, долгота)
 */
transport_catalogue::Coordinates ParseCoordinates(std::string_view str)
{
    static const double nan = std::nan("");

    auto not_space = str.find_first_not_of(' ');
    auto comma = str.find(',');

    if (comma == str.npos)
    {
        return {nan, nan};
    }

    auto not_space2 = str.find_first_not_of(' ', comma + 1);

    double lat = std::stod(std::string(str.substr(not_space, comma - not_space)));

    auto comma2 = str.find(',', comma + 1);
    double lng = std::stod(std::string(str.substr(not_space2, comma2 - not_space2)));

    return {lat, lng};
}

/**
 * Удаляет пробелы в начале и конце строки
 */
std::string_view Trim(std::string_view string)
{
    const auto start = string.find_first_not_of(' ');
    if (start == string.npos)
    {
        return {};
    }
    return string.substr(start, string.find_last_not_of(' ') + 1 - start);
}

/**
 * Разбивает строку string на n строк, с помощью указанного символа-разделителя delim
 */
std::vector<std::string_view> Split(std::string_view string, char delim)
{
    std::vector<std::string_view> result;

    size_t pos = 0;
    while ((pos = string.find_first_not_of(' ', pos)) < string.length())
    {
        auto delim_pos = string.find(delim, pos);
        if (delim_pos == string.npos)
        {
            delim_pos = string.size();
        }
        if (auto substr = Trim(string.substr(pos, delim_pos - pos)); !substr.empty())
        {
            result.push_back(substr);
        }
        pos = delim_pos + 1;
    }

    return result;
}

/**
 * Парсит маршрут.
 * Для кольцевого маршрута (A>B>C>A) возвращает массив названий остановок [A,B,C,A]
 * Для некольцевого маршрута (A-B-C-D) возвращает массив названий остановок [A,B,C,D,C,B,A]
 */
std::vector<std::string_view> ParseRoute(std::string_view route)
{
    if (route.find('>') != route.npos)
    {
        return Split(route, '>');
    }

    auto stops = Split(route, '-');
    std::vector<std::string_view> results(stops.begin(), stops.end());
    results.insert(results.end(), std::next(stops.rbegin()), stops.rend());

    return results;
}

transport_catalogue::input_reader::CommandDescription ParseCommandDescription(std::string_view line)
{
    auto colon_pos = line.find(':');
    if (colon_pos == line.npos)
    {
        return {};
    }

    auto space_pos = line.find(' ');
    if (space_pos >= colon_pos)
    {
        return {};
    }

    auto not_space = line.find_first_not_of(' ', space_pos);
    if (not_space >= colon_pos)
    {
        return {};
    }

    return {std::string(line.substr(0, space_pos)),
            std::string(line.substr(not_space, colon_pos - not_space)),
            std::string(line.substr(colon_pos + 1))};
}

void transport_catalogue::input_reader::InputReader::ParseLine(std::string_view line)
{
    auto command_description = ParseCommandDescription(line);
    if (command_description)
    {
        commands_.push_back(std::move(command_description));
    }
}

std::pair<std::string_view, int> ParseStopAndDistance(std::string_view str)
{
    using namespace std::literals;

    size_t pos_meters = str.find('m');

    size_t pos_to = str.find("to"s, pos_meters);

    size_t stop_pos = str.find_first_not_of(' ', pos_to + 2);

    int distance = std::stoi(std::string(str.substr(0, pos_meters)));

    std::string_view destination = str.substr(stop_pos);

    return {destination, distance};
}

std::vector<std::pair<std::string_view, int>> ParseDistances(std::string_view str)
{
    //отсекаем от строки координаты
    size_t comma = str.find(',', 0);
    size_t comma2 = str.find(',', comma + 1);
    std::string_view dist_str = str.substr(comma2 + 1);

    dist_str = Trim(dist_str);

    std::vector<std::pair<std::string_view, int>> distances;

    if(str.find_first_not_of(' ') == str.npos) return distances;

    //парсинг расстояний и остановок
    if(dist_str.find(',') == dist_str.npos)
    {
        distances.push_back(ParseStopAndDistance(dist_str));
    }
    else
    {
        std::vector<std::string_view> raw_distances = Split(dist_str, ',');
        for(auto raw_dist : raw_distances)
        {
            distances.push_back(ParseStopAndDistance(raw_dist));
        }
    }

    return distances;
}

void transport_catalogue::input_reader::InputReader::ApplyCommands([[maybe_unused]] TransportCatalogue& catalogue) const
{
    using namespace std::literals;

    //обработка запросов на добавление остановок
    for(const CommandDescription& command : commands_)
    {
        if(command.command == "Stop"s)
        {
            Stop stop{command.id, ParseCoordinates(command.description)};
            catalogue.AddStop(stop);
        }
    }

    //добавление расстояний до остановок
    for(const CommandDescription& command : commands_)
    {
        if(command.command == "Stop")
        {
            std::vector<std::pair<std::string_view, int>> to_dist = ParseDistances(command.description);
            if(!to_dist.empty())
            {
                for(auto dist : to_dist)
                {
                    catalogue.AddDistance(command.id, std::string(dist.first), dist.second);
                }
            }
        }
    }

    //обработка запросов на добавление маршрутов
    for(const CommandDescription& command : commands_)
    {
        if (command.command == "Bus"s)
        {
            std::vector<std::string_view> route = ParseRoute(command.description);

            Bus bus{command.id, {}};
            catalogue.AddBus(bus, route);
        }
    }
}