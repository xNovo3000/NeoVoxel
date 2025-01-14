#pragma once

#include <NeoVoxel/Core/Base.h>

namespace NeoVoxel {

    class Timestep {

    public:
        Timestep(double deltaSeconds);
        ~Timestep() = default;

        Timestep(const Timestep&) = default;
        Timestep& operator=(const Timestep&) = default;

        Timestep(Timestep&&) noexcept = default;
        Timestep& operator=(Timestep&&) noexcept = default;

        double deltaSeconds() const noexcept { return m_DeltaSeconds; }
        double deltaMilliseconds() const;

    private:
        double m_DeltaSeconds;

    };

    class Timepoint {

    public:
        Timepoint(int64_t nanoseconds);
        ~Timepoint() = default;

        Timepoint(const Timepoint&) = default;
        Timepoint& operator=(const Timepoint&) = default;

        Timepoint(Timepoint&&) noexcept = default;
        Timepoint& operator=(Timepoint&&) noexcept = default;

        int64_t getNanoseconds() const noexcept { return m_Nanoseconds; }

    private:
        int64_t m_Nanoseconds;

    };

    Timestep operator-(const Timepoint&, const Timepoint&);

}