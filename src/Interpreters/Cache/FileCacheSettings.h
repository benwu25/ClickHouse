#pragma once

#include <Core/Defines.h>
#include <Core/BaseSettingsFwdMacros.h>
#include <Core/FormatFactorySettings.h>
#include <Core/NamesAndTypes.h>
#include <Core/SettingsFields.h>
#include <Core/SettingsEnums.h>
#include <Interpreters/Cache/FileCache_fwd.h>

namespace Poco { namespace Util { class AbstractConfiguration; } } // NOLINT(cppcoreguidelines-virtual-class-destructor)

namespace DB
{
struct FileCacheSettingsImpl;
class NamedCollection;
struct MutableColumnsAndConstraints;

#define FILE_CACHE_SETTINGS_SUPPORTED_TYPES(CLASS_NAME, M) \
    M(CLASS_NAME, String) \
    M(CLASS_NAME, Bool) \
    M(CLASS_NAME, Double) \
    M(CLASS_NAME, UInt64)

FILE_CACHE_SETTINGS_SUPPORTED_TYPES(FileCacheSettings, DECLARE_SETTING_TRAIT)

struct FileCacheSettings
{
    FileCacheSettings();
    FileCacheSettings(const FileCacheSettings & settings);
    FileCacheSettings(FileCacheSettings && settings) noexcept;
    FileCacheSettings & operator=(FileCacheSettings && settings) noexcept;
    bool operator==(const FileCacheSettings & settings) const noexcept;
    ~FileCacheSettings();

    static NamesAndTypesList getSampleBlock();

    FILE_CACHE_SETTINGS_SUPPORTED_TYPES(FileCacheSettings, DECLARE_SETTING_SUBSCRIPT_OPERATOR)

    void loadFromConfig(const Poco::Util::AbstractConfiguration & config, const std::string & config_prefix);
    void loadFromCollection(const NamedCollection & collection);
    void dumpToSystemSettingsColumns(MutableColumnsAndConstraints & params, const FileCachePtr & cache) const;

private:
    std::unique_ptr<FileCacheSettingsImpl> impl;
};

}
