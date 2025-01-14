#include <PCH.h>
#include <NeoVoxel/Core/Time.h>

namespace NeoVoxel {

    /* Timestep */

    Timestep::Timestep(double deltaSeconds) : m_DeltaSeconds(deltaSeconds) {}

    double Timestep::deltaMilliseconds() const { return m_DeltaSeconds * 1000.0F; }

    /* Timepoint */

    Timepoint::Timepoint(int64_t nanoseconds) : m_Nanoseconds(nanoseconds) {}

    /* Utility */

    Timestep operator-(const Timepoint& lhs, const Timepoint& rhs) {
        auto nanosecondsDifference = lhs.getNanoseconds() - rhs.getNanoseconds();
        auto secondsDifference = static_cast<double>(nanosecondsDifference) / 1000000000.0;
        return Timestep(secondsDifference);
    }

}