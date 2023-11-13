#ifndef INTERPOLATE_H
#define INTERPOLATE_H

class LocationsContainer;
namespace Interpolate
{
    void interpolateChildFromParent(const LocationsContainer &parent, LocationsContainer &child);
    void interpolateChildBoundaries(const LocationsContainer &parent, LocationsContainer &child);
}  // namespace Interpolate

#endif  // INTERPOLATE_H
