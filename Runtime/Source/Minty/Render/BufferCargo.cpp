#include "pch.h"
#include "BufferCargo.h"

using namespace Minty;

UUID Minty::BufferCargo::create_group()
{
    // create ID
    UUID id = UUID::create();

    // make group
    m_groups.emplace(id, std::vector<BufferContainer>());

    // return ID to that group
    return id;
}

BufferContainer& Minty::BufferCargo::get_container(UUID const groupId, Size const index)
{
    MINTY_ASSERT_MESSAGE(m_groups.contains(groupId), "BufferCargo does not contain a group with the given ID.");

    // find group
    std::vector<BufferContainer>& group = m_groups.at(groupId);

    MINTY_ASSERT_FORMAT(index <= group.size(), "BufferCargo getting container at index {} when the size of the cargo is {}. The given index should be <= the group size.", index, group.size());

    // ensure there is a value at that index
    if (index == group.size())
    {
        group.push_back(BufferContainer(m_initialCapacity, m_usage));
    }

    // return buffer
    return group.at(index);
}
