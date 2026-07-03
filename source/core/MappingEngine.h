#pragma once

#include "ControlVector.h"
#include "FeatureExtractor.h"

class MappingEngine
{
public:
    virtual ~MappingEngine() = default;

    virtual ControlVector mapFeatures(const FeatureExtractor::BoardFeatures& features) const noexcept = 0;
};

class DeterministicMappingEngine final : public MappingEngine
{
public:
    ControlVector mapFeatures(const FeatureExtractor::BoardFeatures& features) const noexcept override;

private:
    static float clamp01(float value) noexcept;
};
