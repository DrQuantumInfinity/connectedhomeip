#pragma once

#include <app/clusters/mode-base-server/mode-base-server.h>
#include <app/util/af.h>
#include <app/util/config.h>

namespace chip {
namespace app {
namespace Clusters {
namespace TestMode {

const uint8_t ModeNormal  = 0;
const uint8_t ModeDefrost = 1;
const ClusterId Id = 99999;

enum class ModeTag : uint16_t
{
    kNormal  = 0x4000,
    kDefrost = 0x4001,

    // All received enum values that are not listed above will be mapped
    // to kUnknownEnumValue. This is a helper enum value that should only
    // be used by code to process how it handles receiving and unknown
    // enum value. This specific should never be transmitted.
    kUnknownEnumValue = 0,
};

class ExampleTestModeDelegate : public ModeBase::Delegate
{

private:
    using ModeTagStructType              = detail::Structs::ModeTagStruct::Type;
    ModeTagStructType modeTagsNormal[1]  = { { .value = to_underlying(ModeTag::kNormal) } };
    ModeTagStructType modeTagsDefrost[1] = { { .value = to_underlying(ModeTag::kDefrost) } };

    const detail::Structs::ModeOptionStruct::Type kModeOptions[2] = {
        detail::Structs::ModeOptionStruct::Type{ .label    = CharSpan::fromCharString("Normal"),
                                                 .mode     = ModeNormal,
                                                 .modeTags = DataModel::List<const ModeTagStructType>(modeTagsNormal) },
        detail::Structs::ModeOptionStruct::Type{ .label    = CharSpan::fromCharString("Defrost"),
                                                 .mode     = ModeDefrost,
                                                 .modeTags = DataModel::List<const ModeTagStructType>(modeTagsDefrost) }
    };
    CHIP_ERROR Init() override;
    void HandleChangeToMode(uint8_t mode, ModeBase::Commands::ChangeToModeResponse::Type & response) override;

    CHIP_ERROR GetModeLabelByIndex(uint8_t modeIndex, MutableCharSpan & label) override;
    CHIP_ERROR GetModeValueByIndex(uint8_t modeIndex, uint8_t & value) override;
    CHIP_ERROR GetModeTagsByIndex(uint8_t modeIndex, DataModel::List<ModeTagStructType> & tags) override;

public:
    ~ExampleTestModeDelegate() override = default;
};

ModeBase::Instance * Instance();

void Shutdown();

} // namespace TestMode
} // namespace Clusters
} // namespace app
} // namespace chip

void emberAfTestModeClusterInitCallback(chip::EndpointId endpointId);