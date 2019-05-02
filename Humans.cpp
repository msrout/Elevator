#include "MessageBus.h"

#include "Humans.h"

Human::Human(
	const unsigned int		aFloor,
	const unsigned int		aDestinationFloor)
	: myFloor(aFloor)
	, myDestinationFloor(aDestinationFloor)
	, myState(HumanState_Idle)
	, myWaitingCounter(0)
	, myTravelingCounter(0)
{
	if (myFloor == myDestinationFloor)
	{
		Log("[Human][Error] Same floor and destination floor: ", myFloor);
		assert(false);
	}
}

HumanState
Human::GetState() const
{
	return myState;
}

void
Human::SetStateWaiting()
{
	myState = HumanState_Waiting;
}

void
Human::SetStateTraveling()
{
	myState = HumanState_Traveling;
}

void
Human::SetStateArrived()
{
	myState = HumanState_Arrived;
}

void
Human::Step()
{
	if (myState == HumanState_Traveling)
	{
		++myTravelingCounter;
	}
	if (myState == HumanState_Waiting)
	{
		++myWaitingCounter;
	}
}

unsigned int
Human::GetWaitingCounter() const
{
	return myWaitingCounter;
}

unsigned int
Human::GetTravelingCounter() const
{
	return myTravelingCounter;
}

///

Humans::Humans()
{
}

void
Humans::Start()
{
	REGISTER_HUMAN(MessageElevatorReady,		Humans::OnMessageElevatorReady);
	REGISTER_HUMAN(MessageElevatorArrived,		Humans::OnMessageElevatorArrived);
	REGISTER_HUMAN(MessageHumanStep,			Humans::OnMessageHumanStep);

	myHumans.push_back(Human(1, 4)); //going up
	myHumans.push_back(Human(6, 2)); // comming down
}

void
Humans::OnMessageElevatorReady(
	const MessageElevatorReady&		aMessage)
{
	Log("[Humans] Elevator system ready");

	MessageHumanStep message;
	SEND_TO_HUMANS(message);
}

void
Humans::OnMessageElevatorArrived(
	const MessageElevatorArrived&	aMessage)
{
	Log("[Humans] Elevator",aMessage.myElevatorId, "has arrived at floor:", aMessage.myFloor);

	// start the request, which floor you want to go
	
	for (Human& human : myHumans)
	{
		
		if (aMessage.myFloor == human.myDestinationFloor)
		{
			human.SetStateArrived();
		}

		// If Elevator arrives at human floor, human will now request
		if (aMessage.myFloor == human.myFloor)
		{
			MessageElevatorRequest message;
			message.myFloor = human.myDestinationFloor;
			message.myElevatorId = aMessage.myElevatorId;
			SEND_TO_ELEVATORS(message);
			human.SetStateTraveling();
		}
	}
	// Implement me!
}

void
Humans::OnMessageHumanStep(
	const MessageHumanStep&		aMessage)
{	
	Log("[Humans] Step");

	for (Human& human : myHumans)
	{
		human.Step();
	}

	PrivPrintTimers();

	// Implement me!
	
	for (Human& human : myHumans)
	{
		human.SetStateWaiting();
		/*
		* callingthe elevator to his own floor
		*/
		Log("[Humans] calling elevator to floor:", human.myFloor);
		MessageElevatorCall callmessage;
		callmessage.myFloor = human.myFloor;
		if (human.myFloor < human.myDestinationFloor)
		{
			callmessage.myDirection = Direction::Up;
		}
		else
		{
			callmessage.myDirection = Direction::Down;
		}
		
		SEND_TO_ELEVATORS(callmessage);
	}
	

	MessageHumanStep message;
	SEND_TO_HUMANS(message);
}

void
Humans::PrivPrintTimers()
{
	unsigned int sumWaiting = 0;
	unsigned int sumTraveling = 0;
	unsigned int humansArrivedCounter = 0;

	for (const Human& human : myHumans)
	{
		if (human.GetState() != HumanState_Arrived)
		{
			continue;
		}

		sumWaiting += human.GetWaitingCounter();
		sumTraveling += human.GetTravelingCounter();
		++humansArrivedCounter;
	}

	if (humansArrivedCounter == 0)
	{
		return;
	}

	Log("[Stats] Average wait time (steps): ", (sumWaiting / static_cast<float>(humansArrivedCounter)));
	Log("[Stats] Average travel time (steps): ", (sumTraveling / static_cast<float>(humansArrivedCounter)));
}
