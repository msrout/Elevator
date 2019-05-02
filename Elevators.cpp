#include "MessageBus.h"

#include "Elevators.h"

Elevators::Elevators()
{
}

void
Elevators::Start()
{
	REGISTER_ELEVATOR(MessageElevatorCall,			Elevators::OnMessageElevatorCall);
	REGISTER_ELEVATOR(MessageElevatorStep,			Elevators::OnMessageElevatorStep);
	REGISTER_ELEVATOR(MessageElevatorRequest,		Elevators::OnMessageElevatorRequest);

	myElevators.push_back(Elevator{1, 10, 6, Direction::Down});
	myElevators.push_back(Elevator{2, 10, 2, Direction::Up });

	{
		MessageElevatorStep message;
		SEND_TO_ELEVATORS(message);
	}

	{
		MessageElevatorReady message;
		SEND_TO_HUMANS(message); 
	}
}

void
Elevators::OnMessageElevatorCall(
	const MessageElevatorCall&				aMessage)
{
	// Implement me!
	//Log("[Elevator]  Human has called the elevator");
	MessageElevatorArrived message;
	message.myFloor = aMessage.myFloor;
	/*
	* If the elevator ismoving in the same direction and on the current floor
	*/
	for (auto& elevator : myElevators)
	{
		//elevator.SetDestinationFloor();
		if (elevator.CurrentDirection() == aMessage.myDirection &&
			elevator.CurrentFloor()== message.myFloor)
		{
			message.myElevatorId = elevator.Id();
			SEND_TO_HUMANS(message);
			break;
		}
	}
	
}

void
Elevators::OnMessageElevatorRequest(
	const MessageElevatorRequest&			aMessage)
{
	// Implement me!

	Log("[Elevator] human has requested for floor:", aMessage.myFloor);
	MessageElevatorArrived message;
	message.myFloor = aMessage.myFloor;
	message.myElevatorId = aMessage.myElevatorId;
	SEND_TO_HUMANS(message);
	for (auto& elevator : myElevators)
	{
		/*
		* the destination floor is the requested floor
		*/
		if (aMessage.myElevatorId == elevator.Id())
		{
			elevator.SelectFloor(aMessage.myFloor);
			break;
		}	
	}
}

void
Elevators::OnMessageElevatorStep(
	const MessageElevatorStep&				aMessage)
{
	Log("[Elevators] Step");

	for (auto& elevator : myElevators)
	{
		elevator.Step();
	}

	MessageElevatorStep message;
	SEND_TO_ELEVATORS(message);
}