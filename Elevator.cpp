#include "Elevator.h"
#include "MessageBus.h"

#include "Elevators.h"

Elevator::Elevator(
	const unsigned int			aId,
	const unsigned int			aFloorCount)
	: myId(aId)
	, myFloorCount(aFloorCount)
	, myCurrentFloor(1)
	, myCurrentDirection(Direction::Up)
	, myDestinationFloor(0)
{
	Log("[Elevator]", myId, "Created", ToString());
}

Elevator::Elevator(
	const unsigned int		aId,
	const unsigned int		aFloorCount,
	const unsigned int		aCurrentFloor,
	const Direction			aDirection)
	: myId(aId)
	, myFloorCount(aFloorCount)
	, myCurrentFloor(aCurrentFloor)
	, myCurrentDirection(aDirection)
	, myDestinationFloor(0)
{
	Log("[Elevator]", myId, "Created", ToString());
}

void
Elevator::SelectFloor(
	const unsigned int			aFloorId)
{
	myCurrentFloor = aFloorId;
	// Implement me!
}

unsigned int
Elevator::CurrentFloor() const
{
	return myCurrentFloor;
}

Direction
Elevator::CurrentDirection() const
{
	return myCurrentDirection;
}

bool
Elevator::HasWork() const
{
	// Implement me!
	return !(myCurrentFloor == myDestinationFloor);
}

void
Elevator::Step()
{
	if (myCurrentFloor < myDestinationFloor)
	{
		myCurrentFloor++;
	}
	else if (myCurrentFloor > myDestinationFloor)
	{
		myCurrentFloor--;
	}
	// Implement me!
}

unsigned int
Elevator::Id() const
{
	return myId;
}

std::string
Elevator::ToString() const
{
	return "- State: " 
		+ std::to_string(myCurrentFloor) 
		+ "/" 
		+ std::to_string(myFloorCount) 
		+ " " 
		+ ::ToString(myCurrentDirection);
}

void Elevator::SetDestinationFloor(const unsigned int destFloorId)
{
	myDestinationFloor = destFloorId;

}