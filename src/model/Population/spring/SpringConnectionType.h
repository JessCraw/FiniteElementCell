#ifndef SPRINGCONNECTIONTYPE_H
#define SPRINGCONNECTIONTYPE_H

enum class SpringConnectionType
{
    Start,
    End
};

inline SpringConnectionType getOppositeConnectionType(SpringConnectionType type)
{
    return type == SpringConnectionType::Start ? SpringConnectionType::End : SpringConnectionType::Start;
}

#endif  // SPRINGCONNECTIONTYPE_H
