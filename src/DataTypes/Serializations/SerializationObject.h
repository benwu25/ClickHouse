#pragma once

#include <Columns/ColumnObject.h>
#include <DataTypes/DataTypeObject.h>
#include <list>

namespace DB
{

class SerializationObjectDynamicPath;
class SerializationSubObject;

/// Class for binary serialization/deserialization of an Object type (currently only JSON).
class SerializationObject : public ISerialization
{
public:
    /// Serialization can change in future. Let's introduce serialization version.
    struct ObjectSerializationVersion
    {
        enum Value
        {
            BASIC = 0,
        };

        Value value;

        static void checkVersion(UInt64 version);

        explicit ObjectSerializationVersion(UInt64 version);
    };

    SerializationObject(
        std::unordered_map<String, SerializationPtr> typed_path_serializations_,
        const std::unordered_set<String> & paths_to_skip_,
        const std::vector<String> & path_regexps_to_skip_);

    void enumerateStreams(
        EnumerateStreamsSettings & settings,
        const StreamCallback & callback,
        const SubstreamData & data) const override;

    void serializeBinaryBulkStatePrefix(
        const IColumn & column,
        SerializeBinaryBulkSettings & settings,
        SerializeBinaryBulkStatePtr & state) const override;

    void serializeBinaryBulkStateSuffix(
        SerializeBinaryBulkSettings & settings,
        SerializeBinaryBulkStatePtr & state) const override;

    void deserializeBinaryBulkStatePrefix(
        DeserializeBinaryBulkSettings & settings,
        DeserializeBinaryBulkStatePtr & state,
        SubstreamsDeserializeStatesCache * cache) const override;

    void serializeBinaryBulkWithMultipleStreams(
        const IColumn & column,
        size_t offset,
        size_t limit,
        SerializeBinaryBulkSettings & settings,
        SerializeBinaryBulkStatePtr & state) const override;

    void deserializeBinaryBulkWithMultipleStreams(
        ColumnPtr & column,
        size_t rows_offset,
        size_t limit,
        DeserializeBinaryBulkSettings & settings,
        DeserializeBinaryBulkStatePtr & state,
        SubstreamsCache * cache) const override;

    void serializeBinary(const Field & field, WriteBuffer & ostr, const FormatSettings &) const override;
    void deserializeBinary(Field & field, ReadBuffer & istr, const FormatSettings &) const override;
    void serializeBinary(const IColumn & column, size_t row_num, WriteBuffer & ostr, const FormatSettings &) const override;
    void deserializeBinary(IColumn & column, ReadBuffer & istr, const FormatSettings &) const override;

    static void restoreColumnObject(ColumnObject & column_object, size_t prev_size);

private:
    friend SerializationObjectDynamicPath;
    friend SerializationSubObject;

    /// State of an Object structure. Can be also used during deserializing of Object subcolumns.
    struct DeserializeBinaryBulkStateObjectStructure : public ISerialization::DeserializeBinaryBulkState
    {
        ObjectSerializationVersion structure_version;
        size_t max_dynamic_paths;
        std::vector<String> sorted_dynamic_paths;
        std::unordered_set<String> dynamic_paths;
        /// Paths statistics. Map (dynamic path) -> (number of non-null values in this path).
        ColumnObject::StatisticsPtr statistics;

        explicit DeserializeBinaryBulkStateObjectStructure(UInt64 structure_version_) : structure_version(structure_version_) {}
    };

    static DeserializeBinaryBulkStatePtr deserializeObjectStructureStatePrefix(
        DeserializeBinaryBulkSettings & settings,
        SubstreamsDeserializeStatesCache * cache);

    /// Shared data has type Array(Tuple(String, String)).
    static const DataTypePtr & getTypeOfSharedData();

    struct TypedPathSubcolumnCreator : public ISubcolumnCreator
    {
        String path;

        explicit TypedPathSubcolumnCreator(const String & path_) : path(path_) {}

        DataTypePtr create(const DataTypePtr & prev) const override { return prev; }
        ColumnPtr create(const ColumnPtr & prev) const override { return prev; }
        SerializationPtr create(const SerializationPtr & prev) const override;
    };

protected:
    bool shouldSkipPath(const String & path) const;

    std::unordered_map<String, SerializationPtr> typed_path_serializations;
    std::unordered_set<String> paths_to_skip;
    std::vector<String> sorted_paths_to_skip;
    std::list<re2::RE2> path_regexps_to_skip;
    SerializationPtr dynamic_serialization;

private:
    std::vector<String> sorted_typed_paths;
    SerializationPtr shared_data_serialization;
};

}
