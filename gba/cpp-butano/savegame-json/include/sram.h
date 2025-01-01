#ifndef SRAM_H
#define SRAM_H

#include "../hw/include/bn_hw_sram.h"
#include "const.h"
#include "bn_log.h"
#include "bn_string.h"
#include "bn_sstream.h"
#include "bn_sram.h"
#include "log_long_chars.h"
#include "parsers_stack.h"
#include "rapidjson_inc_no_warns.h"
#include "savegame.h"
#include "savegame_parser.h"
#include "savegame_serializer.h"

namespace sram
{
    struct LoadResult
    {
        bool error = true;
        int sram_old_usage = 0;
        CharsPointerCopyWrapper old_save_game_version;
        SaveGame *saveGame = NULL;
    };

    struct GetVersionResult
    {
        bool error = true;
        int start_pos = 0;
        int end_pos = 0;
        CharsPointerCopyWrapper version;
    };

    struct JsonCharsParseSaveGameResult
    {
        bool error = true;
        SaveGame *saveGame = NULL;
    };
}

class Sram
{
public:
    static const int actual_sram_size = bn::hw::sram::size();
    static const int expected_sram_start_length = strlen(app_const::SRAM_BEGINNING_EXPECTED);

    sram::LoadResult load()
    {
        sram::LoadResult result;

        char *sram_chars = new char[actual_sram_size];

        this->_load_chars(sram_chars);

        log_long_chars(sram_chars, 200, "sram chars");

        bn::string_view sram_string_view(sram_chars);

        result.sram_old_usage = sram_string_view.length();

        bool sram_has_savegame = sram_string_view.starts_with(app_const::SRAM_BEGINNING_EXPECTED);

        if (!sram_has_savegame)
        {
            BN_LOG("SRAM doesn't have savegame");

            delete[] sram_chars;
            return result;
        }

        BN_LOG("SRAM has savegame");

        sram::GetVersionResult version_result = this->_sram_chars_get_version(sram_chars);
        if (version_result.error)
        {
            BN_LOG("Can't parse version from SRAM data");
        }

        // BN_LOG("__TEST__ 100: ", version_result.version.get_chars());
        // result.old_save_game_version = version_result.version;
        // result.old_save_game_version.set_chars(version_result.version.get_chars());
        // result.old_save_game_version = *CharsPointerCopyWrapper::copy_new(version_result.version);
        // result.old_save_game_version = CharsPointerCopyWrapper::copy(version_result.version);
        CharsPointerCopyWrapper::set_fields_to_from(&result.old_save_game_version, &version_result.version);
        // result.old_save_game_version = CharsPointerCopyWrapper(version_result.version);
        // BN_LOG("__TEST__ 200: ", result.old_save_game_version.get_chars());

        BN_LOG("__TEST__ 300: ", result.old_save_game_version.get_chars());

        char semicolon_after_version_pos = version_result.end_pos + 1;
        char *json_chars = sram_chars + semicolon_after_version_pos;

        log_long_chars(json_chars, 200, "json chars");

        sram::JsonCharsParseSaveGameResult json_parse_result = this->_json_chars_parse_save_game(json_chars);

        if (json_parse_result.error) {
            delete[] sram_chars;
            return result;
        }

        result.saveGame = json_parse_result.saveGame;
        result.saveGame->loads_count++;
        result.error = false;

        // result.sram_old_usage = 123456;
        delete[] sram_chars;
        return result;
    }

    void format()
    {
        bn::sram::clear(bn::sram::size());
    }

    void save(SaveGame *saveGame, int sram_old_usage)
    {
        rapidjson::StringBuffer sbuf;
        rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(sbuf);
        serialize_savegame(&jsonWriter, saveGame);
        const char *json = sbuf.GetString();

        bn::string<actual_sram_size> *data_to_save_in_sram_string = new bn::string<actual_sram_size>();
        bn::ostringstream data_to_save_in_sram_stream(*data_to_save_in_sram_string);
        data_to_save_in_sram_stream << app_const::SRAM_BEGINNING_EXPECTED << app_const::APP_VERSION << ":" << json;

        int sram_new_usage = data_to_save_in_sram_string->length();

        BN_LOG("old data in SRAM size: ", sram_old_usage);
        BN_LOG("new data to save in SRAM size: ", sram_new_usage);

        if (sram_new_usage < sram_old_usage)
        {
            BN_LOG("new_size smaller, zeroing SRAM [old_size - new_size ... old_size] before write!");
            bn::hw::sram::set_bytes(0, sram_old_usage - sram_new_usage + 1, sram_new_usage);
        }

        const char *data_to_save_in_sram_chars = data_to_save_in_sram_string->c_str();

        log_long_chars(data_to_save_in_sram_chars, 200, "data to save in sram chars");

        // bn::sram::write(data_to_save_in_sram_chars);
        _bn::sram::unsafe_write(data_to_save_in_sram_chars, sram_new_usage, 0);

        delete data_to_save_in_sram_string;
    }

    void _load_chars(char *sram_chars)
    {
        _bn::sram::unsafe_read(sram_chars, actual_sram_size, 0);
    }

    // bool _sram_chars_have_savegame(char *sram_chars)
    // {
    //     bn::string_view sram_beginning_string_view = bn::string_view(
    //         sram_chars,
    //         expected_sram_start_length);

    //     return sram_beginning_string_view == app_const::SRAM_BEGINNING_EXPECTED;
    // }

    sram::GetVersionResult _sram_chars_get_version(char *sram_chars)
    {
        sram::GetVersionResult result;

        bn::string_view sram_version_part_string_view = bn::string_view(
            sram_chars + expected_sram_start_length,
            app_const::APP_VERSION_MAX_LENGTH);

        const char *chars_to_search = ":";

        int version_end_find = sram_version_part_string_view.find(chars_to_search);

        if (!version_end_find)
        {
            return result;
        }

        result.start_pos = expected_sram_start_length;
        result.end_pos = expected_sram_start_length + version_end_find;

        bn::string_view version_string_view = bn::string_view(
            sram_chars + result.start_pos,
            result.end_pos - result.start_pos);

        result.version.set_chars(version_string_view);
        result.error = false;

        return result;
    }

    sram::JsonCharsParseSaveGameResult _json_chars_parse_save_game(char *jsonChars)
    {
        sram::JsonCharsParseSaveGameResult result;
        SaveGameParserHandler *root_handler;
        root_handler = new SaveGameParserHandler();
        rapidjson::Reader reader;
        rapidjson::StringStream json_chars_string_stream(jsonChars);
        ParsersStack *parsersStack = new ParsersStack((AbstractStackableParserHandler *)root_handler, &reader, &json_chars_string_stream);

        // try {
        while (parsersStack->parse_next_token())
        {
        }

        result.saveGame = root_handler->get_result();
        root_handler->destruct_result = false;
        result.error = false;
        // } catch (...) {
        // BN_LOG("sram::json_chars_parse_save_game parse error");
        // }

        delete parsersStack;
        delete root_handler;
        return result;
    }
};
#endif // SRAM_H