#include <string>
#include <sstream>
#include <algorithm>
#include <utility>
#include "../include/client_logger.h"
#include <not_implemented.h>

std::unordered_map<std::string, std::pair<size_t, std::ofstream>> client_logger::refcounted_stream::_global_streams;


logger& client_logger::log(
    const std::string &text,
    logger::severity severity) &
{
    std::string my_output = make_format(text, severity);
    auto check_stream = _output_streams.find(severity);

    if (check_stream == _output_streams.end()) {
        return *this;
    }

    // Запись в поток вывода сообщения (если bool = true, т.е. надо выводить в стандартный поток)
    if (check_stream->second.second) {
        std::cout << my_output << std::endl;
    }

    // Проходим по всему списку объектов класса refcounted_stream
    for (auto &stream: check_stream->second.first) {
        // Если поток норм, то выводим в него наше сообщение
        std::ofstream *basic_ofstream = stream._stream.second;
        if (basic_ofstream != nullptr) {
            *basic_ofstream << my_output << std::endl;
        }
    }

    return *this;
}

// возвращается строка, в которую записано сообщение/вызовы методов
std::string client_logger::make_format(const std::string &message, severity sev) const {
    std::ostringstream my_stream;
    bool check_flag = false;

    for (char symbol : _format) {
        if (check_flag == true) {
            switch (char_to_flag(symbol)) {
                case flag::DATE:
                    my_stream << current_date_to_string();
                break;
                case flag::TIME:
                    my_stream << current_time_to_string();
                break;
                case flag::SEVERITY:
                    my_stream << severity_to_string(sev);
                break;
                case flag::MESSAGE:
                    my_stream << message;
                break;
                default:
                        my_stream << '%' << symbol;
                break;
            }

            check_flag = false;
        } else if (symbol == '%') {
            check_flag = true;
        } else {
            my_stream << symbol;
        }
    }

    if (check_flag == true) {
        my_stream << '%';
    }

    return my_stream.str();
}

// человеческий конструктор
client_logger::client_logger(
        const std::unordered_map<logger::severity, std::pair<std::forward_list<refcounted_stream>, bool>> &streams,
        std::string format) : _output_streams(streams), _format(std::move(format)) {}

// Просто функция для выбора флага в зависимости от переданного чара
client_logger::flag client_logger::char_to_flag(char c) noexcept
{
    switch (c) {
        case 'd':
            return flag::DATE;
        case 't':
            return flag::TIME;
        case 's':
            return flag::SEVERITY;
        case 'm':
            return flag::MESSAGE;
        default:
            return flag::NO_FLAG;
    }
}

// Конструктор копирования
client_logger::client_logger(const client_logger &other) : _output_streams(other._output_streams), _format(other._format) {}

// Перегрузка оператора присваивания
client_logger &client_logger::operator=(const client_logger &other)
{
    if (this == &other) {
        return *this;
    }

    _output_streams = other._output_streams;
    _format = other._format;
    return *this;
}

// Конструктор перемещения
client_logger::client_logger(client_logger &&other) noexcept : _output_streams(std::move(other._output_streams)), _format(std::move(other._format)) {}

// Присваивание и перемещение
client_logger &client_logger::operator=(client_logger &&other) noexcept
{
    if (this == &other) {
        return *this;
    }

    _output_streams = std::move(other._output_streams);
    _format = std::move(other._format);
    return *this;
}

// Деструктор
client_logger::~client_logger() noexcept = default;

// Человеческий конструктор
client_logger::refcounted_stream::refcounted_stream(const std::string &path)
{
    // Ищем по строке
    auto check_stream = _global_streams.find(path);
    if (check_stream == _global_streams.end()) {
        // попытка вставить
        auto insert = _global_streams.emplace(path, std::make_pair(1, std::ofstream(path)));
        // Ссылка на пару
        auto &stream = insert.first->second;

        // Не получилось открыть поток
        if (!stream.second.is_open()) {
            throw std::ios_base::failure("Can't open file " + path);
        }

        _stream = std::make_pair(path, &insert.first->second.second);
    } else {
        // Увеличиваем счетчик ссылок на эту тему
        check_stream->second.first++;
        _stream = std::make_pair(path, &check_stream->second.second);
    }
}

// Конструктор копирования
client_logger::refcounted_stream::refcounted_stream(const client_logger::refcounted_stream &oth)
{
    auto check_stream = _global_streams.find(oth._stream.first);

    // Если нашли, то записываем данные
    if (check_stream != _global_streams.end()) {
        ++check_stream->second.first;
        _stream.second = &check_stream->second.second;
    } else {
        auto inserted = _global_streams.emplace(_stream.first, std::make_pair<size_t>(1, std::ofstream(oth._stream.first)));

        // Элемент не добавлен или поток не открывается
        if (!inserted.second || !inserted.first->second.second.is_open()) {
            // если был, то всплыл
            if (inserted.second) {
                _global_streams.erase(inserted.first);
            }

            throw std::ios_base::failure("Can't open file " + oth._stream.first);
        }
        // Берется адрес потока
        _stream.second = &inserted.first->second.second;
    }
}

// Перегрузка оператора присваивания
client_logger::refcounted_stream &
client_logger::refcounted_stream::operator=(const client_logger::refcounted_stream &oth)
{
    if (this == &oth) {
        return *this;
    }

    if (_stream.second != nullptr) {
        // ищем по строке
        auto check_stream = _global_streams.find(_stream.first);

        if (check_stream != _global_streams.end()) {
            // уменьшаем счетчик
            --check_stream->second.first;

            if (check_stream->second.first == 0) {
                // Закрыли все и удалили из таблицы
                check_stream->second.second.close();
                _global_streams.erase(_stream.first);
            }
        }
    }

    // записываем данные
    _stream.first = oth._stream.first;
    _stream.second = oth._stream.second;

    // мейби oth._stream.second != nullptr => ищем и счетчик +1
    if (_stream.second != nullptr) {
        auto check_stream = _global_streams.find(_stream.first);
        ++check_stream->second.first;
    }

    return *this;
}

// конструктор перемещения
client_logger::refcounted_stream::refcounted_stream(client_logger::refcounted_stream &&oth) noexcept : _stream(std::move(oth._stream))
{
    oth._stream.second = nullptr;
}

// оператор перемещающего присваивания
client_logger::refcounted_stream &client_logger::refcounted_stream::operator=(client_logger::refcounted_stream &&oth) noexcept
{
    if (this == &oth) {
        return *this;
    }

    _stream = std::move(oth._stream);
    oth._stream.second = nullptr;
    return *this;
}

// Человеческий деструктор
client_logger::refcounted_stream::~refcounted_stream()
{
    if (_stream.second != nullptr) {
        auto check_stream = _global_streams.find(_stream.first);

        if (check_stream != _global_streams.end()) {
            // Удалили строчку
            --check_stream->second.first;

            if (check_stream->second.first == 0) {
                // Закрыли поток
                check_stream->second.second.close();
                // Удалили из таблицы
                _global_streams.erase(check_stream);
            }
        }
    }
}
